#pragma once
#include "Events.h"
class Cancellation : public Events
{
public:
	Cancellation() {}
	Cancellation(int ED1, int ID1);
};
