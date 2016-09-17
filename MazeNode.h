#ifndef _MAZE_NODE_H
#define _MAZE_NODE_H

/*
This is the generic node class for the maze containing the essential functions for the node.
Nodes for Level 1 and Level 2 are derived from this. Note that the template parameter, ItemType,
is intended to be a pointer data type so as to avoid overwriting the functions that return
the corresponding path pointers for the node for any derived classes.
*/

template <class ItemType> //Template parameter is intended to be a pointer (e.g. int* instead of int)
class MazeNode
{
public:
	MazeNode(int prizeCode = 0, int pathNum = 3,  bool isFinish = 0); //Default constructor
	MazeNode(const MazeNode&); //Copy constructor
	~MazeNode();


	/*
	Returns the prize number of the node. Note that 0 means no prize, 1 means that the node
	contains a coin, while 2 means that the node has a power.
	*/
	int getPrizeNum() const; //0 = no prize, 1 = coin, 2 = power

	/*
	Returns the value of finish, used to check if the node is the finish node or maze exit.
	*/
	bool isFinish() const;


	/*
	Returns a pointer to the next node (note that ItemType is supposed to be a pointer)
	at pathIndex if it exists, NULL otherwise.
	@param: pathIndex: The index of pathPtr to be accessed
	@return: Returns the pointer contained at pathPtr[pathIndex] if pathIndex is within
	the array bounds, NULL otherwise.
	*/
	ItemType getNextNodePtr(int pathIndex) const;

	/*
	Resets the value of prizeNum to -prizeNum in order to avoid repeat triggering of the node's
	prize anytime the player moves backwards in the maze.
	@post: The value of prizeNum is set to -prizeNum.
	*/
	void resetPrize();

	/*
	Sets the value of pathPtr[pathIndex] to nextNode if pathIndex is within the array bounds.
	Returns a value of true if successful, false otherwise.
	@param pathIndex: The index of pathPtr to be accessed.
	nextNode: A pointer to the next node to be connected
	@post: pathPtr[pathIndex] is set to nextNode if pathIndex is within array bounds, otherwise
	nothing happens.
	@return: True if pathIndex is within the array bounds, false otherwise.
	*/
	bool setNextNodePtr(int pathIndex, ItemType nextNode);


	/*
	Overriding the assignment operator. For the present project, ONLY
	the values of the variables finish, prizeNum, and numPaths are copied.
	Only used to implement the removeEntry function for Level 1 of the maze, the binary
	search tree when swapping with the inorder successor.
	*/
	const MazeNode<ItemType>& operator=(const MazeNode<ItemType>&);

protected:
	bool finish; //Variable to indicate if the node is the maze exit
private:
	int prizeNum; //Prize number of the node. 0 = no prize, 1 = coin, 2 = power
	ItemType* pathPtr; //Pointer array to the node's children/adjacent nodes
	int numPaths; //Size of pathPtr
};

#include "MazeNode.cpp"

#endif
