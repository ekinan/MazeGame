/*
This is the implementation file corresponding to MazeLevelOne.h, where the definitions of the member functions are written.
*/

#include "MazeLevelOne.h"
#include "Queue.h"

#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <iomanip>

MazeLevelOne::MazeLevelOne(int numNodes)
	: playerPtr(NULL), playerLoc(NULL), startPtr(new MazeNodeOne(0, LEVEL_ONE_NUM_DIRECTIONS, 0,
							rand() % KEY_DIST_SIZE))
{
	if (numNodes <= 2) //At least two nodes have to occur, we can't have the starting node
		numNodes = DEFAULT_LEVEL_ONE_NODE_NUM; //also be the finish

	buildMaze(--numNodes); //We decrement by 1 since we've already created the starting node
}

MazeLevelOne::MazeLevelOne(const MazeLevelOne& otherMaze)
{
	playerPtr = NULL; //We don't want the same player, only the same maze
	playerLoc = NULL;

	startPtr = copyTree(startPtr, otherMaze.startPtr); //Copy the shape of the maze
}

MazeLevelOne::~MazeLevelOne()
{
	postOrderDelete(startPtr);
}

void MazeLevelOne::buildMaze(int numNodes)
{
	MazeNodeOne* newNodePtr = NULL;

	do //The following loop inserts nodes into the maze until an amount equivalent to numNodes
	{ //has been inserted.
		int newKey = rand() % KEY_DIST_SIZE; //Randomly generate the sort key for the node
		bool atEnd = (numNodes == 1); //Last node is the finish, so this will be 1 for that case


		//The code section below generates the prize of the node
		//Note that coints have a 1/3 chance of occuring, while a power
		//occurs with a 1/n chance, where n is the prize distribution size.
		int prizeCode = rand() % LEVEL_ONE_PRIZE_DIST_SIZE;
		if (prizeCode <= LEVEL_ONE_COIN_MAX) //Node has a coin
			prizeCode = 1;
		else if (prizeCode == LEVEL_ONE_COIN_MAX + 1) //Node has a power
			prizeCode = 2;
		else //No prize
			prizeCode = 0;


		//Create the new node, and insert it to its corresponding location in the tree.
		newNodePtr = new MazeNodeOne(prizeCode, LEVEL_ONE_NUM_DIRECTIONS, atEnd, newKey);
		startPtr = insertInLoc(startPtr, newNodePtr);

		numNodes--;
	} while (numNodes >= 1);
}

MazeNodeOne* MazeLevelOne::insertInLoc(MazeNodeOne* subTreePtr, MazeNodeOne* newNodePtr)
{
	if (subTreePtr)
	{
		MazeNodeOne* nextNode;
		if (newNodePtr->getSortKey() < subTreePtr->getSortKey())
		{ //newNode belongs to the left subtree
			nextNode = insertInLoc(subTreePtr->getNextNodePtr(1), newNodePtr);
			subTreePtr->setNextNodePtr(1, nextNode);
			nextNode->setNextNodePtr(0, subTreePtr); //Connect new node to the parent (or reconnect)
		}
		else //Move to right subtree, for duplicate keys too
		{
			nextNode = insertInLoc(subTreePtr->getNextNodePtr(2), newNodePtr);
			subTreePtr->setNextNodePtr(2, nextNode);
			nextNode->setNextNodePtr(0, subTreePtr);
		}

		return subTreePtr; //For reconnecting
	}

	return newNodePtr; //Found the insertion location

}

void MazeLevelOne::postOrderDelete(MazeNodeOne* subTreePtr)
{
	if (subTreePtr)
	{
		postOrderDelete(subTreePtr->getNextNodePtr(1));
		postOrderDelete(subTreePtr->getNextNodePtr(2));
		delete subTreePtr;
	}
}

MazeNodeOne* MazeLevelOne::copyTree(MazeNodeOne* subTreePtr, MazeNodeOne* otherTreePtr)
{
	if (otherTreePtr)
	{
		subTreePtr = new MazeNodeOne(*otherTreePtr); //Copy the contents of the node

		subTreePtr->setNextNodePtr(1, copyTree(subTreePtr->getNextNodePtr(1), //Copy left subtree
							otherTreePtr->getNextNodePtr(1)));
		(subTreePtr->getNextNodePtr(1))->setNextNodePtr(0, subTreePtr); //Connect for backwards direction

		subTreePtr->setNextNodePtr(2, copyTree(subTreePtr->getNextNodePtr(2), //Copy right subtree
							otherTreePtr->getNextNodePtr(2)));
		(subTreePtr->getNextNodePtr(2))->setNextNodePtr(0, subTreePtr);

		return subTreePtr;
	}

	return NULL;
}

void MazeLevelOne::setPlayer(Player* newPlayer)
{
	if (playerPtr) //There was a game being played prior to setting the player, so we reset
		resetNodes(startPtr); //the maze

	playerPtr = newPlayer;
	playerPtr->resetNumItems(); //Reset the collectibles of the player in case they exist
	playerLoc = startPtr;
}

int MazeLevelOne::movePlayer(int nextPathIndex)
{
	//Move only if a plyer exists and nextPathIndex is within the array bounds
	int canMove = (playerPtr && 0 <= nextPathIndex && nextPathIndex < LEVEL_ONE_NUM_DIRECTIONS);
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
				std::cout << "Successfully moved "; //Tell the player the move
				switch(nextPathIndex) //was successful
				{
					case 1: //Index 1 = left
						std::cout << "left.";
						break;
					case 2: //Index 2 = right
						std::cout << "right.";
						break;
					default: //Index 0 = backwards
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

const char** MazeLevelOne::getPossiblePaths() const
{
	const char** optionPtr = new const char*[LEVEL_ONE_NUM_DIRECTIONS];
	optionPtr[0] = "Go backwards";
	optionPtr[1] = "Go left";
	optionPtr[2] = "Go right";

	return optionPtr;
}

int MazeLevelOne::getNumPaths() const
{
	return LEVEL_ONE_NUM_DIRECTIONS;
}


bool MazeLevelOne::findPathToFinish(MazeNodeOne* subTreePtr) const
{
	if (subTreePtr)
	{
		if (subTreePtr->isFinish()) //If node is the maze exit, we see that the path does exist
			return true;
		else //Internal node
		{
			bool pathExists = findPathToFinish(subTreePtr->getNextNodePtr(1)); //Check left subtree
			if (!pathExists) //if the path exists. If it doesn't we check the right subTree
				pathExists = findPathToFinish(subTreePtr->getNextNodePtr(2));

			return pathExists; //Returns true if path does exist, false otherwise
		}
	}
	else //We reached a dead end
		return false;
}

bool MazeLevelOne::reveal() const
{
	bool canReveal = (playerPtr && playerPtr->getQuantity(2) > 0); //We can only reveal
	if (canReveal) //if there is a player in the maze, and that player has at least one power to use.
	{
		playerPtr->decrementQuantity(2); //Reduce the available powers by one
		std::cout << "Consuming your power..." << std::endl;
		if (findPathToFinish(playerLoc->getNextNodePtr(1))) //Check if finish is to the left
			std::cout << "Go left to get to the maze exit!" << std::endl;
		else if (findPathToFinish(playerLoc->getNextNodePtr(2))) //Check if finish is to the right
			std::cout << "Go right to get to the maze exit!" << std::endl;
		else //Player has to go backwards
			std::cout << "Go backwards to get to the maze exit!" << std::endl;
	}
	else if (playerPtr) //No powers to consume
		std::cout << "You have no power to use!" << std::endl;

	return canReveal;
}

int MazeLevelOne::calcScore() const
{

	if (playerPtr) //Calculate the score only if a player exists in the maze
	{
		int score = 0;

		for (int i = 0; i < NUM_COLLECTIBLES; ++i)
		{
			int multiplier = MULTIPLIERS[i];
			score += multiplier * (playerPtr->getQuantity(i));
		}

		return score;
	}
	else //Return dummy value if the condition is not met.
		return -1;
}

void MazeLevelOne::resetNodes(MazeNodeOne* subTreePtr) //Pre-order reset of the nodes
{
	if (subTreePtr)
	{
		subTreePtr->resetPrize();
		resetNodes(subTreePtr->getNextNodePtr(1));
		resetNodes(subTreePtr->getNextNodePtr(2));
	}
}

void MazeLevelOne::startOver()
{
	if (playerPtr)
	{
		playerLoc = startPtr;
		playerPtr->resetNumItems();
	}
	resetNodes(startPtr);
}

int MazeLevelOne::getHeight(MazeNodeOne* subTreePtr) const
{
	if (subTreePtr) //Height is 1 + max(height(left subtree), height(right subtree))
		return 1 + std::max(getHeight(subTreePtr->getNextNodePtr(1)),
					getHeight(subTreePtr->getNextNodePtr(2)));
	else //Note that an empty tree has height 0
		return 0;
}

int MazeLevelOne::powTwo(int exponent) const
{
	return (1 << exponent); //Left shift to compute the power of 2
}

void MazeLevelOne::displayMaze() const //Do the level order traversal of the maze
{
	int treeHeight = getHeight(startPtr);
	int currentLevel = 1;
	int maxNodes = powTwo(currentLevel) - 1; //Get the maximum nodes for the current level of the tree
	int nodesVisited = 0; //Stores the number of nodes visited, used to move to the next level

	int nodeSpacing = powTwo(treeHeight); //Node spacing is calculated as 2^(treeHeight - (currentLevel - 1))
						//but here, currentLevel = 1, so currentLevel - 1 = 0

	Queue<MazeNodeOne*> ptrQueue; //Store the next pointer to display
	Queue<bool> nullQueue; //This indicates whether or not the null pointer corresponds to a dead-end
				//or if we need to skip it in order to output the tree in the correct,
				//level order format. 1 = we need to display the dead-end, 0 = we skip it
	ptrQueue.push(startPtr);
	std::cout << std::setw(nodeSpacing);

	while (!ptrQueue.empty()) //While we have no nodes in the ptrQueue
	{
		MazeNodeOne* currentNode = ptrQueue.front(); //Get the next node we need to display
		ptrQueue.pop();
		if (currentNode) //We are at an intersection
		{
			nodesVisited++; //Increment nodesVisited

			if (!currentNode->isFinish()) //If the node is not the finish,
			{
				if (playerLoc != currentNode) //Check if the player is at the node
					std::cout << "O";
				else
					std::cout << "P";

				for (int i = 1; i < LEVEL_ONE_NUM_DIRECTIONS; ++i)
				{ //Place the children in the queue
					MazeNodeOne* childPtr = currentNode->getNextNodePtr(i);
					ptrQueue.push(childPtr);
					if (!childPtr)
						nullQueue.push(1); //We need to display this node, it's a dead end
				}
			}
			else //Node is the finish
			{
				std::cout << "F";
				ptrQueue.push(NULL);
				nullQueue.push(0); //No need to display the node, since the finish
				//has no dead-ends. But, we do need to skip the children of the imaginary
				//node to preserve the correct output format
			}

		}
		else //We have a null pointer, we check if we either display it or skip it
		{
			bool displayNull = nullQueue.front();
			nullQueue.pop();
			if (displayNull) //We display the dead end
			{
				nodesVisited++;
				std::cout << "X";
				ptrQueue.push(NULL); //After displaying, we need to skip this node
				nullQueue.push(0); //by imagining that it has two children, again to preserve
						   //output format
			}
			else //We skip the node
			{
				nodesVisited += 2; //A skip corresponds to "visiting" 2 nodes, since it is
				std::cout << ""; //a binary tree
				std::cout << std::setw(nodeSpacing);
				std::cout << "";
				if (currentLevel <= treeHeight) //We also need to skip the children
				{ //of the skipped nodes too (there are two) if the nodes are at a level
				 //either less than or equal to the tree height to preserve
					for (int i = 0; i < 2; ++i) //correct output format
					{
						ptrQueue.push(NULL);
						nullQueue.push(0);
					}
				}
			}

		}

		if (nodesVisited >= maxNodes) //Move down a level (i.e. put an endl)
		{			     //since we've visited the maximum possible nodes for that level
			nodeSpacing = powTwo(treeHeight - (currentLevel - 1)); //Compute the new node spacing
			currentLevel++; //Increment the level
			maxNodes = powTwo(currentLevel) - 1; //Compute the max possible nodes for the new level

			std::cout << std::endl; //Move to the next line
			std::cout << std::setw(nodeSpacing/2); //Spacing for the first node at the new level
		}						//is always nodeSpacing/2
		else //No need to skip, so we set the spacing of the next node to be displayed
			std::cout << std::setw(nodeSpacing);
	}

}




/*BELOW ARE THE ADDITIONAL FUNCTIONS REQUIRED FOR THE DATA STRUCTURE. NOTE THESE WERE NOT USED
FOR THE PRESENT PROGRAM!!!*/

void MazeLevelOne::traverse(MazeNodeOne* subTreePtr) const //Conduct an inorder traversal of the tree
{
	if (subTreePtr)
	{
		traverse(subTreePtr->getNextNodePtr(1));
		std::cout << subTreePtr->getSortKey() << " ";
		traverse(subTreePtr->getNextNodePtr(2));
	}
}

void MazeLevelOne::displayAll() const //Display the tree in order fashion
{
	traverse(startPtr);
}

MazeNodeOne* MazeLevelOne::findNode(MazeNodeOne* treePtr, int sortKey) const //Locate the node having a sortKey
{									    //of sortKey
	if (treePtr)
	{
		if (treePtr->getSortKey() == sortKey)
			return treePtr;
		else if (sortKey < treePtr->getSortKey())
			treePtr = findNode(treePtr->getNextNodePtr(1), sortKey);
		else
			treePtr = findNode(treePtr->getNextNodePtr(2), sortKey);
	}

	return treePtr;
}

int MazeLevelOne::getEntry(int sortKey) const //Return the sortKey of the node if it is in the tree,
{						//-1 otherwise
	MazeNodeOne* nodePtr = findNode(startPtr, sortKey);
	if (nodePtr)
		return nodePtr->getSortKey();
	else
		return -1; //Dummy value
}

MazeNodeOne* MazeLevelOne::removeValue(MazeNodeOne* treePtr, int sortKey, bool& successful)
{
	//We search for the node having a sortKeey of sortKey
	if (treePtr)
	{
		if (treePtr->getSortKey() == sortKey) //Found it
		{
			successful = true;
			treePtr = deleteNode(treePtr); //We delete the node
		}
		else if (sortKey < treePtr->getSortKey()) //Check left subtree
			treePtr->setNextNodePtr(1, removeValue(treePtr->getNextNodePtr(1), sortKey, successful));
		else //Check right subtree
			treePtr->setNextNodePtr(2, removeValue(treePtr->getNextNodePtr(2), sortKey, successful));
	}

	return treePtr;
}

MazeNodeOne* MazeLevelOne::swapSuccessor(MazeNodeOne* treePtr, MazeNodeOne* nodeToSwap)
{
	if (treePtr->getNextNodePtr(1)) //We haven't found the inorder successor
		treePtr->setNextNodePtr(1, swapSuccessor(treePtr->getNextNodePtr(1), nodeToSwap));
	else
	{
		*nodeToSwap = *treePtr; //Swap the contents of the node with its inorder succcessor

		MazeNodeOne* storeNext = treePtr->getNextNodePtr(2); //Get the right child of the inorder
		if (storeNext) //successor to connect to its parent
			storeNext->setNextNodePtr(0, treePtr->getNextNodePtr(0)); //Connect to parent if
										//child exists
		delete treePtr;
		treePtr = storeNext;
	}

	return treePtr; //For reconnecting
}

MazeNodeOne* MazeLevelOne::deleteNode(MazeNodeOne* treePtr)
{
	if (!treePtr->getNextNodePtr(1) && !treePtr->getNextNodePtr(2)) //Leaf
	{
		delete treePtr;
		return NULL;
	}
	else if (!treePtr->getNextNodePtr(2)) //No inorder successor, but node does have a left child
	{ //so we connect the leftChild to the node's parent, delete the node, and return a left child pointer
		MazeNodeOne* leftChild = treePtr->getNextNodePtr(1);
		leftChild->setNextNodePtr(0, treePtr->getNextNodePtr(0));
		delete treePtr;
		return leftChild;
	}
	else //There is an inorder successor, so we search for it, swap, and then remove it
	{
		treePtr->setNextNodePtr(2, swapSuccessor(treePtr->getNextNodePtr(2), treePtr));
		return treePtr;
	}
}

bool MazeLevelOne::removeEntry(int sortKey) //Removes the node having a sortKey of sortKey
{
	bool successful = false;
	startPtr = removeValue(startPtr, sortKey, successful);
	return successful;
}
