////////////////////////////////////////////////////////////////////////////////
//																			  //
//	File: inputManagerSDL.h													  //
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

#ifndef INPUT_MANAGER_SDL_H  // Inclusion guard.
#define INPUT_MANAGER_SDL_H

namespace PG330Helpers
{

// D E F I N E S   /   C O N S T A N T S   /   . . .
// =================================================

// NOTE: All OTHER return codes are found in <SDL_keysym.h>.
const int g_iEVENT_NONE = -1;	// Return code, meaning: "No input specified/unable to identify input."
const int g_iEVENT_QUIT = -10;  // Return code, meaning: "User application quit event detected."
const int c_iKEY_ARROWUP =		-11;	// Return code for [ARROW UP] key.
const int c_iKEY_ARROWDOWN =	-12;	// Return code for [ARROW DOWN] key.
const int c_iKEY_ARROWLEFT =	-13;	// Return code for [ARROW LEFT] key.
const int c_iKEY_ARROWRIGHT =	-14;	// Return code for [ARROW RIGHT] key.


// C L A S S   D E C L A R A T I O N
// =================================

class CInputManagerSDL
{
public:
	// Constructor.
	CInputManagerSDL();
	// Destructor.
	~CInputManagerSDL();
	// Initialize needed SDL subsystem, as well as set keyRepeat. (Setting iKeyDelay = 0 disables it.)
	bool Init( int iKeyDelay = 250, 
			   int iKeyInterval = 50 ) const;
	// Returns false if no keypress has been cached. (Meaning, there are no keypresses not acted upon yet.)
	// NOTE 1: The param r_piKeyPressed is the return parameter for the last cached keypress (and the cache is flushed).
	// NOTE 2: A cached QUIT event is always returned, no matter input source or place in cache.
	bool GetNextKey( int* r_piKeyPressed ) const;  
	// Does NOT return until a keypress has been cached (and acted upon by this function).
	// NOTE 1: The param r_piKeyPressed is the return parameter for the last cached keypress (and the cache is flushed), 
	//         or the first incoming keypress we wait for, if none is cached.
	// NOTE 2: A cached QUIT event is always returned, no matter input source or place in cache.
	void GetNextKeyWait( int* r_piKeyPressed ) const;  

private:
	// Internal function doing the lower-level calls.
	bool _GetTheKey( int* r_piKeyPressed,
					 bool bWaitIfNoneYet ) const;  
};


} // namespace PG330Helpers
#endif  // INPUT_MANAGER_SDL_H
