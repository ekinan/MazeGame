#ifndef _MAZE_NODE_ONE_H
#define _MAZE_NODE_ONE_H

/*
This is the class that implements the nodes in Level 1 of the maze.

Note that MazeNodeOne is derived from MazeNode since it is going to be used in Level 1 of the maze,
which is a binary search tree. To randomize the maze, each node will have a sort key that's to be
randomly generated - but ONLY maze level one requires that sort key.
*/

#include "MazeNode.h"

class MazeNodeOne : public MazeNode<MazeNodeOne*>
{
public:
	MazeNodeOne(int prizeCode = 0, int pathNum = 3, bool isFinish = 0, int key = 0);
	~MazeNodeOne();

	/*
	Returns the value of sortKey
	*/
	int getSortKey() const;

	/*
	Override the assignment operator to copy the value of sortKey.
	Note this was only used to implement the remaining data structure functions,
	and was not necessary to do the project itself.
	*/
	const MazeNodeOne& operator=(const MazeNodeOne&);
private:
	int sortKey; //Stores the sortKey of the node to insert into the BST.
};

#endif
