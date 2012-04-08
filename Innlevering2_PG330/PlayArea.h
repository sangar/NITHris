#ifndef PlayArea_H
#define PlayArea_H

#include "Piece.h"
#include "outputManagerNITHris.h"
#include "GameStatus.h"

#include <vector>
#include <string>
#include <list>

using namespace PG330Helpers; 

const int HIGHSCORE_TOP = 5;
class PlayArea
{
private:
	COutputManagerNITHris* outputManager;
	void clear()const;
	void updatePreview(Piece currPiecePreview)const;
	void setScore(const unsigned long score)const;
	std::string showInputDialog();
public:
	PlayArea();
	~PlayArea(void);
	void init(int &argc, char* argv[]);
	void showMenu(GameStatus gameStatus)const;
	void showHighScore()const;
	void operator<<(const int score)const;
	void drawGameObjects(const vector<Piece> pieceList, const Piece currPiecePreview, const unsigned long score)const;
};

#endif
