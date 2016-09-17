#ifndef _MAZE_INTERFACE_H
#define _MAZE_INTERFACE_H

/*
This is the interface for any maze object, Level 1 or Level 2. It is an ADT containing all of the
necessary functions required. It also houses the multipliers for each power-up available in the maze, used in
calculating the score
*/


#include "Player.h"

//Used to calculate the score. Note that a life is worth 500 points, a coin is 100,
//and a power is 1000.
const int MULTIPLIERS[NUM_COLLECTIBLES] = {500, 100, 1000};

class MazeInterface
{
public:
	virtual ~MazeInterface() {}; //Virtual destructor in order to deallocate the dynamic memory
					//for Levels 1 and 2 of the maze.

	/*
	Sets a new player into the maze, and resets the nodes and the player collectibles
	to accommodate the change. Also removes the existing player.
	@param newPlayer: pointer to the player object
	@post: The player of the maze is set to newPlayer, all of the nodes of the maze are reset,
	and the number of collectibles of the player is set to the default values.
	*/
	virtual void setPlayer(Player* newPlayer) = 0;


	/*
	Moves the player to the next direction in the maze, if nextPathIndex is within the array bounds
	of pathPtr in the corresponding maze.
	If the player's new location is a dead end, the player is moved back to their previous intersection
	and a life is lost, with the player being notified of their status. NOTE: A NULL pointer is a dead-end.
	Otherwise, checks if the node contains a prize. If it does, the player is notified of the prize,
	and the corresponding collectible is incremented.
	Then it checks if the node is the maze exit. If it is, the player is sent a congratulatory
	message upon completing the maze. Otherwise, the player is notified that they've successfully
	moved to the direction corresponding to pathIndex.

	@param: nextPathIndex : index of pathPtr for the intersection node.
	@post: If nextPathIndex is a dead end, the player's lives are decremented.
	Otherwise, the player is moved to the next intersection.
	If the node contains a prize, the corresponding collectible is incremented.

	@return: Returns 0 if the player hit a dead end and lost a life, or if nextPathIndex
	is outside of the array bounds, or if the player's move was successful and their new
	location is not the maze exit.
	Returns -1 if the player reached the maze exit.
	Returns -2 if the player, after the move, is out of lives to indicate game over.
	*/
	virtual int movePlayer(int nextPathIndex) = 0;

	/*
	Returns the possible paths the player can take in the maze for the client.
	This is level dependent, so see the corresponding derived classes for more information.
	@reurn: Pointer to the const char strings containing the possible directions the player
	can take.
	*/
	virtual const char** getPossiblePaths() const = 0;

	/*
	Returns the number of possible paths the player can take in the maze.
	These are 3 for Level 1, and 5 for Level 2.
	*/
	virtual int getNumPaths() const = 0;

	/*
	Outputs the next direction the player must take to reach the finish, if they have a power.
	Decrements their power afterwards.
	*/
	virtual bool reveal() const = 0;

	/*
	Calculates the player's total score for the maze. Note that a life is worth 500 points,
	a coin 100, and a power 1000.
	*/
	virtual int calcScore() const = 0;

	/*
	If a player exists in the maze, then the player is moved back to the start, and their
	collectibles are reset to their default values.
	Finally, the nodes of the maze are also reset.

	@post: The player collectibles are at their default values, and the player is moved back
	to the start IF a player exists. Otherwise, nothing happens. Finally, all nodes of the maze
	are reset to their default values.
	*/
	virtual void startOver() = 0;

	/*
	Displays the entire maze, including the location of the player, the finish and all dead ends.
	This is for the grader only, so that they can verify the random distribution of the maze
	and that the player moves as desired. See the implementations for Level 1 and 2 of the maze
	for details on how this is done.
	*/
	virtual void displayMaze() const = 0;
};

#endif
