/*
This is the implementation file corresponding to MazeLevelTwo.h containing the definitions of the member functions
in the header file.
*/

#include "MazeLevelTwo.h"
#include "Stack.h"

#include <iostream>

MazeLevelTwo::MazeLevelTwo(int numNodes)
	: playerPtr(NULL), playerLoc(NULL)
{
	if (numNodes <= 2) //There has to be at least two nodes
		numNodes = DEFAULT_LEVEL_TWO_NODE_NUM;

	arraySize = numNodes;

	listArray = new MazeNodeTwo*[arraySize];
	for (int i = 0; i < numNodes; listArray[i++] = NULL);

	buildMaze(numNodes);
}

MazeLevelTwo::MazeLevelTwo(const MazeLevelTwo& otherMaze)
{
	playerPtr = NULL;
	playerLoc = NULL;
	arraySize = otherMaze.arraySize;

	int arrayIndex = 0;

	listArray = new MazeNodeTwo*[arraySize];
	listArray[arrayIndex++] = copyMaze(otherMaze.listArray[0]);
	Queue<MazeNodeTwo*> nodeQueue; //Copied the shape of the graph
	nodeQueue.push(listArray[0]);
	while (!nodeQueue.empty()) //Now we allocate the nodes to listArray using breadth order traversal
	{
		MazeNodeTwo* nodePtr = nodeQueue.front();
		nodeQueue.pop();

		for (int i = 1; i < LEVEL_TWO_NUM_DIRECTIONS; ++i)
		{
			MazeNodeTwo* childPtr = nodePtr->getNextNodePtr(i);
			if (childPtr)
			{
				listArray[arrayIndex++] = childPtr;
				nodeQueue.push(childPtr);
			}
		}
	}
}

MazeLevelTwo::~MazeLevelTwo()
{
	removeAll();
}

void MazeLevelTwo::removeAll()
{
	Stack<MazeNodeTwo*> nodeStack; //Use a stack to carry out the deletion
	nodeStack.push(listArray[0]); //Stack will store the next intersection/"Node" that will be deleted
	while (!nodeStack.empty())
	{
		MazeNodeTwo* currentNodePtr = nodeStack.top();
		nodeStack.pop();
		for (int i = 1; i < LEVEL_TWO_NUM_DIRECTIONS; ++i)
		{
			MazeNodeTwo* nextNode = currentNodePtr->getNextNodePtr(i);
			if (nextNode) //Get the non-NULL intersections of the node,
				nodeStack.push(nextNode); //and store them onto the stack
		}
		delete currentNodePtr; //Delete the current node after, and move on to the next intersection
	}

	delete [] listArray; //No need to delete the individual members of listArray, as the nodes they point to have
				//already been deallocated from above
}

MazeNodeTwo* MazeLevelTwo::copyMaze(MazeNodeTwo* otherNode)
{
	if (otherNode)
	{
		MazeNodeTwo* newNode = new MazeNodeTwo(*otherNode);
		for (int i = 1; i < LEVEL_TWO_NUM_DIRECTIONS; ++i)
		{
			MazeNodeTwo* childPtr = copyMaze(otherNode->getNextNodePtr(i));
			newNode->setNextNodePtr(i, childPtr);
			if (childPtr) //Connect backwards
				childPtr->setNextNodePtr(0, newNode);
		}

		return newNode;
	}

	return NULL;
}

void MazeLevelTwo::buildMaze(int numNodes)
{
	Queue<MazeNodeTwo*> nodeQueue; //Use a breadth-first style build for the maze
	int numPaths = 0;
	int arrayIndex = 0;

	listArray[arrayIndex++] = new MazeNodeTwo(0, LEVEL_TWO_NUM_DIRECTIONS, 0);
	numNodes--;

	nodeQueue.push(listArray[0]); //ADD LABEL
	MazeNodeTwo* nodePtr = NULL;

	while ((numPaths = pathsFromNode(numNodes)) == 0); //Cannot have the start be the finish, so we want
	while (!nodeQueue.empty() && numNodes > 0) //at least one path from the start.
	{ //Keep building the maze until either the Queue is empty, or we've reached the node limit.
		nodePtr = nodeQueue.front(); //Get the next intersection
		nodeQueue.pop();

		MazeNodeTwo** newNodes = createNodeArray(numPaths, numNodes, nodePtr); //Generate the children

		if (newNodes) //If any new nodes were created
		{
			connectChildren(nodePtr, newNodes, numPaths); //We connect them to the current node
			delete [] newNodes; //Deallocate the array of pointers, but NOT the nodes they point to

			for (int i = 1; i < LEVEL_TWO_NUM_DIRECTIONS; ++i) //Then we put them into queue
			{						//and store them in listArray
				MazeNodeTwo* childNode = nodePtr->getNextNodePtr(i);
				if (childNode)
				{
					listArray[arrayIndex++] = childNode;
					nodeQueue.push(childNode);
				}
			}
		}
		numPaths = pathsFromNode(numNodes); //Generate the number of paths for the next node
	}

	if (numNodes > 0) //Premature end, so we set the last new node constructed to be the finish.
		nodePtr->resetFinish();
}

int MazeLevelTwo::getPrizeNum() const
{
	int prizeCode = rand() % LEVEL_TWO_PRIZE_DIST_SIZE; //Same syntax as MazeNodeLevelOne for generating the prize numbers for each node

	if (prizeCode <= LEVEL_TWO_COIN_MAX)
		prizeCode = 1;
	else if (prizeCode == LEVEL_TWO_COIN_MAX + 1)
		prizeCode = 2;
	else //No prize
		prizeCode = 0;

	return prizeCode;
}

int MazeLevelTwo::pathsFromNode(int numNodes) const
{
	int num;
	while ((num = (rand() % LEVEL_TWO_NUM_DIRECTIONS)) > numNodes);
	return num; //Generate # between 0 and LEVEL_TWO_NUM_DIRECTIONS - 1 (so a # between 1 and 4 for this case)
}

MazeNodeTwo** MazeLevelTwo::createNodeArray(int numPaths, int& numNodes,
						MazeNodeTwo* parentNode) const
{
	if (numPaths > 0)
	{
		MazeNodeTwo** nodeArray = new MazeNodeTwo*[numPaths]; //Create the new array of pointers

		for (int i = 0; i < numPaths; nodeArray[i++] = NULL); //Assign the NULL value to them

		for (int i = 0; i < numPaths; ++i)
		{
			nodeArray[i] = new MazeNodeTwo(getPrizeNum(), LEVEL_TWO_NUM_DIRECTIONS, 0);
			nodeArray[i]->setNextNodePtr(0, parentNode); //Connect for the backwards direction.

			if ((numNodes--) == 1) //If we reached the node limit, then set the last node to be the finish
				nodeArray[i]->resetFinish();
		}

		return nodeArray;
	}

	return NULL;

}

void MazeLevelTwo::connectChildren(MazeNodeTwo* parentNode, MazeNodeTwo** nodeArray, int numPaths) const
{
	int arrayIndex = 0;
	int pathArray[4] = {-1, -1, -1, -1}; //Initially, none of the indices of pathPtr are connected
	int numElements = 0; //Note we can have a maximum of four directions (excluding backwards)
	while (arrayIndex < numPaths)
	{
		int pathIndex;
		while (contains((pathIndex = (rand() % (LEVEL_TWO_NUM_DIRECTIONS - 1) + 1)),
				pathArray, numElements)); //Get the next available pathIndex to connect

		parentNode->setNextNodePtr(pathIndex, nodeArray[arrayIndex++]); //Connect the parent to its child
		pathArray[numElements++] = pathIndex; //Update pathArray, while also incrementing numElements
	}
}

bool MazeLevelTwo::contains(int pathIndex, const int pathArray[], int numElements) const
{
	bool contains = false;
	int arrayIndex = 0;

	while (arrayIndex < numElements && !contains) //Check each index of pathArray
		contains = (pathIndex == pathArray[arrayIndex++]);

	return contains;
}

void MazeLevelTwo::setPlayer(Player* newPlayer)
{
	if (playerPtr) //If a player exists, we want to reset the prizes in the maze
		resetNodes(listArray[0]);

	playerPtr = newPlayer; //Put the new player at the start of the maze.
	playerLoc = listArray[0];
}

int MazeLevelTwo::movePlayer(int nextPathIndex)
{
	int canMove = (playerPtr && 0 <= nextPathIndex && nextPathIndex < LEVEL_TWO_NUM_DIRECTIONS);
	if (canMove)
	{
		if (playerLoc->getNextNodePtr(nextPathIndex)) //Valid movement
		{
			canMove = 0;
			playerLoc = playerLoc->getNextNodePtr(nextPathIndex); //Move the player
			if (!playerLoc->isFinish()) //Player did not reach the finish
			{
				switch (playerLoc->getPrizeNum()) //Check if the player won a prize
				{
					case 1: //Player won a coin
						std::cout << "Congratulations! You've received a coin!" << std::endl;
						playerPtr->incrementQuantity(1);
						std::cout << "You have " << playerPtr->getQuantity(1)
							<< " coins." << std::endl;
						playerLoc->resetPrize();
						break;
					case 2: //Player won a power
						std::cout << "CONGRATULATIONS!! You've received a power!" << std::endl;
						std::cout << "Activate it to reveal the next location that";
						std::cout << " will lead you to the finish line!" << std::endl;
						playerPtr->incrementQuantity(2);
						std::cout << "You have " << playerPtr->getQuantity(2)
							<< " powers to use." << std::endl;
						playerLoc->resetPrize();
						break;
				}
				std::cout << "Successfully moved ";
				switch(nextPathIndex) //Tell the player which way they moved
				{
					case 1:
						std::cout << "straight.";
						break;
					case 2:
						std::cout << "left.";
						break;
					case 3:
						std::cout << "right.";
						break;
					case 4:
						std::cout << "diagonal.";
						break;
					default:
						std::cout << "backwards.";
						break;
				}
				std::cout << std::endl;
			}
			else //Finish node is reached, so we calculate the player's score.
			{
				std::cout << "CONGRATULATIONS!!! You've successfully reached the finish line!" << std::endl;
				if (playerLoc->getPrizeNum() > 0) //Check if the finish node
				{				//also contains a prize
					std::cout << "AND you're lucky! You've just stumbled upon an extra ";
					switch (playerLoc->getPrizeNum())
					{
						case 1:
							std::cout << "coin!" << std::endl;
							playerPtr->incrementQuantity(1);
							break;
						case 2:
							std::cout << "power!" << std::endl;
							playerPtr->incrementQuantity(2);
							break;
						default:
							break;
					}
				}
				canMove = -1;

			}
		}
		else if (nextPathIndex > 0) //Dead end, > 0 accounts for the case where player moves
					//backwards from the starting position
		{
			std::cout << "You've hit a dead end and lost a life!" << std::endl;
			playerPtr->decrementQuantity(0); //Decrement the life

			if (playerPtr->getQuantity(0) == 0) //Game over if the player has no lives
				canMove = -2;
			else //Good to go otherwise, we tell the player how many lives they have left.
			{
				std::cout << "You have " << playerPtr->getQuantity(0) << " lives left, ";
				std::cout << "and are still at the same intersection." << std::endl;
				canMove = 0;
			}
		}
		else //Player tries to move backwards, but they are at the start so they can't
		{
			canMove = 0;
			std::cout << "Cannot move backwards since you are already at the start!" << std::endl;
		}
	}

	return canMove;
}

const char** MazeLevelTwo::getPossiblePaths() const //Return all the possible directions
{
	const char** optionPtr = new const char*[LEVEL_TWO_NUM_DIRECTIONS];
	optionPtr[0] = "Go backwards.";
	optionPtr[1] = "Go straight.";
	optionPtr[2] = "Go left.";
	optionPtr[3] = "Go right.";
	optionPtr[4] = "Go diagonal.";

	return optionPtr;
}

int MazeLevelTwo::getNumPaths() const //Return the maximum possible paths for this maze
{
	return LEVEL_TWO_NUM_DIRECTIONS;
}

bool MazeLevelTwo::findPathToFinish(MazeNodeTwo* nodePtr) const
{
	if (nodePtr)
	{
		int nextPathIndex = 1;
		bool isFinish = (nodePtr->isFinish()); //If curent node is the finish, we are done.
		while (!isFinish && nextPathIndex < LEVEL_TWO_NUM_DIRECTIONS) //Otherwise, check its adjacent
			isFinish = findPathToFinish(nodePtr->getNextNodePtr(nextPathIndex++)); //nodes

		return isFinish; //Returns true if path does not exist, false otherwise
	}
	return false; //Occurs if we've reached a dead end
}

bool MazeLevelTwo::reveal() const
{
	bool canReveal = (playerPtr && playerPtr->getQuantity(2) > 0); //We can only reveal
	if (canReveal) //if there is a player in the maze, and that player has at least one power to use
	{
		int pathIndex = 1;
		playerPtr->decrementQuantity(2); //Reduce the available powers by one
		std::cout << "Consuming your power..." << std::endl;

		for (; pathIndex < LEVEL_TWO_NUM_DIRECTIONS && //Check if path exists for all its children
			!findPathToFinish(playerLoc->getNextNodePtr(pathIndex)); ++pathIndex);

		switch (pathIndex) //Tell user which direction to go to reach the finish
		{
			case 1: //pathIndex of 1 means we go straight
				std::cout << "Go straight to get to the maze exit!" << std::endl;
				break;
			case 2: //pathIndex of 2 means we go left
				std::cout << "Go left to get to the maze exit!" << std::endl;
				break;
			case 3: //pathIndex of 3 means we go right
				std::cout << "Go right to get to the maze exit!" << std::endl;
				break;
			case 4: //pathIndex of 4 means we go diagonal
				std::cout << "Go diagonal to get to the maze exit!" << std::endl;
				break;
			default: //Otherwise, none of the next intersections reach the finish. So we go backwards
				std::cout << "Go backwards to get to the maze exit!" << std::endl;
				break;
		}
	}
	else //No powers to consume
		std::cout << "You have no power to use!" << std::endl;

	return canReveal;
}

int MazeLevelTwo::calcScore() const
{
	if (playerPtr) //Calculate the score only if a player exists in the maze
	{
		int score = 0;

		for (int i = 0; i < NUM_COLLECTIBLES; ++i)
		{
			int multiplier = MULTIPLIERS[i]; //Values of multiplies are contained in MazeInterface
			score += multiplier * (playerPtr->getQuantity(i));
		}

		return score;
	}
	else //Return dummy value if the condition is not met.
		return -1;
}

void MazeLevelTwo::resetNodes(MazeNodeTwo* nodePtr) //Pre-order reset of the nodes
{
	if (nodePtr)
	{
		nodePtr->resetPrize(); //Reset current node first, then all of its adjacent nodes
		for (int i = 1; i < LEVEL_TWO_NUM_DIRECTIONS; ++i)
			resetNodes(nodePtr->getNextNodePtr(i));
	}
}

void MazeLevelTwo::startOver() //Reset the maze
{
	if (playerPtr)
	{
		playerLoc = listArray[0];
		playerPtr->resetNumItems();
	}
	resetNodes(listArray[0]);
}

void MazeLevelTwo::displayMaze() const //Display the information in listArray
{
	std::cout << "   S L R D" << std::endl; //S = Straight, L = Left, R = Right, D = Diagonal
	for (int i = 0; i < arraySize && listArray[i]; ++i)
	{
		MazeNodeTwo* currentNode = listArray[i]; //Get the next node to visit
		if (!currentNode->isFinish()) //If node isn't the finish
		{
			if (playerLoc == currentNode)
				std::cout << "P  ";
			else
				std::cout << "O  ";

			for (int i = 1; i < LEVEL_TWO_NUM_DIRECTIONS; ++i)
			{ //This loop puts all of the non-dead end or NULL adjacent nodes into the queue to
				MazeNodeTwo* nextNode = currentNode->getNextNodePtr(i); //visit next
				if (nextNode) //Not a dead-end, so we put it into the queue for display
				{
					if (!nextNode->isFinish())
					{
						if (playerLoc == nextNode)
							std::cout << "P ";
						else
							std::cout << "O ";
					}
					else
						std::cout << "F ";
				}
				else
					std::cout << "X ";
			}

		}
		else //It is the finish node. Recall finish node has no connecting intersections
		{
			std::cout << "F";
			std::cout << "  - - - -";
		}
		std::cout << std::endl;
	}
}














