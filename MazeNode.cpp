#ifndef _MAZE_NODE_CPP //This avoids compilation issues with template classes
#define _MAZE_NODE_CPP

/*
This is the implementation file corresponding to MazeNode.h containing the definitions of the member
functions.
*/

#include "MazeNode.h"

#include <cstdlib>
#include <ctime>
#include <iostream>


template <class ItemType>
MazeNode<ItemType>::MazeNode(int prizeCode, int pathNum, bool isFinish)
	: numPaths(pathNum), prizeNum(prizeCode), finish(isFinish), pathPtr(new ItemType[pathNum])
{ //Note that MazeNode is encapsulated inside the maze object, so negative values of pathNum
  //or prizeCode will not occur.

	for (int i = 0; i < numPaths; ++i) //Set the default value of pathPtr[i] to NULL.
		pathPtr[i] = NULL;
}

template <class ItemType>
MazeNode<ItemType>::MazeNode(const MazeNode& otherNode)
	: prizeNum(otherNode.prizeNum), finish(otherNode.finish), numPaths(otherNode.numPaths),
		pathPtr(new ItemType[otherNode.numPaths])
{
	for (int i = 0; i < numPaths; ++i) //We only want the node's contents, not its children.
		pathPtr[i] = NULL;
}

template <class ItemType>
MazeNode<ItemType>::~MazeNode()
{
	delete [] pathPtr;
}

template <class ItemType>
int MazeNode<ItemType>::getPrizeNum() const
{
	return prizeNum;
}

template <class ItemType>
bool MazeNode<ItemType>::isFinish() const
{
	return finish;
}

template <class ItemType>
ItemType MazeNode<ItemType>::getNextNodePtr(int pathIndex) const
{
	if (0 <= pathIndex && pathIndex < numPaths) //Check if pathIndex is within the bounds
		return pathPtr[pathIndex];
	else //Return NULL if it isn't.
		return NULL;
}

template <class ItemType>
void MazeNode<ItemType>::resetPrize()
{
	prizeNum = -prizeNum;
}

template <class ItemType>
bool MazeNode<ItemType>::setNextNodePtr(int pathIndex, ItemType nextNode)
{
	bool canSet = (0 <= pathIndex && pathIndex < numPaths); //Checking array bounds

	if (canSet) //Setting the value of pathPtr[pathIndex] to nextNode if canSet is true
		pathPtr[pathIndex] = nextNode;

	return canSet;
}

template <class ItemType>
const MazeNode<ItemType>& MazeNode<ItemType>::operator=(const MazeNode<ItemType>& other)
{
	if (this != &other) //We only want the values of finish and prizeNum.
	{			//Note that this was only used in implementing the remaining
		finish = other.finish; //functions of the data structure itself,
		prizeNum = other.prizeNum; //not to implement the project
	}

	return *this;
}

#endif
