#include "PlayArea.h"
#include "Win32InputBox.h"
#include "Score.h"

#include <SDL.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <algorithm>
#include <sstream>

#include "InitializationException.h"
#include "IOException.h"

using namespace std;

/*
* PlayArea.cpp
* Magnus&Gard ris v1.0
*
* Created by: Gard Sandholt and Magnus Østeng
* Date: 20.11.09
* Description:
* This class is responsible for handling all drawing
*/

PlayArea::PlayArea()
{}

/*
* Method inits playarea
*/
void PlayArea::init(int &argc, char* argv[])
{
	cout << "Init playarea!" << endl;
	if(SDL_Init(0) < 0)
	{
		throw InitializationException("Initialization exception", "Unable to init SDL!");
	}
	outputManager = new COutputManagerNITHris();
	outputManager->Init("Magnus&Gard ris");
	outputManager->DrawBackground();
	outputManager->OutputGraphics();
}
/*
* Method shows menu
*/
void PlayArea::showMenu(GameStatus gameStatus)const
{
	if(gameStatus.getGameStatus() == gameStatus.NEW_GAME_MENU)
		outputManager->DrawStartupMessage();
	else
		outputManager->DrawGameOverMessage();
	outputManager->OutputGraphics();
}
/*
* Methods shows highScore
*/
void PlayArea::showHighScore()const
{
	try
	{
		string fileName = "highScores.txt";
		ifstream oFileIn(fileName.c_str());
		string resultString = "";
		string nextLine;
		string name;
		string score;
		
		//Reading file
		while(!oFileIn.eof())
		{
			if(!oFileIn.good())
				throw IOException("IOException", "Error while reading: " + fileName);
			nextLine = "";
			getline(oFileIn, nextLine);
			resultString += nextLine + "\n";
			getline(oFileIn, nextLine);
			resultString += nextLine + "\n";
		}
		oFileIn.close();
		const char* results = resultString.c_str();
		MessageBox(NULL, results, "HIGH SCORES", MB_OK);
	}
	catch(IOException ioex)
	{
		string errorMessageString = ioex.what();
		
		size_t length = errorMessageString.length() + 1;
		char* buffer = new char[length];
		strcpy_s(buffer, length, errorMessageString.c_str());
	
		MessageBox(NULL, buffer, "IOException", MB_OK);
		delete [] buffer;
	}
}
/*
* Methods attempts to register a new score, if among top five
*/
void PlayArea::operator <<(const int score)const
{
	try
	{
		string fileName = "highScores.txt";
		ifstream oFileIn(fileName.c_str(), ios::in);
		string nextLine;
		vector<Score> highScores;
		Score sortedHighScores[5];
		char buff[100] = {0};
		//Inits sortedHighScores
		for(int i = 0; i < sizeof(&sortedHighScores); i++)
		{
			sortedHighScores[i] = Score("", 0);
		}
		//Read file
		while(!oFileIn.eof())
		{
			if(!oFileIn.good())
				throw IOException("IOException", "Error while reading: " + fileName);
			nextLine = "";
			getline(oFileIn, nextLine);
			string name = nextLine;
			getline(oFileIn, nextLine); 
			Score score;
			score.setName(name);
			score.setScore(atoi(nextLine.c_str()));
		
			highScores.push_back(score);
			if(highScores.size() == 5)
				break;
		}
		oFileIn.close();
	
		bool highScoreAdded = false;
		//Iterates highscores read
		for(int i = 0, j = 0; j < highScores.size(); i++, j++)
		{
			//Transfer highscore to sortedlist
			sortedHighScores[i] = highScores.at(j);
			/*
			* Checks if current score should be added to sortedList
			*/
			if(((score > highScores.at(j).getScore()) || (i == highScores.size()-1 && highScores.size() < 5)) && !highScoreAdded)
			{
				string ranking = "Uknown";
				switch(i+1)
				{
				case 1:
					ranking = "1st";
					break;
				case 2:
					ranking = "2nd";
					break;
				case 3:
					ranking = "3rd";
					break;
				case 4:
					ranking = "4th";
					break;
				case 5:
					ranking = "5th";
					break;
				}
				//Shows input box, in order to get name
				string leadText = "You have been ranked as " + ranking  + " best!\nPlease enter your name:";
			
				CWin32InputBox::InputBox("Congratulations", leadText.c_str(), buff, 100, false); 
				string name = buff;
				cout << "NAME: " << name << " paa plass: " << i << endl;
			
				if(name.size() == 0)
					name = "Uknown";
			
				sortedHighScores[i].setName(name);
				sortedHighScores[i].setScore(score);
				//Make a step back for y, in order to get all scores after the gap
				if(score > highScores.at(j).getScore())
					j--;
				highScoreAdded = true;
			
				}
			}
		//Writes to file
		ofstream streamOut;
		if(!streamOut.is_open())
		streamOut.open(fileName.c_str());
		for(int i = 0; i < 5; i++)
		{
			Score &currScore = sortedHighScores[i];
			if(currScore.getScore() > 0 && currScore.getName().length() > 0)
			{
				if(!streamOut.good())
					throw IOException("IOException", "Error while writing to: " + fileName);
				streamOut << currScore.getName();
				streamOut << '\n';
				streamOut << currScore.getScore();
				streamOut << '\n';
			}
		}
		streamOut.close();
	}
	catch(IOException ioex)
	{
		string errorMessageString = ioex.what();
		
		size_t length = errorMessageString.length() + 1;
		char* buffer = new char[length];
		strcpy_s(buffer, length, errorMessageString.c_str());
	
		MessageBox(NULL, buffer, "IOException", MB_OK);
		delete [] buffer;
	}
}
/*
* Method sets score
*/
void PlayArea::setScore(unsigned long score)const
{
	outputManager->DrawScore(g_usSCORE_POS_X, g_usSCORE_POS_Y, score);
}
/*
* Method updates the preview
*/
void PlayArea::updatePreview(Piece currPiecePreview)const
{
	Cell* tiles = currPiecePreview.getTiles();
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			Cell* currCell = tiles + (i*4+j);
			outputManager->DrawTile(g_usPREVIEWPIECE_POS_X+i, g_usPREVIEWPIECE_POS_Y+j, currCell->getColor());
		}
	}
}
/*
* Method draws gameobjects
*/
void PlayArea::drawGameObjects(const vector<Piece> pieceList, const Piece currPiecePreview, const unsigned long score)const
{
	//First clear gameArea
	clear();
	outputManager->DrawBackground();
	for(int i = 0; i < pieceList.size(); i++)
	{
		Piece currPiece = pieceList.at(i);
		Cell *tiles = currPiece.getTiles();
		for(int j = 0; j < NUMBER_OF_TILES; j++)
		{
			Cell* tile = (tiles+j);
			if(tile->getColor() != TC_BACKGROUND && tile->isActive())
			{
				outputManager->DrawTile(currPiece.getX() + tile->getX(), currPiece.getY() + tile->getY(), tile->getColor());
			}
		}	
	}
	outputManager->DrawBackground();
	updatePreview(currPiecePreview);
	setScore(score);
	outputManager->OutputGraphics();
}
/*
* Method clears gameArea by drawing background
*/
void PlayArea::clear()const
{
	for(int i = 0; i < g_usPLAYAREA_TILEWIDTH; i++)
	{
		for(int j = 0; j < g_usGAME_TILEHEIGHT; j++)
		{
			outputManager->DrawTile(i, j, TC_BACKGROUND);
		}
	}
}
PlayArea::~PlayArea(void)
{
}
