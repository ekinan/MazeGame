#include "MazeNodeTwo.h"
#include <iostream>

/*
This is the implementation file for MazeNodeTwo.h containing the definitions of the member functions
*/

MazeNodeTwo::MazeNodeTwo(int prizeCode, int pathNum, bool isFinish)
	: MazeNode<MazeNodeTwo*>(prizeCode, pathNum, isFinish)
{
}

MazeNodeTwo::~MazeNodeTwo()
{
}

void MazeNodeTwo::resetFinish()
{
	finish = !finish;
}
