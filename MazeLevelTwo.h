#ifndef _MAZE_LEVEL_TWO_H
#define _MAZE_LEVEL_TWO_H

/*
This is the class that implements Level 2 of the maze. It is derived from MazeInterface. A specialized
adjacency list is used to implement the nodes in the maze.

For Level 2, there are five possible directions the user can take:
B = Backwards, S = Straight, L = Left, R = Right, and D = Diagonal.
*/


#include "MazeInterface.h"
#include "MazeNodeTwo.h"
#include "Queue.h"

#include <cstdlib>

const int DEFAULT_LEVEL_TWO_NODE_NUM = 10; //Default number of nodes in level 1 of the maze
const int LEVEL_TWO_NUM_DIRECTIONS = 5; //Number of possible paths that the player can take,
//for the present project, index 0 = backwards, 1 = straight, 2 = left, 3 = right, and 4 = diagonal

/*
These variables store how the prizes in level 2 will be distributd.
Coins occur with a 1/3 change for all levels, but the encounter of a power
is dependent on LEVEL_TWO_PRIZE_DIST_SIZE, in that larger values will make a power rarer
to encounter.
*/
const int LEVEL_TWO_PRIZE_DIST_SIZE = 6; //Make sure this is a multiple of 3 if it is being changed.
const int LEVEL_TWO_COIN_MAX = (LEVEL_TWO_PRIZE_DIST_SIZE - 1)/3;

class MazeLevelTwo : public MazeInterface
{
public:
	//Constructor, takes the number of nodes in the maze (i.e. valid intersections) as input
	MazeLevelTwo(int numNodes = DEFAULT_LEVEL_TWO_NODE_NUM);
	MazeLevelTwo(const MazeLevelTwo&); //Copy constructor
	~MazeLevelTwo(); //Destructor

	void setPlayer(Player* newPlayer); //Described in maze interface
	int movePlayer(int nextPathIndex); //Described in maze interface

	/*
	Described in maze interface. Here, a pointer, call it ptr, is returned having the following strings
	at its indices:
	ptr[0] = "Go backwards."
	ptr[1] = "Go straight."
	ptr[2] = "Go left."
	ptr[3] = "Go right."
	ptr[4] = "Go diagonal."

	These are the directions the player can take
	*/
	const char** getPossiblePaths() const;
	int getNumPaths() const; //Described in maze interface
	bool reveal() const; //Described in maze interface
	int calcScore() const; //Described in maze interface
	void startOver(); //Described in maze interface

	/*
	Outputs the tree in an adjacency list style with a breadth first traversal.
	Shows all of the intersections, the player location, the finish, and all the dead ends.
	Example output is shown below:

		S L R D
	     P  O X X F
	     O  X X X X
	     F  - - - -

	The above is representative of a typical output, where P = player, O = intersection, and X =
	dead end. Unlike MazeLevelOne, this style of formatting does not require as much console space,
	so feel free to use as many nodes as desired when displaying the maze.
	*/
	void displayMaze() const;

private:
	Player* playerPtr; //Pointer to store the player
	MazeNodeTwo* playerLoc; //Pointer to store the player's location in the maze
	MazeNodeTwo** listArray; //Array to store the nodes in the maze
	int arraySize; //Variable to store the size of the list array

	/*
	Builds a maze having a maximum number of nodes specified by numNodes, excluding the starting
	location. The last node of the maze is the finish. Uses a "breadth-first" style build-up,
	where the number of directions each intersection will have is randomly assigned and randomly
	allocated and that the first intersection created is the next intersection that will be considered
	during the construction process, similar to how the first node visited from the starting node
	is the next node that will have a breadth-first traversal in a breadth-first style traversal
	of a graph.

	@para numNodes: The number of nodes the maze will have
	@post: A maze having a maximum of numNodes + 1 nodes (+1 is for the starting location) is created,
	but the final node count could be less due to the random allocation of directions.
	*/
	void buildMaze(int numNodes); //Build the maze using a queue.

	/*
	Returns either 0, 1 or 2 to designate the prize number. Used when creatig a new node.
	Recall that 0 = the node has no prize, 1 = it has a coin, and 2 = it has a power.
	*/
	int getPrizeNum() const;

	/*
	Function returns the number of intersections (i.e. non-dead end paths) that the current intersection
	will be connected to/have. A maximum of 4 intersections is possible, as the fifth direction is
	"backwards" which is already accounted for.
	@param: numNodes - The remaining number of nodes that can be created.
	@return: A value between 0 and 4, or 0 and numNodes.
	*/
	int pathsFromNode(int numNodes) const;

	/*
	Returns an array of pointers that contain the next set of intersections that will be connected
	to the maze, and also connects these new pointers to the current intersection (to establish their
	"backwards" direction).

	@param: numPaths - The number of paths from the intersection
	numNodes - The number of nodes remaining that can be made. If numNodes drops to 1 in createNodeArray,
	then the corresponding new node is set to be the finish.
	parentNode - A pointer to the current intersection in order to connect the new, created nodes
	to their "parent" [to establish the backwards direction]

	@post: numNodes will be decremented by an amount equivalent to numPaths
	and an array of node pointers will be returned containing the next intersections
	to be connected. Also, these new pointers will be connected to their parent, parentNode.
	*/
	MazeNodeTwo** createNodeArray(int numPaths, int& numNodes,
							MazeNodeTwo* parentNode) const;

	/*
	Function randomly connects the new nodes contained in nodeArray to the parent, where these nodes
	will serve as either the straight, left, right, or diagonal directions of the parent with the
	corresponding direction chosen randomly. After being connected, these nodes will then be placed into
	the nodeQueue.

	@param: nodeQueue - The queue containing the next set of intersections/nodes to build more
	intersections/nodes off of.
	parentNode - Pointer to the current intersection, used to connect its children to itself.
	nodeArray - Array of pointeres storing the next set of intersections to be created
	numPaths - The number of new nodes/intersections to be connected

	@post: parentNode has numPaths children connected to it, where the corresponding pointer
	is chosen randomly for each pointer in nodeArray. Finally, these same numPaths pointers are placed
	in nodeQueue to further build the maze, if necessary.
	*/
	void connectChildren(MazeNodeTwo* parentNode, MazeNodeTwo** nodeArray, int numPaths) const;

	/*
	Function checks if the corresponding "pathIndex" child pointer of parentNode from above
	has already been connected.

	@param: pathIndex - The index of pathPtr to be checked
	pathArray[] - An array storing the indices of pathPtr that have been connected
	numElements - Number of elements to examine in pathArray

	@return: True if pathIndex is in pathArray, false otherwise.
	*/
	bool contains(int pathIndex, const int pathArray[], int numElements) const;


	/*
	Recursive copy function, used with the copy constructor.
	*/
	MazeNodeTwo* copyMaze(MazeNodeTwo* otherNode);

	/*
	Function that resets the prizeNum of each node in the maze. Used in conjunction with startOver.
	@param: nodePtr - Pointer to the current intersection to be resetted
	@post: All the prizes in the maze are resetted.
	*/
	void resetNodes(MazeNodeTwo* nodePtr);

	/*
	Checks if a path to the maze exit exists starting at nodePtr
	@return: True if a path does exist, false otherwise.
	*/
	bool findPathToFinish(MazeNodeTwo* nodePtr) const;

	/*
	Deletes the entire maze
	*/
	void removeAll();

};

#endif
