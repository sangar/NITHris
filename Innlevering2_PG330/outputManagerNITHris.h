////////////////////////////////////////////////////////////////////////////////
//																			  //
//	File: outputManagerNITHris.h											  //
//																			  //
//	Original author: Tomas Sandnes / NITH / 2007-2008						  //
//	Changed by:	---															  //
//																			  //
//	Description: Takes care of graphics output (drawing tiles to the screen,  //
//				 blitting, etc.).											  //
//																			  //
//	Special notes: Requires the SDL library and previous SDL_Init call.		  //
//				   Calls, if necessary, SDL_InitSubSystem(SDL_INIT_VIDEO)	  //
//				   on use of own Init() function. (Assumes external			  //
//				   SDL_Quit() AFTER destruction of own object.)				  //
//																			  //
////////////////////////////////////////////////////////////////////////////////

#ifndef OUTPUTMANAGERNITHRIS_H  // Inclusion guard.
#define OUTPUTMANAGERNITHRIS_H



// I N C L U D E S
// ===============

#include <SDL.h>
#include "nithris.h"

namespace PG330Helpers
{

// C L A S S   D E C L A R A T I O N
// =================================

class COutputManagerNITHris
{
public:
	// Constructor
	COutputManagerNITHris();
	// Destructor
	~COutputManagerNITHris();
	// Initialize needed SDL subsystems and application window stuff.
	bool Init( const char* szWindowTitle );  
	// Blits the scorearea background image.
	void DrawBackground() const;  
	// Blits the game over "window" onto the gamearea.
	void DrawGameOverMessage() const;  
	// Blits the startup "window" onto the gamearea.
	void DrawStartupMessage() const;  
	// Function to blit a tile. (At the position and with the color specified.)
	void DrawTile( short sPosX,
				   short sPosY,
				   ETileColor eColor ) const;  
	// Function to blit a 4-digit score. (At the position and with the value specified.)
	void DrawScore( short sPosX, 
					short sPosY, 
					unsigned short usScore ) const;  
	// Updates what we see on screen with any newly blitted changes. (Flips current visual buffer and current update buffer.)
	void OutputGraphics() const;  

private:
	// Helper function to load and optimize graphics.
	// Note to Students: This function has a "double pointer" as the first parameter! 
	//                   This was explained in the lecture when the game was announced.
	bool _LoadAndFormatBMP( SDL_Surface** ppoBitmapSurface, 
							const char* szBitmapName ) const;  
	// Helper function to draw a single digit.
	void _DrawScoreDigit( short sPosX, 
						  short sPosY, 
						  unsigned short usDigit ) const;

	SDL_Surface* m_poScreenSurface;      // Our output/window surface. This is the one we Blit the others to.
	SDL_Surface* m_poTileSurface;        // Surface containing tile graphics.
	SDL_Surface* m_poNumberSurface;      // Surface containing digit/number graphics (text-as-bitmap).
	SDL_Surface* m_poStartupSurface;     // Surface containing startup image.
	SDL_Surface* m_poGameOverSurface;    // Surface containing game over image.
	SDL_Surface* m_poBackgroundSurface;  // Surface containing scoreboard background image.

};  // END class COutputManagerNITHris

} // namespace PG330Helpers

#endif  // OUTPUTMANAGERNITHRIS_H