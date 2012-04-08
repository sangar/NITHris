#ifndef _PIECE_H
#define _PIECE_H

#include "nithris.h"
#include "Cell.h"

#include <time.h>
#include <stdlib.h>
#include <vector>

using namespace PG330Helpers;
using std::vector;

const int NUMBER_OF_TILES = 16;
class Piece
{
private:
	Cell tiles[4][4];
	int xCoord, yCoord;
	bool collided;
	enum PieceType 
	{
		BOX, LEFT_STAIR, RIGHT_STAIR, 
		PYRAMID, LEFT_HOOK, RIGHT_HOOK, TOWER
	};
	PieceType pieceType;
	void getRandomPiece();
	Cell createCell(const int upperLeftXLocal, const int upperRightYLocal, const ETileColor tileColor)const;
	
public:
	Piece(void);
	~Piece(void);
	int &getX();
	int &getY();
	bool hasCollided()const;
	void pieceCollied();
	bool toRemove();
	Cell* getTiles();
	void initPiece();
};

#endif
