#ifndef _PLAYER_H
#define _PLAYER_H

/*
This is the Player class, derived from Person. A player is a person, except they contain a variety
of collectibles.
*/

#include "Person.h"

const int NUM_COLLECTIBLES = 3; //Default number of collectibles is a life, number of coins and power
const int DEFAULT_LIVES = 5; //Default lives are 5

class Player : public Person
{
public:
	Player(const char** personalInfo = NULL); //Default constructor
	~Player();

	/*
	Increments the quantity of the collectible at numItems[index] and returns true if successful,
	false otherwise.
	@param index: The index of numItems to be accessed
	@post The quantity at numItems[index] is incremented by 1 if index is within the array bounds,
	and a value of true is returned. Otherwise, nothing happens and a value of false is returned.
	*/
	bool incrementQuantity(int index);

	/*
	Decrements the quantity of the collectible at numItems[index] and returns true if successful,
	false otherwise.
	@param index: The index of numItems to be accessed.
	@post The quantity at numItems[index] is decremented by 1 if index is within the array bounds
	AND the value at numItems[index] is greater than 0 prior to decrementing it. Returns true
	if these conditions are met, false otherwise.
	*/
	bool decrementQuantity(int index);

	/*
	Returns the quantity of the collectible at numItems[index] if index is within the array bounds.
	@param index: The index of numItems to be accessed.
	@return Returns the value of numItems[index] if index is within bounds, otherwise it returns
	a dummy value of -1.
	*/
	int getQuantity(int index) const;

	/*
	Resets the values of the collectibles at numItems to their default values.
	These are: for index 0 (the lives), DEFUALT_LIVES
	for all other indices, 0.
	@post The value of numItems[0] is DEFAULT_LIVES, and the value of numItems[1], numItems[2], ...
	numItems[NUM_COLLECTIBLES - 1] is 0.
	*/
	void resetNumItems();


	/*
	Override the assignment operator
	*/
	Player& operator=(const Player&);
private:
	int numItems[NUM_COLLECTIBLES]; //Index 0 = lives, 1 = number of coins, 2 = number of power
};

#endif
