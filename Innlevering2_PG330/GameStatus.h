#ifndef GAME_STATUS_H
#define GAME_STATUS_H


class GameStatus
{
private:
public:
	enum Status {QUIT, PLAY_GAME, MAIN_MENU, NEW_GAME_MENU};
	Status gameStatus;
	void setGameStatus(Status gameStatus);
	int getGameStatus();
	GameStatus(void);
	~GameStatus(void);
	void quit();
	void showMainMenu();
	void showNewMainMenu();
	void playGame();
};
#endif
