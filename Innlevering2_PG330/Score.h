#ifndef Score_H
#define Score_H

#include <string>
class Score
{
private:
	std::string name;
	int score;
public:
	void setName(std::string newName);
	void setScore(int newScore);
	std::string getName()const;
	int getScore()const;
	Score(std::string newName, int newScore);
	~Score(void);
	Score();
};

#endif