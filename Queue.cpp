#ifndef _QUEUE_CPP
#define _QUEUE_CPP

/*
This is the implementation file for the Queue class containing the definitions of the member functions in
Queue.h.
*/

#include "Queue.h"

#include <cassert>
#include <iostream>

template <class ItemType>
Queue<ItemType>::Queue() : backPtr(NULL)
{
}

template <class ItemType>
Queue<ItemType>::Queue(const Queue<ItemType>& aQueue) : backPtr(NULL)
{
	if (!aQueue.empty()) //Copying a circular linked list if the other Queue isn't empty
	{
		backPtr = new Node<ItemType>(aQueue.backPtr->getItem());
		backPtr->setNext(backPtr);
		Node<ItemType>* otherCurrent = aQueue.backPtr->getNext();
		Node<ItemType>* trailCurrent = backPtr;
		while (otherCurrent != aQueue.backPtr)
		{
			Node<ItemType>* newNode = new Node<ItemType>(otherCurrent->getItem());
			trailCurrent->setNext(newNode);
			trailCurrent = newNode;
			otherCurrent = otherCurrent->getNext();
		}
		trailCurrent->setNext(backPtr);
	}
}

template <class ItemType>
Queue<ItemType>::~Queue()
{
	if (!empty()) //Delete the contents of a circular linked list. Note that all cases are handled
	{
		Node<ItemType>* current = backPtr->getNext();
		while (current != backPtr)
		{
			Node<ItemType>* storeNext = current->getNext();
			delete current;
			current = storeNext;
		}
		delete current;
		backPtr = NULL;
	}
}

template <class ItemType>
bool Queue<ItemType>::empty() const
{
	return (!backPtr);
}

template <class ItemType>
bool Queue<ItemType>::push(const ItemType& newEntry)
{
	Node<ItemType>* newNode = new Node<ItemType>(newEntry);
	if (!empty()) //Insert into a non-empty circular list.
	{
		newNode->setNext(backPtr->getNext());
		backPtr->setNext(newNode);
		backPtr = newNode;
	}
	else //Queue is empty, so we create a new head node
	{
		backPtr = newNode;
		backPtr->setNext(backPtr);
	}

	return true;
}

template <class ItemType>
bool Queue<ItemType>::pop()
{
	bool ableToRemove = (!empty());
	if (ableToRemove) //Queue isn't empty
	{
		Node<ItemType>* storeNext = backPtr->getNext();
		if (storeNext == backPtr) //Single node case
			backPtr = NULL;
		else //More than one node
			backPtr->setNext(storeNext->getNext());
		delete storeNext;
	}
}

template <class ItemType>
ItemType Queue<ItemType>::front() const
{
	assert(!empty()); //Assert that the queue isn't empty.
	return ((backPtr->getNext())->getItem()); //Return the front of the queue.
}

#endif
