/*
* main.cpp
* Magnus&Gard ris v1.0
*
* Created by: Gard Sandholt and Magnus Østeng
* Date: 20.11.09
* Description:
* Main class
*/


#include <iostream>
#include <SDL.h>
#include "GameManager.h"


int main( int argc,		  
		  char* argv[] ) 
{
	GameManager manager;
	manager.initGame(argc, argv);
	return 0;

}  // END main



