#ifndef _MAZE_NODE_TWO_H
#define _MAZE_NODE_TWO_H

/*
This is the class that implements the nodes in Level 2 of the maze.

It is derived from MazeNode having a template parameter of MazeNodeTwo* to avoid overwriting
the corresponding base class functions.
*/

#include "MazeNode.h"

class MazeNodeTwo : public MazeNode<MazeNodeTwo*>
{
public:
	MazeNodeTwo(int prizeCode = 0, int pathNum = 5, bool isFinish = 0);
	~MazeNodeTwo();

	/*
	Resets the "finish" variable of the node.
	@post If finish == 1 before function call, finish = 0. Otherwise,
	finish = 1
	*/
	void resetFinish();
};


#endif
