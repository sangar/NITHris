////////////////////////////////////////////////////////////////////////////////
//																			  //
//	File: outputManagerNITHris.cpp											  //
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



// I N C L U D E S
// ===============

#include <iostream>  // For cout, endl.
using std::cout;
using std::endl;
#include "outputManagerNITHris.h"

namespace PG330Helpers
{

// D E F I N E S   /   C O N S T A N T S   /   . . .
// =================================================

// (NOTE: Static globals are only visible (only have scope) in this file.)
static const unsigned short g_usTILE_PIXELWIDTH =    32;  // Tiles will be this amount of pixels large (squared).
static const unsigned short g_usWINDOW_PIXELWIDTH =  (g_usPLAYAREA_TILEWIDTH + g_usSCOREAREA_TILEWIDTH) * g_usTILE_PIXELWIDTH; // Window width in pixels.
static const unsigned short g_usWINDOW_PIXELHEIGHT = g_usGAME_TILEHEIGHT * g_usTILE_PIXELWIDTH; // Window height in pixels.
static const unsigned short g_usWINDOW_BIT_DEPTH =   16;  // Our bit depth (number of colors and such).
static const unsigned int   g_uiSDL_FLAGS =          SDL_HWSURFACE | SDL_DOUBLEBUF;  // Flags for SDL: We're using graphic card memory (opposed to system memory), and double-buffering.

static const char* g_szTILE_BMP =       "Graphics//tiles_32x32.bmp";      // Tiles as one bmp.
static const char* g_szNUMBER_BMP =     "Graphics//numbers_32x32.bmp";    // Scoreboard numbers as one bmp.
static const char* g_szSTARTUP_BMP =    "Graphics//startup.bmp";          // Startup (main menu) image.
static const char* g_szGAMEOVER_BMP =   "Graphics//gameover.bmp";         // Game over image.
static const char* g_szBACKGROUND_BMP = "Graphics//scoreboard_back.bmp";  // Scoreboard background image.



// P U B L I C   F U N C T I O N S
// ===============================

COutputManagerNITHris::COutputManagerNITHris()
{
	// NOTE: It's important to set the surface pointers to 0 in the constructor. 
	//       Se comments in the destructor below for more information.
	m_poScreenSurface = 0;
	m_poTileSurface = 0;
	m_poNumberSurface = 0;
	m_poStartupSurface = 0;
	m_poGameOverSurface = 0;
	m_poBackgroundSurface = 0;

}  // END COutputManagerNITHris



COutputManagerNITHris::~COutputManagerNITHris()
{
	// Free all surfaces.
	// NOTE: It's important to set the surface pointers to 0 in the constructor. 
	//       If we fail during init (graphics in the wrong place, for example), 
	//       pointers with no content will then still be 0 when we enter the destructor to clean up, 
	//       and by checking for null-pointers here, we only free initialized stuff. 
	//       (Without null-pointers to start with, we would crash here if we failed during init.)
	if (m_poBackgroundSurface)
	{
		SDL_FreeSurface(m_poBackgroundSurface);
		m_poBackgroundSurface = 0;
	}
	if (m_poGameOverSurface)
	{
		SDL_FreeSurface(m_poGameOverSurface);
		m_poGameOverSurface = 0;
	}
	if (m_poStartupSurface)
	{
		SDL_FreeSurface(m_poStartupSurface);
		m_poStartupSurface = 0;
	}
	if (m_poNumberSurface)
	{
		SDL_FreeSurface(m_poNumberSurface);
		m_poNumberSurface = 0;
	}
	if (m_poTileSurface)
	{
		SDL_FreeSurface(m_poTileSurface);
		m_poTileSurface = 0;
	}
	if (m_poScreenSurface)
	{
		SDL_FreeSurface(m_poScreenSurface);
		m_poScreenSurface = 0;
	}

}  // END ~COutputManagerNITHris



// Initialize needed SDL subsystems and application window stuff.
bool COutputManagerNITHris::Init( const char* szWindowTitle )
{
	bool r_bRetVal = true;

	// Initialize wanted SDL module for COutputManager (We only really need video - add other modules if you want to add sounds, etc. :-)
	if (!SDL_WasInit(SDL_INIT_VIDEO))
	{
		if (0 > SDL_Init(SDL_INIT_VIDEO))
		{
			cout << "COutputManagerNITHris::Init - Cannot Initialize SDL Video SubSystem: " << SDL_GetError() << endl;
			r_bRetVal = false;
		}
	}

	if (r_bRetVal)
	{
		// Create a graphics output screen/window.
		m_poScreenSurface = SDL_SetVideoMode(g_usWINDOW_PIXELWIDTH, g_usWINDOW_PIXELHEIGHT, g_usWINDOW_BIT_DEPTH, g_uiSDL_FLAGS);
		if (!m_poScreenSurface) 
		{
			cout << "COutputManagerNITHris::Init - Cannot set Video Mode:" << SDL_GetError() << endl;
			r_bRetVal = false;
		}
		else
		{
			//Set the Window Caption.
			SDL_WM_SetCaption(szWindowTitle, szWindowTitle);

			// Init bitmaps. (As long as we don't get any error, we move on to the next.)
			r_bRetVal = _LoadAndFormatBMP(&m_poTileSurface, g_szTILE_BMP);
			if(r_bRetVal) 
			{
				r_bRetVal = _LoadAndFormatBMP(&m_poNumberSurface, g_szNUMBER_BMP);
				if(r_bRetVal) 
				{
					r_bRetVal = _LoadAndFormatBMP(&m_poStartupSurface, g_szSTARTUP_BMP);
					if(r_bRetVal) 
					{
						r_bRetVal = _LoadAndFormatBMP(&m_poGameOverSurface, g_szGAMEOVER_BMP);
						if(r_bRetVal) 
						{
							r_bRetVal = _LoadAndFormatBMP(&m_poBackgroundSurface, g_szBACKGROUND_BMP);
						}
					}
				}
			}
		}
	}

	return r_bRetVal;

}  // END Init



// Blits the scorearea background image.
void COutputManagerNITHris::DrawBackground() const
{
	// Paste the scoreboard background image.
	SDL_Rect oBackRect;
	oBackRect.x = g_usPLAYAREA_TILEWIDTH * g_usTILE_PIXELWIDTH;
	oBackRect.y = 0;
	SDL_BlitSurface(m_poBackgroundSurface, 0, m_poScreenSurface, &oBackRect);

}  // END DrawBackground



// Blits the game over "window" onto the gamearea.
void COutputManagerNITHris::DrawGameOverMessage() const
{
	SDL_Rect sScreenRect;  // Part of screen we want to draw to.

	sScreenRect.x = 30; 
	sScreenRect.y = 245;

	// Blit Game over image to the screen.
	SDL_BlitSurface(m_poGameOverSurface, 0, m_poScreenSurface, &sScreenRect);

}  // END DrawGameOverMessage



// Blits the startup "window" onto the gamearea.
void COutputManagerNITHris::DrawStartupMessage() const
{
	SDL_Rect sScreenRect;  // Part of screen we want to draw to.

	sScreenRect.x = 30; 
	sScreenRect.y = 245;

	// Blit Game over image to the screen.
	SDL_BlitSurface(m_poStartupSurface, 0, m_poScreenSurface, &sScreenRect);

}  // END DrawStartupMessage



// Function to blit a tile. (At the position and with the color specified.)
void COutputManagerNITHris::DrawTile( short      sPosX, 
									  short      sPosY, 
									  ETileColor eColor ) const
{
	if (TC_NO_DRAW != eColor)
	{
		SDL_Rect oScreenRect;  // Part of screen we want to draw to.
		SDL_Rect oTileRect;  // Part of tileset (all tiles are stored in a single bmp, and we use a small part of it) we want to draw from.

		// Setting up where on screen we want to draw/blit.
		oScreenRect.x = sPosX * g_usTILE_PIXELWIDTH; 
		oScreenRect.y = sPosY * g_usTILE_PIXELWIDTH;

		// Setting up what part of the tile-bmp we want to use.
		oTileRect.x = eColor * g_usTILE_PIXELWIDTH;   
		oTileRect.y = 0;
		oTileRect.w = g_usTILE_PIXELWIDTH;   
		oTileRect.h = g_usTILE_PIXELWIDTH;

		// When positions are set, blit the wanted tile to the screen.
		SDL_BlitSurface(m_poTileSurface, &oTileRect, m_poScreenSurface, &oScreenRect);
	}

}  // END DrawTile



// Function to blit a 4-digit score. (At the position and with the value specified.)
void COutputManagerNITHris::DrawScore( short          sPosX, 
									   short          sPosY, 
									   unsigned short usScore ) const
{
	// Render the scoretext output.
	unsigned short usTempScore = usScore;
	unsigned short usDigit = 0;
	for (unsigned short usDigitSpot = 0; usDigitSpot < 4; ++usDigitSpot)
	{
		usDigit = usTempScore % 10;
		usTempScore /= 10;
		_DrawScoreDigit(sPosX + (3 - usDigitSpot), sPosY, usDigit);
	}

}  // END DrawScore



// Updates what we see on screen with any newly blitted changes. (Flips current visual buffer and current update buffer.)
void COutputManagerNITHris::OutputGraphics() const
{
	//Redraw the entire Display.
	SDL_Flip(m_poScreenSurface);

}  // END OutputGraphics



// P R I V A T E   F U N C T I O N S
// =================================

// Helper function to load and optimize graphics.
// Note to Students: This function has a "double pointer" as the first parameter! 
//                   This was explained in the lecture when the game was announced.
bool COutputManagerNITHris::_LoadAndFormatBMP( SDL_Surface** r_ppoBitmapSurface, 
											   const char*   szBitmapName       ) const
{
	bool r_bRetVal = true;

	// First we load the bitmap into a temporary storage.
	SDL_Surface *poTempSurface = SDL_LoadBMP(szBitmapName);
	if(!poTempSurface) 
	{
		cout << "COutputManagerNITHris::Init - File not Found: " << szBitmapName << endl;
		r_bRetVal = false;
	}
	else
	{
		// Then we convert the temporary stored bitmap to the ideal format and put it in the variable pointed to in the param list.
		*r_ppoBitmapSurface = SDL_DisplayFormat(poTempSurface);

		// Finally, we free the dynamically created temporary storage again.
		SDL_FreeSurface(poTempSurface);
	}

	return r_bRetVal;

}  // END _LoadAndFormatBMP



// Helper function to draw a single digit.
void COutputManagerNITHris::_DrawScoreDigit( short          sPosX, 
											 short          sPosY, 
											 unsigned short usDigit ) const
{
	SDL_Rect oScreenRect;  // Part of screen we want to draw to.
	SDL_Rect oNumberRect;  // Part of the number-bmp-file (all numbers are stored in a single bmp, and we use a small part of it) we want to draw from.

	oScreenRect.x = sPosX * g_usTILE_PIXELWIDTH; 
	oScreenRect.y = sPosY * g_usTILE_PIXELWIDTH;

	oNumberRect.x = usDigit * g_usTILE_PIXELWIDTH;   
	oNumberRect.y = 0;
	oNumberRect.w = g_usTILE_PIXELWIDTH;   
	oNumberRect.h = g_usTILE_PIXELWIDTH;

	// When positions are set, blit the wanted tile to the screen.
	SDL_BlitSurface(m_poNumberSurface, &oNumberRect, m_poScreenSurface, &oScreenRect);

}  // END _DrawScoreDigit

} // namespace PG330Helpers