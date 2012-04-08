////////////////////////////////////////////////////////////////////////////////
//																			  //
//	File: inputManagerSDL.cpp												  //
//																			  //
//	Original author: Tomas Sandnes / NITH / 2008							  //
//	Changed by:      ---													  //
//																			  //
//	Descr.: A class for handling keyboard input. Keypresses are returned	  //
//			with values that matches those in the SDL file <SDL_keysym.h>.	  //
//			(Which returns the ASCII value for normal keys - however: letter  //
//			keys (a-z) are ALWAYS treated as lowercase!) Other events we 	  //
//			handle (the QUIT event) are given a NEGATIVE value (see the		  //
//			const values in "inputManagerSDL.h") as they aren't represented	  //
//			in the normal <SDL_keysym.h> range.								  //
//																			  //
//	Special notes: 1) Requires the SDL library and previous SDL_Init call.	  //
//					  Calls, if necessary, SDL_InitSubSystem(SDL_INIT_VIDEO)  //
//					  on use of own Init() function. (Assumes external		  //
//					  SDL_Quit() AFTER destruction of own object.)			  //
//				   2) To identify returned key values, see the SDL file		  //
//				      <SDL_keysym.h> for key codes (enums), as well as the	  //
//					  consts in "inputManagerSDL.h".						  //
//				   2) Letter keys (a-z) are ALWAYS treated as lowercase!	  //
//																			  //
////////////////////////////////////////////////////////////////////////////////



// I N C L U D E S
// ===============

#include <iostream>  // To use cout & endl for debug output.
using std::cout;
using std::endl;
#include <SDL.h>
#include "inputManagerSDL.h"

namespace PG330Helpers
{

// P U B L I C   F U N C T I O N S
// ===============================

CInputManagerSDL::CInputManagerSDL()
{
}  // END CInputManagerSDL



CInputManagerSDL::~CInputManagerSDL()
{
}  // END ~CInputManagerSDL



// Initialize needed SDL subsystem, as well as set keyRepeat. (Setting iKeyDelay = 0 disables it.)
bool CInputManagerSDL::Init( int iKeyDelay,			   // Defaults to 250.
							 int iKeyInterval ) const  // Defaults to 50.
{
	bool r_bRetVal = true;

	// Initialize wanted SDL module for CInputManagerSDL (We somehow need video for keyRepeat to work ...)
	if (!SDL_WasInit(SDL_INIT_VIDEO))
	{
		if (0 > SDL_Init(SDL_INIT_VIDEO))
		{
			cout << "CInputManagerSDL::Init - Cannot Initialize SDL Video SubSystem: " << SDL_GetError() << endl;
			r_bRetVal = false;
		}
	}

	if (r_bRetVal)
	{
		SDL_EnableKeyRepeat(iKeyDelay, iKeyInterval);
	}

	return r_bRetVal;

}  // END Init.



// Returns false if no keypress has been cached. (Meaning, there are no keypresses not acted upon yet.)
// NOTE 1: The param r_piKeyPressed is the return parameter for the last cached keypress (and the cache is flushed).
// NOTE 2: A cached QUIT event is always returned, no matter input source or place in cache.
bool CInputManagerSDL::GetNextKey( int* r_piKeyPressed ) const  // Return value for key pressed, if any. 
{
	return _GetTheKey(r_piKeyPressed, false);

}  // END GetNextKey



// Does NOT return until a keypress has been cached (and acted upon by this function).
// NOTE 1: The param r_piKeyPressed is the return parameter for the last cached keypress (and the cache is flushed), 
//         or the first incoming keypress we wait for, if none is cached.
// NOTE 2: A cached QUIT event is always returned, no matter input source or place in cache.
void CInputManagerSDL::GetNextKeyWait( int* r_piKeyPressed ) const  // Return value for key pressed, if any. 
{
	_GetTheKey(r_piKeyPressed, true);

}  // END GetNextKeyWait



// P R I V A T E   F U N C T I O N S
// =================================

// Internal function doing the lower-level calls.
bool CInputManagerSDL::_GetTheKey(	int* r_piKeyPressed,		 // Return value for actual keypress.
									bool bWaitIfNoneYet	) const  // If true, dont return - wait for first coming keypress.
{
	bool r_bAnyKeyPressed = false;
	SDL_Event oEvent;
	*r_piKeyPressed = g_iEVENT_NONE;

	// Get the last cached keypress. 
	// NOTE: An if-statement would get the first keypress not acted upon yet, keeping the rest in queue. 
	//       As we want to flush the queue and get the last keypress not acted upon, we do a while-statement instead.
	while (SDL_PollEvent(&oEvent))
	{
		if (SDL_QUIT == oEvent.type) 
		{
			// Exit program event detected.
			r_bAnyKeyPressed = true;
			*r_piKeyPressed = g_iEVENT_QUIT;
			break;
		}
		else if (SDL_KEYDOWN == oEvent.type)
		{
			// A key has been pressed.
			r_bAnyKeyPressed = true;
			*r_piKeyPressed = oEvent.key.keysym.sym;
		}
	}

	// If no key was cached, wait for input if relevant.
	if (bWaitIfNoneYet && !r_bAnyKeyPressed)
	{
		while (!r_bAnyKeyPressed && SDL_WaitEvent(&oEvent))
		{
			if (SDL_QUIT == oEvent.type) 
			{
				// Exit program event detected.
				r_bAnyKeyPressed = true;
				*r_piKeyPressed = g_iEVENT_QUIT;
			}
			else if (SDL_KEYDOWN == oEvent.type)
			{
				// A key has been pressed.
				r_bAnyKeyPressed = true;
				*r_piKeyPressed = oEvent.key.keysym.sym;
			}
		}
	}

	return r_bAnyKeyPressed;

}  // END _GetTheKey

} // namespace PG330Helpers

