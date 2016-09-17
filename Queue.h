#ifndef _QUEUE_H
#define _QUEUE_H

//Queue class, implemented as a circular linked list.
//Has the standard functions of a typical queue.

#include "Node.h"

template <class ItemType>
class Queue
{
private:
	Node<ItemType>* backPtr; //Pointer to the back of the queue.

public:
	//Constructors and Destructor
	Queue();
	Queue(const Queue<ItemType>& aQueue);
	~Queue();

	bool empty() const; //Checks if the queue is empty
	bool push(const ItemType& newEntry); //Pushes newEntry to the end of the queue
	bool pop(); //Removes the item at the front of the queue

	ItemType front() const; //Returns the item at the front of the queue
};

#include "Queue.cpp"

#endif
