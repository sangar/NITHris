#ifndef PieceManager_H
#define PieceManager_H

#include "CollisionDetector.h"
#include "PlayArea.h"
#include "Piece.h"
#include "loopTimerSDL.h"
#include "GameStatus.h"

#include <vector>

using namespace PG330Helpers;
using namespace std;

class PieceManager
{
private:
	PlayArea playArea;
	Piece currPiecePreview;
	vector<Piece> pieceList; 
	CollisionDetector collisionDetector;
	void collisionProcedure(Piece &currPiecePreview, CLoopTimer &pieceTimer, 
		GameStatus &gameStatus, unsigned long &score);
public:
	PieceManager(void);
	~PieceManager(void);
	void init(PlayArea newPlayArea);
	bool updatePiece(CLoopTimer &pieceTimer, GameStatus &gameStatus, unsigned long &score);
	void rotatePiece(bool clockwise, unsigned long score);
	void movePiece(bool right, unsigned long score);
};
#endif
