#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Node.h"

#include<iostream>
using namespace std;
template <typename T>
class LinkedList
{
private:
	Node<T>* Head;
	int count;
	
public:

	LinkedList()
	{
		count = 0;
		Head = nullptr;
	}
	void setHead(Node<T>* k)
	{
		Head = k;
	}
	void setCount(int n)
	{
		count = n;
	}
	void PrintList()	const
	{
		cout << "\nList has " << count << " nodes";
		cout << "\nPrinting list contents:\n\n";
		Node<T>* p = Head;

		while (p)
		{
			cout << "[ " << p->getItem() << " ]";
			cout << "--->";
			p = p->getNext();
		}
		cout << "*\n";
	}
	bool isEmpty() {
		if (Head == nullptr) {
			return true;
		}
		else {
			return false;
		}
	}
	void InsertBeg(const T& data)
	{
		if (count == 0)
		{
			Node<T>* R = new Node<T>(data);
			Head = R;
			count++;
		}
		else
		{
			Node<T>* R = new Node<T>(data);
			R->setNext(Head);
			Head = R;
			count++;
		}
	}
	void DeleteAll()
	{

		Node<T>* P = nullptr;
		while (P)
		{
			P = this->getHead()->getNext();
			delete P;
			setHead(P);
		}
		count = 0;
		Head = nullptr;

	}

	~LinkedList()
	{
		DeleteAll();
	}

	void InsertEnd(const T& data) {

		if (Head == nullptr) {
			InsertBeg(data);
			return;
		}
		else {
			Node<T>* p = Head;
			while (p->getNext()) {
				p = p->getNext();
			}
			Node<T>* R = new Node<T>(data);
			p->setNext(R);
			count++;
			return;
		}
	}
	bool Find(T Key) {
		Node<T>* p = Head;
		while (p) {
			if (p->getItem() == Key) {
				cout << "Found the element " << Key << endl;
				return true;
			}
			p = p->getNext();
		}
		cout << "\nElement " << Key << " not found" << endl;
		return false;
	}

	int CountOccurance(const T& value)
	{
		Node<T>* p;
		p = Head;
		int occurance = 0;
		while (p)
		{
			if (p->getItem() == value)
			{
				occurance++;
			}
			p = p->getNext();
		}
		return occurance;
	}

	void DeleteFirst()
	{
		if (Head == nullptr)
		{
			exit(0);
		}

		Head = Head->getNext();
		count--;
	}

	void DeleteLast()
	{
		Node<T>* p;
		p = Head;
		if (Head == nullptr)
		{
			cout << " List is empty" << endl;
		}
		else if (Head->getNext() == nullptr)
		{
			Head = nullptr;
			count--;
		}
		else
		{
			while ((p->getNext())->getNext())
			{
				p = p->getNext();
			}
			p->setNext(nullptr);
			count--;
		}


	}

	bool DeleteNode(const T& value)
	{
		Node<T>* p;
		p = Head;
		bool deleted = 0;
		if (Head == nullptr)
		{
			cout << "List is empty" << endl;
			return deleted;
		}
		else if (Head->getItem() == value)
		{
			DeleteFirst();
			deleted = 1;
			return deleted;
		}
		for (int i = 0; i < count - 1; i++)
		{
			if ((p->getNext())->getItem() == value)
			{
				Node<T>* d = p->getNext();
				p->setNext((p->getNext())->getNext());
				//delete d;				
				deleted = 1;
				count--;
				break;

			}
			p = p->getNext();
		}
		return deleted;
	}

	bool DeleteNodes(const T& value)
	{
		if (Find(value))
		{
			while (Find(value) != 0)
			{
				DeleteNode(value);
			}
			return 1;
		}
		else { return 0; }
	}

	Node<T>* getHead() const
	{
		return Head;
	}

	int getCount() const { return count; }

	void Merge(const LinkedList& L)
	{
		Node<T>* p;
		p = Head;
		while (p->getNext())
		{
			p = p->getNext();
		}

		p->setNext(L.getHead());

		int count2 = 0;
		Node<T>* p2;
		p2 = L.Head;
		while (p2 != 0)
		{
			count2++;
			p2 = p2->getNext();
		}
		count = count + count2;
	}
	void Reverse()
	{
		Node<T>* p = Head;
		Node<T>* before = nullptr;
		Node<T>* after = nullptr;

		while (p != nullptr)
		{
			after = p->getNext();
			p->setNext(before);
			before = p;
			p = after;
		}
		Head = before;
	}

	void printKth(int k)
	{
		if (k > count)
		{
			cout << "Number is bigger than number of elements" << endl;

		}
		else if (k == 1)
		{
			cout << Head->getItem();
		}
		else
		{
			Node<T>* p;
			p = Head;
			for (int i = 0; i < (k - 1); i++)
			{
				p = p->getNext();

			}
			cout << p->getItem();
		}
	}


	Node<T>* removeMin()
	{
		Node<T>* p = Head;
		Node<T>* min = Head;
		for (int i = 0; i < count; i++)
		{
			if (p->getItem() < min->getItem())
			{
				min = p;
			}
			p = p->getNext();
		}
		if (min == Head)
		{
			DeleteFirst();
		}
		else
		{
			p = Head;
			while (p->getNext() != min)
			{
				p = p->getNext();
			}

			p->setNext((p->getNext())->getNext());
			count--;
		}
		return min;
	}

	LinkedList cloneList()
	{
		LinkedList<T>* newListP = new LinkedList<T>();
		Node<T>* h;
		h = new Node<T>(Head->getItem());
		newListP->setHead(h);
		newListP->setCount(1);

		Node <T>* p = Head->getNext();

		while (p)
		{
			Node<T>* n = new Node<T>(p->getItem());
			h->setNext(n);

			h = h->getNext();
			p = p->getNext();

			newListP->setCount(newListP->getCount() + 1);

		}

		return *newListP;
	}
	
	int largestPriority( )
	{
		
		int max = -1;
		Node<T>* p = Head;
		while (p != NULL) {

			if (max < p->priority)
				max = p->priority;
			p = p->getNext();
		}
		return max;
	}
	Node<T>* FindMaxPrioritNode()
	{
		int max = -1;
		Node<T>* p = Head;
		while (p != NULL) {


			if (max < p->priority)
				max = p->priority;
				p = p->getNext();
		}
		
		Node<T>* p1 = Head;
	  

		while (p1) {
			if (p1->getpriority() == max)
			{
				cout << "Found the element " << max << endl;
				return p1;
			}
			p1 = p1->getNext();
		}
		cout << "\nElement " << max << " not found" << endl;
		return nullptr;
	}
};
#endif