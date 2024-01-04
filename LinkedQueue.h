#ifndef LINKEDQUEUE_H
#define LINKEDQUEUE_H
#include "Node.h"
#include <iostream>
using namespace std;
template <typename T>
class LinkedQueue
{
private:
	Node<T>* backPtr;
	Node<T>* frontPtr;
public:
	LinkedQueue()
	{
		backPtr = nullptr;
		frontPtr = nullptr;
	}
	bool isEmpty() const
	{
		return (frontPtr == nullptr);
	}
	bool enqueueMission(const T& newEntry, const int & newProp)
	{
		Node<T>* newNodePtr = new Node<T>(newEntry, newProp);
		if (isEmpty())
			frontPtr = newNodePtr;
		else
			backPtr->setNext(newNodePtr);

		backPtr = newNodePtr;
		//priority = newProp;
		newNodePtr->setpriority(newProp);
		return true;
	}
	bool enqueue(const T& newEntry)
	{
		Node<T>* newNodePtr = new Node<T>(newEntry);
		if (isEmpty())
			frontPtr = newNodePtr;
		else
			backPtr->setNext(newNodePtr);

		backPtr = newNodePtr;
		
		return true;
	}
	bool dequeue(T& frntEntry)
	{
		if (isEmpty())
			return false;

		Node<T>* nodeToDeletePtr = frontPtr;
		frntEntry = frontPtr->getItem();
		frontPtr = frontPtr->getNext();
		if (nodeToDeletePtr == backPtr)
			backPtr = nullptr;
		delete nodeToDeletePtr;
		return true;
	}
	bool peek(T& frntEntry)  const
	{
		if (isEmpty())
			return false;

		frntEntry = frontPtr->getItem();
		return true;
	}
	~LinkedQueue()
	{
		T temp;
		while (dequeue(temp));
	}
};

#endif