#ifndef __EVENTS_H_
#define __EVENTS_H_
#include"Definitions.h"
#include<iostream>
using namespace std;
class Events
{
private:
	//	TYPE type; // (Letter ) means a event type.
	int	ED;   // ED is the event day
	int ID;   // ID is the ID of the mission 

public:
	Events() {}
	Events(int ED1, int ID1);

	int getED();
	int getID();



};

#endif