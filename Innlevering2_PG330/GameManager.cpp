#include "GameManager.h"
#include "PieceManager.h"
#include "InitializationException.h"

#include <iostream>
#include <windows.h>

using std::cout;
using std::endl;

/*
* GameManager.cpp
* Magnus&Gard ris v1.0
*
* Created by: Gard Sandholt and Magnus Østeng
* Date: 20.11.09
* Description:
* This class manages the game flow, not actual game logics.
*/
GameManager::GameManager(void)
{}
void GameManager::initGame(int &argc, char* argv[])
{
	try
	{
		//Instansiating
		PlayArea playArea;
		CInputManagerSDL inputManager;

		//Init playarea
		playArea.init(argc, argv);
		unsigned long score = 0;
		gameStatus.showNewMainMenu();

		//Game controller loop
		while(gameStatus.getGameStatus() != gameStatus.QUIT)
		{
			//Show menu
			if(gameStatus.getGameStatus() == gameStatus.MAIN_MENU
				|| gameStatus.getGameStatus() == gameStatus.NEW_GAME_MENU)
			{
				//Display menu
				playArea.showMenu(gameStatus);
				int key = 0;
				//Show main menu while not any given commands are actived 
				while(true)
				{
					//Wait for user input
					inputManager.GetNextKeyWait(&key);
					//Quit command
					if(key == SDLK_ESCAPE || key == g_iEVENT_QUIT)
					{
						gameStatus.quit();
						break;
					}
					//Play game command
					else if(key == SDLK_SPACE)
					{	
						gameStatus.playGame();
						break;
					}
					//Show highscore command
					else if(static_cast<char>(key) == 'h' || static_cast<char>(key) == 'H')
						playArea.showHighScore();
				}
			}
			else//Run game
			{
				//Reset score
				score = 0;
				//Run the game
				cout << "Run game" << endl;
				runGame(playArea, inputManager, score);
				//Attempt to save users highscore
				playArea << score;
			}
		}
	}
	catch(InitializationException initException)
	{
		string errorMessageString = initException.what();
		
		size_t length = errorMessageString.length() + 1;
		char* buffer = new char[length];
		strcpy_s(buffer, length, errorMessageString.c_str());
	
		MessageBox(NULL, buffer, "Error", MB_OK);
		delete [] buffer;
	}
}
/*
* RunGame is running the actual game logics.
*/
void GameManager::runGame(PlayArea playArea, CInputManagerSDL inputManager,
			 unsigned long &score)
{
	//Instansiating piece manager
	PieceManager pieceManager;
	pieceManager.init(playArea);

	//Instansiating timers
	CLoopTimer mainLoopTimer;
	mainLoopTimer.SetUpdatesPerInterval(60);

	CLoopTimer pieceTimer;
	pieceTimer.SetUpdatesPerInterval(20);
	pieceTimer.SetIntervalDuration(1000*10);

	CLoopTimer fastActionTimer;
	fastActionTimer.SetUpdatesPerInterval(10);
	fastActionTimer.SetIntervalDuration(1000);
	
	//Runtime variables
	int key = 0, fastMoveTimeout = 25;
	bool collision;
	bool fastKeyActivated = false;

	//Starting timers
	mainLoopTimer.Start();
	pieceTimer.Start();
	fastActionTimer.Start();
	
	//Main game loop
	while(key != g_iEVENT_QUIT  && key != SDLK_ESCAPE && 
		gameStatus.getGameStatus() == gameStatus.PLAY_GAME)
	{
		//No user input -> fall
		while(!fastKeyActivated && !inputManager.GetNextKey(&key) && 
			gameStatus.getGameStatus() == gameStatus.PLAY_GAME)
		{
			//Piecetimer sleeps
			pieceTimer.SleepUntilUpdate();
			//Update the current pice -> make it fall
			pieceManager.updatePiece(pieceTimer, gameStatus, score);
			
		}
		//Pause activated
		if(static_cast<char>(key) == 'p' || static_cast<char>(key) == 'P')
		{
			inputManager.GetNextKeyWait(&key);
			pieceTimer.Start();
		}
		//Reset fastKeyActiveted
		fastKeyActivated = false;

		//Testing userinput
		switch(key)
		{
			//Move up
			case SDLK_UP:
				fastMoveTimeout = 25;
				//Running fast rotate
				while(key == SDLK_UP || fastMoveTimeout > 0)
				{
					if(key == SDLK_UP)
						pieceManager.rotatePiece(true, score);

					//Prevent user from spamming rotate in order to not fall
					if(pieceTimer.IsTimeForUpdate())
						if(pieceManager.updatePiece(pieceTimer, gameStatus, score))
							break;
					fastActionTimer.SleepUntilUpdate();
					inputManager.GetNextKey(&key);

					//Update the timer
					if(key == g_iEVENT_NONE)
					{
						fastMoveTimeout--;
					}
					//Antoher game related key input, will break loop
					else if(key == SDLK_LEFT || key == SDLK_RIGHT || key == SDLK_DOWN)
					{
						fastKeyActivated = true;
						break;
					}
				}
				break;
			//Drop down
			case SDLK_DOWN:
				key = -1;
				collision = false;

				/*
				* Dropping while theres no followed attempt to break and no
				* collision detected
				*/
				while(!inputManager.GetNextKey(&key) && !collision)
				{	
					mainLoopTimer.SleepUntilUpdate();
					collision = pieceManager.updatePiece(pieceTimer, gameStatus, score);
				}
				break;
			//Move commands
			case SDLK_RIGHT:
			case SDLK_LEFT:
				fastMoveTimeout = 25;
				//Running fast move
				while(key == SDLK_RIGHT || key == SDLK_LEFT || fastMoveTimeout > 0)
				{
					if(key == SDLK_RIGHT || key == SDLK_LEFT)
						pieceManager.movePiece((key == SDLK_RIGHT ? true : false), score);

					//Prevent user from spamming rotate in order to not fall
					if(pieceTimer.IsTimeForUpdate())
						if(pieceManager.updatePiece(pieceTimer, gameStatus, score))
							break;

					fastActionTimer.SleepUntilUpdate();
					inputManager.GetNextKey(&key);
					
					//Update the timer
					if(key == g_iEVENT_NONE)
					{
						fastMoveTimeout--;
					}
					//Antoher game related key input, will break loop
					else if(key == SDLK_UP || key == SDLK_DOWN)
					{
						fastKeyActivated = true;
						break;
					}
				}
				break;
		}//End user command switch

		//Main loop timer sleeps
		mainLoopTimer.SleepUntilUpdate();

	}//End main loop

	//Ending loop 
	if(key == g_iEVENT_QUIT)
		gameStatus.quit();
	else
		gameStatus.showMainMenu();
}

GameManager::~GameManager(void)
{
}
