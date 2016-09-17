#ifndef _MAZE_LEVEL_ONE_H
#define _MAZE_LEVEL_ONE_H

/*
This is the class implementing Level 1 of the maze. It is derived from MazeInterface.
A binary search tree (BST) is used as the corresponding data structure.
Here, the user has up to three directions they can take:

B = Backward, L = Left, and R = Right.
*/

#include "MazeInterface.h"
#include "MazeNodeOne.h"
#include "Player.h"

#include <cstdlib>

const int DEFAULT_LEVEL_ONE_NODE_NUM = 4; //Default number of nodes in level 1 of the maze
const int LEVEL_ONE_NUM_DIRECTIONS = 3; //Number of possible paths that the player can take,
//for the present project, index 0 = backwards, index 1 = left, index 2 = right
const int KEY_DIST_SIZE = 100; //Size of the distribution of the keys for the level.

/*
These variables store how the prizes in level 1 will be distributed.
Coins occur with a 1/3 chance for all levels, but the encounter of a power
is dependent on LEVEL_ONE_PRIZE_DIST_SIZE, in that larger values will make a power
rarer to encounter.
*/
const int LEVEL_ONE_PRIZE_DIST_SIZE = 6; //Make sure PRIZE_DIST_SIZE is a multiple of 3
const int LEVEL_ONE_COIN_MAX = (LEVEL_ONE_PRIZE_DIST_SIZE - 1)/3;

class MazeLevelOne : public MazeInterface
{
public:
	//Constructor, takes the number of nodes in the maze (i.e. valid intersections) as input.
	MazeLevelOne(int numNodes = DEFAULT_LEVEL_ONE_NODE_NUM);
	MazeLevelOne(const MazeLevelOne&); //Copy constructor
	~MazeLevelOne(); //Destructor

	void setPlayer(Player* newPlayer); //Described in maze interface
	int movePlayer(int nextPathIndex); //Described in maze interface

	/*
	Described in maze interface. Here, a pointer, call it ptr, is returned having the following strings
	at its indices:
	ptr[0] = "Go backwards."
	ptr[1] = "Go left."
	ptr[2] = "Go right."

	These are the directions the player can take.
	*/
	const char** getPossiblePaths() const;
	int getNumPaths() const; //Described in maze interface
	bool reveal() const; //Described in maze interface
	int calcScore() const; //Described in maze interface
	void startOver(); //Described in maze interface

	/*
	Outputs the tree in level order fashion, showing all of the intersections, the player,
	the finish, and all dead ends. Output is formatted so as to look like a tree. Example:
				P
			O		O
		      X   X           X   F

	The above is representative of a typical output, where P = player, O = intersection,
	and X = dead end. Because it is formatted as such, use this function for a maze having
	at most 8 nodes to avoid filling up the entire space of the command prompt.
	The number of nodes can be configured in both the present class, via
	DEFAULT_LEVEL_ONE_NODE_NUM, or in the client program itself.
	*/
	void displayMaze() const;

private:
	Player* playerPtr; //Pointer to store the player
	MazeNodeOne* playerLoc; //Pointer to store the player's location in the maze
	MazeNodeOne* startPtr; //Pointer to store the starting location of the maze

	/*
	Builds a maze having the number of nodes specified by numNodes, excluding the starting
	location. The last node of the maze is the finish
	@para numNodes: The number of nodes the maze will have.
	@post: A maze having numNodes + 1 nodes, (+1 is for the starting location) is created,
	with a random distribution of prizes and nodes. The last node created is the maze exit.
	*/
	void buildMaze(int numNodes);


	/*
	Inserts newNode into the maze using the generic BST insertion algorithm.
	@param: subTreePtr - Pointer to the root of the subtree
	newNode - Pointer to the new node to be inserted.
	@post: newNode is inserted
	@return: Returns a pointer to reconnect the tree after insertion.

	NOTE: This is the INSERT function for the data structure
	*/
	MazeNodeOne* insertInLoc(MazeNodeOne* subTreePtr, MazeNodeOne* newNode);

	/*
	Deletes all of the nodes in the entire maze in a postOrder fashion.
	@post: The maze is empty.

	NOTE: This is the REMOVE-ALL function for the data structure.
	*/
	void postOrderDelete(MazeNodeOne* subTreePtr);


	/*
	Copies the contents of the maze pointed to by otherTreePtr to the present maze.
	@param subTreePtr - Pointer to the root of the new subtree to be created
	otherTreePtr - Pointer to the root of the other tree's subtree
	@return: A pointer to reconnect the tree.
	*/
	MazeNodeOne* copyTree(MazeNodeOne* subTreePtr, MazeNodeOne* otherTreePtr);

	/*
	Checks if a path to the maze exit exists starting at subTreePtr.
	@return: True if a path does exist, false otherwise.
	*/
	bool findPathToFinish(MazeNodeOne* subTreePtr) const;

	/*
	Recursively resets the prizes in each node via a pre-order fashion.
	@post: All prizes in the maze are resetted
	*/
	void resetNodes(MazeNodeOne* subTreePtr);

	/*
	Returns the height of the tree
	*/
	int getHeight(MazeNodeOne* subTreePtr) const;

	/*
	Computes 2^(exponent)
	*/
	int powTwo(int exponent) const;



	 /*The functions below were not used for the present project, but were only implemented
	in order to satisfy the assignment's requirement that the data structure
	implement the insertion, removal, remove-all, and get entry algorithms.
	They should NOT be considered an essential part of the project, or a part of the
	overall assignment grade, hence why they do not follow the style guidelines.

	Note that insert and remove-all were used in the present project, and hence are a part
	of the core functions above (see insertInLoc and postOrderDelete).
	*/

	/*
	NOTE: This is the displayAll function, displays the sort keys of the nodes in the BST
	in an inorder fashion. Used in conjunction with traverse
	*/
	void displayAll() const;
	void traverse(MazeNodeOne* subTreePtr) const;

	/*
	NOTE: These are the getEntry functions.
	*/
	MazeNodeOne* findNode(MazeNodeOne* treePtr, int sortKey) const;
	int getEntry(int sortKey) const;


	/*
	NOTE: These functions remove the node having the sort key having a value of sortKey
	using the BST removal algorithms.
	*/
	MazeNodeOne* removeValue(MazeNodeOne* treePtr, int sortKey, bool& successful);
	MazeNodeOne* swapSuccessor(MazeNodeOne* treePtr, MazeNodeOne* nodeToSwap);
	MazeNodeOne* deleteNode(MazeNodeOne* treePtr);
	bool removeEntry(int sortKey);

};

#endif
