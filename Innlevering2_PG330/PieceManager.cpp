#include "PieceManager.h"
#include <iostream>
using std::cout;
using std::endl;

/*
* PieceManager.cpp
* Magnus&Gard ris v1.0
*
* Created by: Gard Sandholt and Magnus Østeng
* Date: 20.11.09
* Description:
* This class represents the layer between GameManager and all other classes.
* GameManager simply distributes the reposibility the PieceManager
*/
PieceManager::PieceManager(void)
{}
/*
* Method inits the play area
*/
void PieceManager::init(PlayArea newPlayArea)
{
	playArea = newPlayArea;
	Piece currPiece;
	currPiece.initPiece();
	currPiecePreview.initPiece();
	pieceList.push_back(currPiece);
}
/*
* Methods updates the piece, which means falling
*/
bool PieceManager::updatePiece(CLoopTimer &pieceTimer, GameStatus &gameStatus, unsigned long &score)
{
	if(gameStatus.getGameStatus() == gameStatus.PLAY_GAME)
	{
		//Checks if falling
		if(!collisionDetector.doFall(pieceList))
		{
			//Running collision precedure
			collisionProcedure(currPiecePreview, pieceTimer, gameStatus, score);
			return true;
		}
		//No crash, draw pieces
		else
			playArea.drawGameObjects(pieceList, currPiecePreview, score);
		return false;
	}
	return true;
}
/*
* Method moves the piece
*/
void PieceManager::movePiece(const bool right, unsigned long score)
{
	if(collisionDetector.doMove(right, pieceList))
		playArea.drawGameObjects(pieceList, currPiecePreview, score);
}
/*
* Method rotates the piece
*/
void PieceManager::rotatePiece(const bool clockwise, unsigned long score)
{
	if(collisionDetector.doRotate(clockwise, pieceList))
		playArea.drawGameObjects(pieceList, currPiecePreview, score);
}
void PieceManager::collisionProcedure(Piece &currPiecePreview, CLoopTimer &pieceTimer, 
									  GameStatus &gameStatus, unsigned long &score)
{
	//Add currPiecePreview to back of the list, means that its the new "currPiece"
	pieceList.push_back(currPiecePreview);
	//Resets currPiecePreview
	currPiecePreview.initPiece();
	//Draw gameobjects
	playArea.drawGameObjects(pieceList, currPiecePreview, score);
	//Check for combos
	const vector<int> comboYCoord = collisionDetector.findCombo(pieceList);

	//A combo detected
	if(comboYCoord.size() > 0)
	{
		//Add timers updates per intervall
		pieceTimer.SetUpdatesPerInterval(pieceTimer.getUpdatesPerIntervall()+ 1);
		pieceTimer.Start();
		//Updates score
		score += (comboYCoord.size() * comboYCoord.size()*5);
		//Check for pieces to remove
		for(int i = 0; i < pieceList.size(); i++)
		{
			if(pieceList.at(i).toRemove())
			{
				vector<Piece>::iterator iterator = pieceList.begin()+i;
				pieceList.erase(iterator);
			}
		}
	}
	//Detects gameover
	for(int i = 0; i < pieceList.size(); i++)
	{
		Piece &currPiece = pieceList.at(i);
		if(currPiece.hasCollided())
		{
			Cell* tiles = currPiece.getTiles();
			for(int j = 0; j < NUMBER_OF_TILES; j++)
			{
				Cell* currTile = tiles+j;
				if(currTile->isActive() && currTile->getColor() != TC_BACKGROUND &&
					currPiece.getY() + currTile->getY() < 0)
				{
					gameStatus.showMainMenu();
					return;
				}
			}
		}
	}
}
PieceManager::~PieceManager(void)
{
}
