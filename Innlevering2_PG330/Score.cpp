#include "Score.h"

#include <iostream>


using std::string;
using std::cout;
using std::endl;

/*
* Score.cpp
* Magnus&Gard ris v1.0
*
* Created by: Gard Sandholt and Magnus Østeng
* Date: 20.11.09
* Description:
* Simple Value Object class for reading/writing scores
*/
Score::Score()
{
}
Score::Score(std::string newName, int newScore)
{
	setName(newName);
	setScore(newScore);
}

//GETTERS AND SETTERS
void Score::setName(string newName)
{
	name = newName;
}
void Score::setScore(int newScore)
{
	score = newScore;
}
string Score::getName()const
{
	return name;
}
int Score::getScore()const
{
	return score;
}
Score::~Score(void)
{
}
