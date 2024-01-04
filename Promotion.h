#pragma once
#include "Events.h"
#include"Node.h"
#include"Mission.h"
//#include"Station.h"
#include"Events.h"
//template <typename T>
class Promotion : public Events
{
	int	ED;     // is the event day.
	int	ID;     // is the ID of the mission to be promoted to emergency.This ID must be of a mountainous mission.
	static int myAuto;
public:
	Promotion() {}
	Promotion(int ED1, int ID1);
//	void Promot(Promotion* Emergency);
	static void SetAutoP(int Auto1);
	static int GetAutoP();
};
