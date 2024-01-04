#ifndef NODE_H
#define NODE_H
#include <iostream>
#include"Mission.h"
using namespace std;
template < typename T>
class Node
{
private:
	T item; // A data item
	Node<T>* next; // Pointer to next node
	//int WaitingMountPromotionDays;
public:
	int priority;
	Node()
	{
		next = nullptr;
	}
    Node(const T& data)
    {
        item = data;
        next = nullptr;
        //emergencypriority()
        //Mission::emergencypriority();

    }
	Node(const T& data, int priorit)
	{
		item = data;
		next = nullptr;
		//emergencypriority()
		priority = priorit;
			//Mission::emergencypriority();
		
	}
	T getItem() const
	{
		return item;
	}
	Node<T>* getNext() const
	{
		return next;
	}
	void setItem(const T& data)
	{
		item = data;
	}
	void setNext(Node<T>* nextNodePtr)
	{
		next = nextNodePtr;
	}
	int getpriority()
	{
		return priority;
	}
	void setpriority(const int& priorit)
	{
		priority = priorit;

	}
	/*int detWaitingMountPromotionDays() {
		return WaitingMountPromotionDays;
	}*/

};

#endif