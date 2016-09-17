#include <iostream>
#include <iomanip>

#include "MazeLevelOne.h"
#include "MazeLevelTwo.h"

/*
Author: Enis K Inan
Date: 07/17/2015

The following program allows the user to play a game. They are in a maze, and are allowed to move
only a certain number of directions. Along the way, they may encounter pitfalls/dead ends, where they
will lose a life, or encounter prizes to use in their journey or store for bonus points. There are two
prizes in this game: coins and lives. A coin is simply used to increase the player's score, while having
a power allows the player a chance to reveal where they should go next at a particular intersection to
reach the finish.

The player's name and background are stored and displayed on the screen. Throughout their journey,
the player can choose to move in the maze, start over, or try another maze. If they run out of lives,
they have the option to either go back to the beginning, try another maze, or exit the game. If, however,
they finish the maze, their score will be calculated, and they will be taken back to the main menu of the
program.

There are two levels, Level 1 and Level 2 in the maze. The player can select which to play in from the
main menu.
*/

using namespace std;

//Default number of nodes for Level 1 of the maze. Please keep MAZE_NODES <= 8
//when using this program for Level 1 of the maze, as the displayMaze function will overflow the console
//for node values greater than this, unless you do not care to see the tree-structure of the maze.
//This is acceptable as it was intended only to verify the randomness of the maze. Do not worry about this if you are running Program 2.
const int MAZE_NODES_ONE = 8;

const int MAZE_NODES_TWO = 15; //Default nodes for Level 2;

/*
Pauses the program, continues once the user enters any key
*/
void pause();


/*
inputHandler, displayOptions, and isValidChoice handle the input for the program.
*/

/*
Returns a valid, user input.
@param: numOptions - Stores the number of options the user has
options[] - Stores the list of options for the user
@return: A valid user input in the range of 0 to (numOptions - 1)
*/
int inputHandler(int numOptions, const char* const options[]);

/*
Displays the user options.
@param: numOptions - The number of options to be dsplayed
optionsp[] - Stores the list of options for the user
*/
void displayOptions(int numOptions, const char* const options[]);

/*
Checks if the user choice is valid, where a valid value is defined as 0 <= userChoice < numOptions
@param: userChoice : The user's choice
numOptions : The number of options available for the user
@return: 1 if the userChoice is valid, 0 otherwise
*/
bool isValidChoice(int userChoice, int numOptions);


/*
Gets the player information from the user, specifically their name and background.
Returns a pointer to the corresponding player object that's created from this info.
*/
Player* getPlayerInfo();

/*
Function to play the maze game. Here, the player information is acquired. They are then introduced
to the game via displayMazeInfo, and then they are given 6 options to choose from. These are:

0. Move
1. Start over
2. Try a new maze
3. Display maze
4. Main menu

Depending on the choice, the program moves to the functions movePlayer (for 0), startOver (for 1),
newMaze (for 2), displayMaze (for 3), or back to main() (4).

@param: userChoice: 0 = Level 1, 1 = Level 2

@return Returns the player's total score for the games they playe if they exit back to the main menu.
*/
int playGame(int userChoice);

/*
Welcomes the player to the game and displays information about the maze
*/
void displayMazeInfo(Player* newPlayer);

/*
Here, the player is allowed to traverse the maze. The possible paths they can take
depends on what level of the maze is being implemented. They are allowed to move around in the maze
until they either (1) indicate that they want to return to the game menu, (2) are out of lives, or
(3) are at the finish. They can also use any powers they've acquired to reveal which direction leads
to the exit.

@return: Returns -1 if the player reached the finish, -2 if the player is out of lives, or 0 otherwise.
*/
int movePlayer(MazeInterface* mazePtr);

/*
Displays the entire maze, along with the corresponding legend for each symbol in the output.

NOTE: If you are going to be using Level 1 of the maze, please be sure that MAZE_NODES is less than 10,
otherwise the level-order style of the display will overflow the console
*/
void displayMaze(MazeInterface* mazePtr);

/*
Resets the entire maze, and moves the player back to the beginning. Calls the startOver()
function of the maze object.
*/
void startOver(MazeInterface* mazePtr);

/*
Creates a new maze object.
*/
void newMaze(MazeInterface* &mazePtr);

/*
Handles the case of game over, is called when movePlayer returns -2 to playGame.
Player can either 0) Start over, (1) Try a new maze, or (2) exit the game.

@return Returns the action taken by the player. -1 if they started a new game, 0 if they started over,
or 1 if they chose to quit the game.
*/
int gameOver(MazeInterface* &mazePtr);

int main()
{
	srand(time(0));
	int userChoice;

	//Create the options array for input handler
	const int NUM_OPTIONS = 3;
	const char* options[NUM_OPTIONS] = {"Play Level 1", "Play Level 2",  "Quit"};

	do
	{
		int playerScore;

		system("clear");
		cout << "Welcome to the maze game!! Please enter your choice from the options below!" << endl;
		userChoice = inputHandler(NUM_OPTIONS, options); //Get the user choice

		if (userChoice != (NUM_OPTIONS - 1)) //Means that they want to play the game
		{
			playerScore = playGame(userChoice); //Take them to the menu to do that
			system("clear");
			cout << "Your final score for the level(s) you played: " << playerScore << endl << endl;
			pause();
		}
	} while (userChoice != (NUM_OPTIONS-1));

	return 0;
}

void pause()
{
	char choice;
	cout << "Enter any key to continue: ";
	cin >> choice;

	cin.clear();
	cin.ignore(1000, '\n');
}

int inputHandler(int numOptions, const char* const options[])
{
	int userChoice = -1;

	displayOptions(numOptions, options);
	cout << "Choice: ";
	cin >> userChoice;
	cout << endl;

	//Cycles the input until a valid input is made
	while (!cin || !isValidChoice(userChoice, numOptions) )
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Invalid input! Please enter your choice from the options below." << endl;

		displayOptions(numOptions, options);
		cout << "Choice: ";
		cin >> userChoice;
		cout << endl;
	}

	cin.clear();
	cin.ignore(1000, '\n');

	return userChoice;
}

void displayOptions(int numOptions, const char* const options[])
{
	for (int i = 0; i < numOptions; ++i)
		cout << setw(5) <<  i << ". " << options[i] << endl;
	cout << endl;
}

bool isValidChoice(int userChoice, int numOptions)
{
	return ((0 <= userChoice) && (userChoice < numOptions));
}

int playGame(int userChoice)
{
	int controlVar;
	int userScore = 0;

	//Create options array for input handler
	const int NUM_OPTIONS = 5;
	const char* options[NUM_OPTIONS] = {"Move player", "Start over", "Try a new maze",
					"Display Maze (GRADER ONLY)", "Main Menu"};

	Player* newPlayer = getPlayerInfo();
	system("clear");

	MazeInterface* mazePtr = NULL;
	if (userChoice)
		mazePtr = new MazeLevelTwo(MAZE_NODES_TWO);
	else
		mazePtr = new MazeLevelOne(MAZE_NODES_ONE);
	mazePtr->setPlayer(newPlayer);


	displayMazeInfo(newPlayer);
	pause();

	do
	{
		system("clear");
		cout << setw(20) << "Player Status" << endl; //Display the player status, and how many
		cout << "Name: " << newPlayer->getInfo(0) << endl; //collectibles they have
		cout << "Background: " << newPlayer->getInfo(1) << endl;
		cout << "Lives: " << newPlayer->getQuantity(0) << endl;
		cout << "Coins: " << newPlayer->getQuantity(1) << endl;
		cout << "Power: " << newPlayer->getQuantity(2) << endl << endl;

		cout << "Please select from one of the six options below to proceed" << endl;

		controlVar = inputHandler(NUM_OPTIONS, options); //Get the player input

		switch (controlVar)
		{
			case 0: //Player wants to move
				controlVar = movePlayer(mazePtr);
				if (controlVar == -1) //The player reached the finish
				{
					userScore += mazePtr->calcScore(); //Calc score and
					controlVar = NUM_OPTIONS - 1; //exit the game
				}
				else if (controlVar == -2) //Game over, player's out of lives
				{
					controlVar = gameOver(mazePtr); //Check what player wants to do next
					if (controlVar == 1) //Player wants to quit
						controlVar = NUM_OPTIONS - 1;
					else if (controlVar == -1) //New game was started
					{
						newPlayer->resetNumItems();
						mazePtr->setPlayer(newPlayer);
					}
				}
				break;

			case 1: //Player wants to start over
				startOver(mazePtr);
				pause();
				break;
			case 2: //Player wants to start a new maze
				newMaze(mazePtr);
				newPlayer->resetNumItems();
				mazePtr->setPlayer(newPlayer);
				pause();
				break;
			case 3: //Display the maze
				displayMaze(mazePtr);
				cout << endl;
				pause();
				break;
			default: //Player wants to exit
				break;
		}

	} while (controlVar != NUM_OPTIONS - 1);

	delete newPlayer; //Deallocate the player and maze memory
	delete mazePtr;

	return userScore;
}

void displayMazeInfo(Player* newPlayer) //Displaying the maze information
{
	cout << "Welcome " << newPlayer->getInfo(0) << "!" << " The maze has been created." << endl;
	cout << "Your objective is to safely navigate to the maze exit." << endl;
	cout << "Along the way, you will find coins that will improve your score or, if lucky,";
	cout << " you might even get a power!" << endl;
	cout << "The power(s) can either be used to foresee the next path to take to the exit or " << endl;
	cout << "saved up to get a big bonus in your score after you complete the level!" << endl;
	cout << "You start with " << newPlayer->getQuantity(0) << " lives." << endl;
	cout << "Every dead end that you hit, you will lose a life! So be sure to keep track" << endl;
	cout << "of your movements so that you do not make the same mistake twice!" << endl << endl;
	cout << "Good luck! And may the force be with you." << endl << endl;
}

int movePlayer(MazeInterface* mazePtr)
{
	const char** paths = mazePtr->getPossiblePaths(); //Get the possible paths the player can take
	int numPaths = mazePtr->getNumPaths(); //Get the number of paths the player can take
	int numOptions = numPaths + 2; //Adding 2 for "Use power" and "Exit"
	const char** options = new const char*[numOptions];
	int userChoice;
	int moveResult = 0;

	int index = 0;
	for (; index < numPaths; ++index) //Generate the options array, first few indices
		options[index] = paths[index]; //are the possible directions the player can take
	options[index++] = "Use power"; //Next option is to let the player use a power
	options[index] = "Return to game options"; //Last option is to take them back to the game options

	do
	{
		system("clear");
		cout << "Please select one of the " << numOptions << " options below to proceed." << endl;
		userChoice = inputHandler(numOptions, options);

		if (userChoice < numPaths) //Player decided to move
		{
			moveResult = mazePtr->movePlayer(userChoice);
			if (moveResult != 0) //Player either lost all their lives or reached the exit
				userChoice = numOptions-1;
			cout << endl;
			pause();
		}
		else if (userChoice == numPaths) //Player decided to use a power
		{
			mazePtr->reveal();

			cout << endl;
			pause();
		}

	} while (userChoice != index);

	delete [] paths; //Deallocate dynamic memory for const char pointers
	delete [] options; //Note that the strings stored in the indices are deallocated after program execution
			   //since they are read-only memory.
	return moveResult;
}

void displayMaze(MazeInterface* mazePtr)
{
	mazePtr->displayMaze();
	cout << endl;
	cout << "Legend:" << endl;
	cout << setw(5);
	cout << "P = Player location" << endl;
	cout << "O = Intersection" << endl;
	cout << "X = Dead end" << endl;
}

void startOver(MazeInterface* mazePtr)
{
	cout << "Resetting your stats..." << endl;
	mazePtr->startOver();
	cout << "Done! You are back at the beginning of the maze." << endl << endl;
}

void newMaze(MazeInterface* &mazePtr)
{
	MazeInterface* oldMaze = mazePtr;

	cout << "Creating a new maze.." << endl;

	if (dynamic_cast<MazeLevelOne*>(oldMaze))
		mazePtr = new MazeLevelOne(MAZE_NODES_ONE);
	else //It is Level 2
		mazePtr = new MazeLevelTwo(MAZE_NODES_TWO);

	delete oldMaze;

	cout <<"Done!" << endl;
}


int gameOver(MazeInterface* &mazePtr)
{
	system("clear");

	int returnVal = 0;

	//Creating the user options
	const int NUM_OPTIONS = 3;
	const char* options[NUM_OPTIONS] = {"Start over", "Try a new maze", "Exit game"};
	int userChoice;

	cout << "Game over, you are out of lives!" << endl;
	cout << "Please select your next course of action from the available ones below." << endl;

	userChoice = inputHandler(NUM_OPTIONS, options); //Get the user input

	switch (userChoice)
	{
		case 0: //0 = the user wants to start over
			startOver(mazePtr);
			pause();
			break;
		case 1: //1 = user wants to try a new maze
			returnVal = -1;
			newMaze(mazePtr);
			pause();
			break;
		default: //User wants to exit the game
			returnVal = 1;
			break;
	}

	return returnVal;
}

Player* getPlayerInfo()
{
	system("clear");
	const int MAX_LENGTH = 50; //Maximum of 50 characters for the name, 100 for the background
	char name[MAX_LENGTH], background[2*MAX_LENGTH];

	const char** info = new const char*[2]; //Generate the info array to put into the player constructor
	info[0] = name;
	info[1] = background;

	cout << "Please enter your name." << endl; //Get the inputs
	cout << "Name: ";
	cin.get(name, MAX_LENGTH);
	cout << endl;
	cin.clear();
	cin.ignore(1000, '\n');

	cout << "Please enter some background information about yourself." << endl;
	cout << "Background: ";
	cin.get(background, 2*MAX_LENGTH);
	cin.clear();
	cin.ignore(1000, '\n');
	cout << endl;

	Player* temp =  new Player(info); //Create the player object

	delete [] info; //Free the memory from info

	return temp;
}
