#ifndef _STACK_CPP
#define _STACK_CPP

/*
Implementation file corresponding to Stack.h, containing the definitions of the member functions.
*/

#include "Stack.h"
#include <cassert>

template <class ItemType>
Stack<ItemType>::Stack() : topPtr(NULL)
{
}

template <class ItemType>
Stack<ItemType>::Stack(const Stack& otherStack)
{
	topPtr = NULL;
	if (otherStack.topPtr) //Copy contents of the other stack
	{
		topPtr = new Node<ItemType>(otherStack.topPtr->getItem());

		Node<ItemType>* otherCurrent = otherStack.topPtr->getNext();
		Node<ItemType>* storeFirstPtr = topPtr;

		while (otherCurrent)
		{
			topPtr->setNext(new Node<ItemType>(otherCurrent->getItem()));
			topPtr = topPtr->getNext();
			otherCurrent = otherCurrent->getNext();
		}

		topPtr->setNext(NULL);
		topPtr = storeFirstPtr;
	}
}

template <class ItemType>
Stack<ItemType>::~Stack()
{
	while (topPtr) //Delete contents of the stack
	{
		Node<ItemType>* storeNext = topPtr->getNext();
		delete topPtr;
		topPtr = storeNext;
	}
}

template <class ItemType>
bool Stack<ItemType>::empty() const
{
	return (!topPtr);
}

template <class ItemType>
bool Stack<ItemType>::push(const ItemType& item)
{
	Node<ItemType>* prev = topPtr; //Store previous top of the stack and insert item on top
	topPtr = new Node<ItemType>(item);
	topPtr->setNext(prev);

	return true;
}

template <class ItemType>
bool Stack<ItemType>::pop()
{
	bool ableToPop = !empty();
	if (ableToPop) //Pops top of the stack if it is nonempty.
	{
		Node<ItemType>* storeNext = topPtr->getNext();
		delete topPtr;
		topPtr = storeNext;
	}

	return ableToPop;
}

template <class ItemType>
ItemType Stack<ItemType>::top() const
{
	assert(!empty()); //Terminates program if top() is called with an empty stack.
	return topPtr->getItem();
}

#endif
