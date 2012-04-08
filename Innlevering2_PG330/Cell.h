#ifndef CELL_H
#define CELL_H

#include "nithris.h"
using namespace PG330Helpers;

class Cell
{
private:
	int xD, yD;
	bool active;
	ETileColor color;
public:
	Cell::Cell(void);
	Cell(int x, int y, ETileColor color);
	~Cell(void);
	void setCoords(int x, int y);
	void setColor(ETileColor c);
	ETileColor getColor()const;
	int getX()const;
	int getY()const;
	bool isActive()const;
	void disable();
};
#endif