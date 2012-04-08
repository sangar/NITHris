////////////////////////////////////////////////////////////////////////////////
//																			  //
//	File: nithris.h															  //
//																			  //
//	Original author: Tomas Sandnes / NITH / 2008							  //
//	Changed by:	---															  //
//																			  //
//	Description: A few enums and constants that are							  //
//				 used throughout the game.									  //
//																			  //
//	Special notes: ---														  //
//																			  //
////////////////////////////////////////////////////////////////////////////////

#ifndef NITHRIS_H  // Inclusion guard.
#define NITHRIS_H


namespace PG330Helpers
{

// D E F I N E S   /   C O N S T A N T S   /   . . .
// =================================================

enum ETileColor { TC_NO_DRAW = -1,
				  TC_BACKGROUND,
				  TC_ORANGE,
				  TC_RED,
				  TC_GREEN,
				  TC_BLUE,
				  TC_CYAN,
				  TC_MAGENTA,
				  TC_YELLOW       };

const unsigned short g_usPLAYAREA_TILEWIDTH = 10;  // Width, in tiles, of playarea.
const unsigned short g_usSCOREAREA_TILEWIDTH = 8;  // Width, in tiles, of scorearea.
const unsigned short g_usGAME_TILEHEIGHT = 20;     // Height, in tiles, of application.
const unsigned short g_usPREVIEWPIECE_POS_X = 12;  // X-pos (measured in tiles) for upper left corner of preview piece.
const unsigned short g_usPREVIEWPIECE_POS_Y = 3;   // Y-pos (measured in tiles) for upper left corner of preview piece.
const unsigned short g_usSCORE_POS_X = 12;		   // X-pos (measured in tiles) for most significant digit of score value.
const unsigned short g_usSCORE_POS_Y = 11;		   // Y-pos (measured in tiles) for most significant digit of score value.

} // namespace PG330Helpers

#endif  // NITHRIS_H