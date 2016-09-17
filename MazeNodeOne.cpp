#include "MazeNodeOne.h"

/*
This is the implementation file for MazeNodeOne.h containing the definitions of the member functions.
*/

MazeNodeOne::MazeNodeOne(int prizeCode, int pathNum, bool isFinish, int key)
	: MazeNode<MazeNodeOne*>(prizeCode, pathNum, isFinish), sortKey(key)
{
}

MazeNodeOne::~MazeNodeOne()
{
}

int MazeNodeOne::getSortKey() const
{
	return sortKey;
}

const MazeNodeOne& MazeNodeOne::operator=(const MazeNodeOne& other)
{
	if (this != &other)
	{
		MazeNode<MazeNodeOne*>* ptr1 = this;
		const MazeNode<MazeNodeOne*>* ptr2 = &other;

		*ptr1 = *ptr2; //Use polymorphism to copy the base class values.
		sortKey = other.sortKey; //Copy the sortKey value.
	}

	return *this;
}
