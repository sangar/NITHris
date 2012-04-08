#ifndef GameManager_H
#define GameManager_H

#include "GameStatus.h"
#include "inputManagerSDL.h"
#include "loopTimerSDL.h"
#include "PlayArea.h"

using namespace PG330Helpers;
class GameManager
{
private:
	GameStatus gameStatus;
	void runGame(PlayArea playArea, CInputManagerSDL inputManager, 
		unsigned long &score);
public:
	GameManager(void);
	~GameManager(void);
	void initGame(int &argc, char* argv[]);
};

#endif
