#ifndef _NODE_CPP
#define _NODE_CPP

/*
Implementation file for Node.h, containing the definitions of the member functions.
*/

#include "Node.h"

template <class ItemType>
Node<ItemType>::Node() : next(NULL)
{
}

template <class ItemType>
Node<ItemType>::Node(const ItemType& anItem) : item(anItem), next(NULL)
{
}

template <class ItemType>
Node<ItemType>::Node(const ItemType& anItem, Node<ItemType>* nextNodePtr) :
		item(anItem), next(nextNodePtr)
{
}

template <class ItemType>
void Node<ItemType>::setItem(const ItemType& anItem)
{
	item = anItem;
}

template <class ItemType>
void Node<ItemType>::setNext(Node<ItemType>* nextNodePtr)
{
	next = nextNodePtr;
}


template <class ItemType>
ItemType Node<ItemType>::getItem() const
{
	return item;
}

template <class ItemType>
Node<ItemType>* Node<ItemType>::getNext() const
{
	return next;
}

#endif
