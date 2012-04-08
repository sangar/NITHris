#include "Cell.h"
#include <iostream>
using std::cout;
using std::endl;
/*
Cell.cpp
Magnus&Gard ris v1.0

Created by: Gard Sandholt and Magnus Østeng
Date: 20.11.09
Description:
This is a value object class.
*/
Cell::Cell(void)
{
	active = false;
}
Cell::Cell(int x, int y, ETileColor color)
{
	setCoords(x, y);
	setColor(color);
	active = true;
}

Cell::~Cell(void)
{
}
/*
	GETTERS AND SETTERS
*/
void Cell::setCoords(int x, int y)
{
	xD = x;
	yD = y;
}
int Cell::getX()const
{
	return xD;
}
int Cell::getY()const
{
	return yD;
}
void Cell::setColor(ETileColor colorLocal)
{
	color = colorLocal;
}
ETileColor Cell::getColor()const
{
	return color;
}
bool Cell::isActive()const
{
	return active;
}
void Cell::disable()
{
	active = false;
}
