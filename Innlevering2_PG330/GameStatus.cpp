#include "GameStatus.h"
/*
* GameStatus.cpp
* Magnus&Gard ris v1.0
*
* Created by: Gard Sandholt and Magnus Østeng
* Date: 20.11.09
* Description:
* Value object, maintains a GameStaus enum
* by GETTERS and SETTTERS
*/
GameStatus::GameStatus(void)
{
}

GameStatus::~GameStatus(void)
{
}
void GameStatus::setGameStatus(Status status)
{
	gameStatus = status;
}
int GameStatus::getGameStatus()
{
	return gameStatus;
}
void GameStatus::quit()
{
	gameStatus = QUIT;
}
void GameStatus::showMainMenu()
{
	gameStatus = MAIN_MENU;
}
void GameStatus::showNewMainMenu()
{
	gameStatus = NEW_GAME_MENU;
}
void GameStatus::playGame()
{
	gameStatus = PLAY_GAME;
}
