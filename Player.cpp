/*
This is the implementation file for Player.h, containing the definitions of the necessary member functions
*/

#include "Player.h"

Player::Player(const char** personalInfo) : Person(personalInfo)
{
	resetNumItems(); //Reset the values of numItems
}

Player::~Player()
{
}

bool Player::incrementQuantity(int index)
{
	bool canIncrement = (0 <= index && index < NUM_COLLECTIBLES); //Check if index is within bounds
	if (canIncrement) //If it is, increment the quantity at numItems[index]
		numItems[index]++;

	return canIncrement;
}

bool Player::decrementQuantity(int index)
{
	//Unlike incrementQuantity, decrementQuantity also requires numItems[index] to be > 0 prior
	//to decrementing it, since we cannot have negative collectibles.
	bool canDecrement = (0 <= index && index < NUM_COLLECTIBLES && numItems[index] > 0);
	if (canDecrement)
		numItems[index]--;

	return canDecrement;
}

int Player::getQuantity(int index) const
{
	if (0 <= index && index < NUM_COLLECTIBLES) //Check if index is within array bounds.
		return numItems[index];
	else //Return dummy value if it isn't
		return -1;
}

void Player::resetNumItems()
{
	numItems[0] = DEFAULT_LIVES; //Reset the number of lives to DEFAULT_LIVES
	for (int i = 1; i < NUM_COLLECTIBLES; ++i) //All other collectibles are reset to 0.
		numItems[i] = 0;
}

Player& Player::operator=(const Player& other)
{
	if (this != &other)
	{
		Person* ptr1 = this; //Use polymorphism to access = operator defined in base class
		const Person* ptr2 = &other; //to create deep copies of base class parameter info

		*ptr1 = *ptr2;

		for (int i = 0; i < NUM_COLLECTIBLES; ++i) //Now copy the additional derived class parameter
			numItems[i] = other.numItems[i];
	}

	return *this;
}

