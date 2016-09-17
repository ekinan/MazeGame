#ifndef _STACK_H
#define _STACK_H

/*
Stack class, implemented as a chain of linked nodes.
Contains the relevant functions for a stack object.
*/

#include "Node.h"

template <class ItemType>
class Stack
{
public:
	Stack();
	Stack(const Stack&); //Copy constructor
	~Stack();

	bool empty() const; //Checks if stack is empty
	bool push(const ItemType& item); //Pushes an item onto the stack
	bool pop(); //Pops the stack
	ItemType top() const; //Returns the item at the top of the stack if it is nonempty, otherwise program
				//terminates.
private:
	Node<ItemType>* topPtr; //Pointer to the top of the stack
};

#include "Stack.cpp"

#endif
