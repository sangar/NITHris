#include "CollisionDetector.h"
#include <iostream>
#include <algorithm>
using std::sort;
using std::cout;
using std::endl;

/*
* CollisionDetector.cpp
* Magnus&Gard ris v1.0
*
* Created by: Gard Sandholt and Magnus Østeng
* Date: 20.11.09
* Description:
* This class manages all collision detecting and handling.
*/

CollisionDetector::CollisionDetector(void)
{}

/*
* Method search for a combo in piecelist
*/
vector<int> CollisionDetector::findCombo(vector<Piece> &pieceList)const
{
	//Vector of y coord for combos
	vector<int> comboYCoord;
	//Cell at current row
	int numberOfCellsInLine;
	
	//Iterating gameheight
	for(int g = 0; g < g_usGAME_TILEHEIGHT; g++)
	{
		numberOfCellsInLine = 0;
		//Iterating gamewidth
		for(int h = 0; h < g_usPLAYAREA_TILEWIDTH; h++)
		{
			//Iterating piecelist
			for(int i = 0; i < pieceList.size(); i++)
			{
				Piece &currPiece = pieceList.at(i);
				Cell* tiles = currPiece.getTiles();
				//Iterating cell of currentpiece
				for(int j = 0; j < NUMBER_OF_TILES; j++)
				{
					Cell* currCell = tiles+j;
					//If cell is active, has a color and is in right y coord
					if(currCell->isActive() && currCell->getColor() != TC_BACKGROUND && 
						(currPiece.getX() + currCell->getX()) == h &&
						(currPiece.getY() + currCell->getY()) == g)
					{
						++numberOfCellsInLine;
					}
				}
			}
		}
		//If number of cells in line is equals to playarea widths the we got a combo
		if(numberOfCellsInLine == g_usPLAYAREA_TILEWIDTH)
		{
			comboYCoord.push_back(g);
			//Iterating gamearea width
			for(int h = 0; h < g_usPLAYAREA_TILEWIDTH; h++)
			{
				//Iterating piecelist
				for(int i = 0; i < pieceList.size(); i++)
				{
					Piece &currPiece = pieceList.at(i);
					Cell* tiles = currPiece.getTiles();
					for(int j = 0; j < NUMBER_OF_TILES; j++)
					{
						Cell* currCell = tiles+j;
						//Disable current cell if being a part of a combo
						if(currCell->isActive() && currCell->getColor() != TC_BACKGROUND && 
						(currPiece.getX() + currCell->getX()) == h &&
						(currPiece.getY() + currCell->getY()) == g)
						{
							currCell->disable();
						}
					}
				}
			}
		}
	}
	if(comboYCoord.size() > 0)
	{
		//Sort the yCombo vector
		sort(comboYCoord.begin(), comboYCoord.end());
		for(int g = 0; g < comboYCoord.size() ; g++)
		{
			for(int i = 0; i < pieceList.size(); i++)
			{
				Piece &currPiece = pieceList.at(i);
				Cell* tiles = currPiece.getTiles();
				for(int j = 0; j < NUMBER_OF_TILES; j++)
				{
					Cell* currCell = tiles+j;
					//Makes cells above combo fall
					if(comboYCoord.at(g) < g_usGAME_TILEHEIGHT &&
						currCell->getColor() != TC_BACKGROUND && currCell->isActive() && 
						currPiece.getY() + currCell->getY() < comboYCoord.at(g))
					{
						//If current cell is split over/under combo then update the cell y coord
						if(currPiece.getY() + 3 > comboYCoord.at(g))
						{
							currCell->setCoords(currCell->getX(), currCell->getY()+1);
						}
						//If all cells of current piece is over combo the update the whole piece y coord
						else
						{
							currPiece.getY()++;
							break;
						}
					}
				}
			}
		}
	}
	return comboYCoord;
}

/*
* Method checks if current piece can fall and update y coord
*/
bool CollisionDetector::doFall(vector<Piece> &pieceList)const
{
	bool doFall = true;
	Piece &currPiece = pieceList.back();
	Cell* currPieceTiles = currPiece.getTiles();
	for(int j = 0; j < NUMBER_OF_TILES; j++)
	{
		Cell* currCell = currPieceTiles+j;
		//Cannot fall beneath "floor"
		if(currCell->getColor() != TC_BACKGROUND &&
			(currPiece.getY() + currCell->getY()+1) >= g_usGAME_TILEHEIGHT)
		{
			doFall = false;
			break;
		}
	}
	if(!doFall)
	{	
		currPiece.pieceCollied();
		return doFall;
	}
	//Iterates piecelist, apart from itself
	for(int i = 0; i < pieceList.size()-1; i++)
	{
		Piece &p = pieceList.at(i);
		Cell* tiles = p.getTiles();
		//Iterates tiles of curr piece (the one falling)
		for(int j = 0; j < NUMBER_OF_TILES; j++)
		{
			Cell* currPieceCell = currPieceTiles+j;
			//Iterates tiles of currentPiece in list
			for(int k = 0; k < NUMBER_OF_TILES; k++)
			{
				Cell* currCell = tiles+k;
				//If currCell
				if(currCell->getColor() != TC_BACKGROUND && 
					currPieceCell->getColor() != TC_BACKGROUND &&
					currCell->isActive() &&
					(p.getY() + currCell->getY()) == (currPiece.getY() + currPieceCell->getY()+1)
					&& (p.getX() + currCell->getX()+1) == (currPiece.getX() + currPieceCell->getX() + 1))
				{
					doFall = false;
					break;
				}
			}
		}
		if(!doFall)
			break;
	}
	if(doFall)
		currPiece.getY()++;
	else
		currPiece.pieceCollied();
	return doFall;
}

/*
* Method moves the current Piece and detects collisions
*/
bool CollisionDetector::doMove(const bool right, vector<Piece> &pieceList)const
{
	bool doMove = true;
	Piece &currPiece = pieceList.back();
	
	Cell* currPieceTiles = currPiece.getTiles();
	//Iterates tiles
	for(int i = 0; i < NUMBER_OF_TILES; i++)
	{
		Cell* currCell = currPieceTiles+i;
		//Checks if piece is within playarea bounds
		if(currCell->getColor() != TC_BACKGROUND && currCell->isActive())
		{
			if((currPiece.getX() + currCell->getX() + 1) >= g_usPLAYAREA_TILEWIDTH && right)
			{
				doMove = false;
				break;
			}
			else if((currPiece.getX() + currCell->getX()) < 1 && !right)
			{
				doMove = false;
				break;
			}
		}		
	}
	if(!doMove)
		return doMove;
	//Checks if piece is moving on antother piece
	for(int i = 0; i < pieceList.size()-1; i++)
	{
		Piece &p = pieceList.at(i);
		Cell* tiles = p.getTiles();
		for(int j = 0; j < 16; j++)
		{
			Cell* currPieceCell = currPieceTiles+j;
			for(int k = 0; k < 16; k++)
			{
				Cell* currCell = tiles+k;
				/* 
				 * If currCells have a color, both are active,
				 * and have equal x and y coords, depending on which way 
				 * they are moving.
				 */
				if(currCell->getColor() != TC_BACKGROUND && 
					currPieceCell->getColor() != TC_BACKGROUND &&
					currPieceCell->isActive() &&
					currCell->isActive() &&
					(p.getY() + currCell->getY()) == (currPiece.getY() + currPieceCell->getY()) && 
					((((currPiece.getX() + currPieceCell->getX() + 1) == (p.getX() + currCell->getX())) && right) ||
					(((currPiece.getX() + currPieceCell->getX()) == (p.getX() + currCell->getX()+1)) && !right)))
				{
					doMove = false;
					break;
				}
			}
		}
		if(!doMove)
			break;
	}
	if(doMove && right)
		currPiece.getX()++;
	else if(doMove)
		currPiece.getX()--;
	return doMove;
}

/*
* Method calculates if piece can be rotated and rotates if legal action
*/
bool CollisionDetector::doRotate(const bool clockwise, vector<Piece> &pieceList)const
{
	//Step 1: do rotate some dummy tiles

	Cell rotatedTiles[4][4];
	int newX, newY;
	Cell* tiles = pieceList.back().getTiles();
	//Iterates currPiece tiles
	for(int i = 0; i < sizeof(&rotatedTiles); i++)
	{
		for(int j = 0; j < sizeof(&rotatedTiles[i]); j++)
		{
			Cell* currTile = tiles + (i*sizeof(&rotatedTiles[i]) + j);
			//Clockwise rotation
			if(clockwise)
			{
				newX = 3 - currTile->getY();
				newY = currTile->getX();
			}
			//Not clockwise rotation
			else
			{
				newX = currTile->getY();
				newY = 3 - currTile->getX();
			}
			//Set new coords and color
			rotatedTiles[i][j].setCoords(newX, newY);
			rotatedTiles[i][j].setColor(currTile->getColor());
		}
	}
	//Now we got rotated coords and may check if its a legal action
	bool doRotate = true;
	Piece &currPiece = pieceList.back();
	//Check if piece is rotating too close to the edges
	for(int i = 0; i < sizeof(&rotatedTiles); i++)
	{
		for(int j = 0; j < sizeof(&rotatedTiles[i]); j++)
		{
			Cell currCell = rotatedTiles[i][j];
			if(currCell.getColor() != TC_BACKGROUND &&
				((currPiece.getX() + currCell.getX() + 1 > g_usPLAYAREA_TILEWIDTH) ||
				(currPiece.getX() + currCell.getX()+1 < 1) ||
				(currPiece.getY() + currCell.getY() + 1 > g_usGAME_TILEHEIGHT)))
			{
				return false;
			}
		}
	}
	//Iterates rotateted tiles
	for(int i = 0; i < sizeof(&rotatedTiles); i++)
	{
		//Iterates rotateted tiles, second dimension
		for(int j = 0; j < sizeof(&rotatedTiles[i]); j++)
		{
			Cell currRotatedCell = rotatedTiles[i][j];
			//Iterates piece list
			for(int k = 0; k < pieceList.size()-1; k++)
			{
				Piece piece = pieceList.at(k);
				Cell* currCellTiles = piece.getTiles();
				//Iterates curr piece tiles
				for(int l = 0; l < NUMBER_OF_TILES; l++)
				{
					Cell* currCell = currCellTiles+l;
					/*
					* If currCell and currRotated cell has a color 
					* and they are crashing the rotation is aborted
					*/
					if(currCell->getColor() != TC_BACKGROUND &&
						currRotatedCell.getColor() != TC_BACKGROUND && 
						(piece.getX() + currCell->getX() == currPiece.getX() + currRotatedCell.getX()) &&
						(piece.getY() + currCell->getY() == currPiece.getY() + currRotatedCell.getY()))
					{
						return false;
					}
				}
			}
		}
	}
	//Rotates
	if(doRotate)
	{
		for(int i = 0; i < sizeof(&rotatedTiles); i++)
		{
			for(int j = 0; j < sizeof(&rotatedTiles[i]); j++)
			{
				Cell* currTile = tiles + (i*sizeof(&rotatedTiles[i]) + j);
				Cell rotatedCell = rotatedTiles[i][j];
				currTile->setCoords(rotatedCell.getX(), rotatedCell.getY()); 
			}
		}
	}
	return doRotate;
}


CollisionDetector::~CollisionDetector(void)
{
}

