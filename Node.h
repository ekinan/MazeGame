#ifndef _NODE_H
#define _NODE_H

/*
Generic node class used to implement the Queue and Stack classes for the project
*/

#include <cstddef>

template <class ItemType>
class Node
{
public:
	//Constructors
	Node();
	Node(const ItemType& anItem);
	Node(const ItemType& anItem, Node<ItemType>* nextNodePtr);

	void setItem(const ItemType& anItem); //Sets item to whatever anItem is
	void setNext(Node<ItemType>* nextNodePtr); //Sets next to whatever nextNodePtr is
	ItemType getItem() const; //Returns the value of item
	Node<ItemType>* getNext() const; //Returns the value of next

private:
	ItemType item; //Variable to store the item of the node
	Node<ItemType>* next; //Variable to store the next pointer of the node
};

#include "Node.cpp"
#endif



