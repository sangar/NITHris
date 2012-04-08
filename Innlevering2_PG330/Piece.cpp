#include "Piece.h"
#include "InitializationException.h"

#include <iostream>
using std::cout;
using std::endl;

/*
* Piece.cpp
* Magnus&Gard ris v1.0
*
* Created by: Gard Sandholt and Magnus Østeng
* Date: 20.11.09
* Description:
* This class represents a piece.
*/
Piece::Piece(void)
{}

/*
* Inits the piece, by getting a 
* random piceType and set a color and cells 
*/
void Piece::initPiece()
{
	xCoord = 2;
	yCoord = -4;
	collided = false;
	getRandomPiece();

	ETileColor tileColor;
	switch(pieceType)
	{
	case BOX:
		tileColor = TC_BLUE;
		break;
	case LEFT_STAIR:
		tileColor = TC_CYAN;
		break;
	case RIGHT_STAIR:
		tileColor = TC_GREEN;
		break;
	case PYRAMID:
		tileColor = TC_YELLOW;
		break;
	case LEFT_HOOK:
		tileColor = TC_ORANGE;
		break;
	case RIGHT_HOOK:
		tileColor = TC_MAGENTA;
		break;
	case TOWER:
		tileColor = TC_RED;
		break;
	default:
		throw InitializationException("Initialization exception", "Unable a gameobject!");;
	}
	for(int i = 0; i < sizeof(&tiles); i++)
	{
		for(int j = 0; j < sizeof(&tiles[i]); j++)
		{
			ETileColor currColor = TC_BACKGROUND;
			switch(pieceType)
			{
			case BOX:
				if((i == 1 && j == 1) || (i == 2 && j == 1) || (i == 1 && j == 2) || (i == 2 && j == 2))
					currColor = tileColor;
				break;
			case LEFT_STAIR:
				if((i == 0 && j == 1) || (i == 1 && j == 1) || (i == 1 && j == 2) || (i == 2 && j == 2))
				{
					currColor = tileColor;
				}
				break;
			case RIGHT_STAIR:
				if((i == 1 && j == 1) || (i == 2 && j == 1) || (i == 0 && j == 2) || (i == 1 && j == 2))
				{
					currColor = tileColor;
				}
				break;
			case PYRAMID:
				if((i == 1 && j == 1) || (i == 0 && j == 2) || (i == 1 && j == 2) || (i == 2 && j == 2))
				{
					currColor = tileColor;
				}
				break;
			case LEFT_HOOK:
				if((i == 1 && j == 1) || (i == 2 && j == 1) || (i == 2 && j == 2) || (i == 2 && j == 3))
				{
					currColor = tileColor;
				}
				break;
			case RIGHT_HOOK:
				if((i == 1 && j == 1) || (i == 2 && j == 1) || (i == 1 && j == 2) || (i == 1 && j == 3))
				{
					currColor = tileColor;
				}
				break;
			case TOWER:
				if((i == 1 && j == 0) || (i == 1 && j == 1) || (i == 1 && j == 2) || (i == 1 && j == 3))
				{
					currColor = tileColor;
				}
				break;
			default:
				throw InitializationException("Initialization exception", "Unable to init a gameobject!");
			}
			tiles[i][j] = createCell(i, j, currColor);
		}
	}
}
/*
* Methods creates a cell
*/
Cell Piece::createCell(const int upperLeftXLocal, const int upperRightYLocal, const ETileColor tileColor)const
{
	return Cell(upperLeftXLocal, upperRightYLocal, tileColor);
}
/*
* Methods set a new random piecetype
*/
void Piece::getRandomPiece()
{
	srand(static_cast<int>(time(0)));
	int randomTile = (rand() % 7);
	switch(randomTile)
	{
	case 0:
		pieceType = BOX;
		break;
	case 1:
		pieceType = LEFT_STAIR;
		break;
	case 2:
		pieceType = RIGHT_STAIR;
		break;
	case 3:
		pieceType = PYRAMID;
		break;
	case 4:
		pieceType = LEFT_HOOK;
		break;
	case 5:
		pieceType = RIGHT_HOOK;
		break;
	case 6:
		pieceType = TOWER;
		break;
	default:
		throw InitializationException("Initialization exception", "Unable to init a gameobject!");
	}
}
/*
* Methods checks if the piece should be removed
*/
bool Piece::toRemove()
{
	bool toRemove = true;
	for(int i = 0; i < sizeof(tiles); i++)
	{
		for(int j = 0; j < sizeof(tiles[i]); j++)
		{
			Cell &currCell = tiles[i][j];
			if(currCell.isActive() && 
				currCell.getColor() != TC_BACKGROUND &&
				yCoord + currCell.getY() < 20)
			{
				toRemove = false;
				break;
			}
		}
		if(!toRemove)
			break;
	}
	return toRemove;
}
//GETTERS AND SETTERS
Piece::~Piece(void)
{
}
int &Piece::getX()
{
	return xCoord;
}
int &Piece::getY()
{
	return yCoord;
}
bool Piece::hasCollided()const
{
	return collided;
}
void Piece::pieceCollied()
{
	collided = true;
}
Cell* Piece::getTiles()
{
	return *tiles;
}

