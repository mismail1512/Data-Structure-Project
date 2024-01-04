#include "Rover.h"
Rover::Rover(TYPE t, roverInfo i)
{
	type = t;
	info = i;
}
void Rover::setType(TYPE t)
{
	type = t;
}
void Rover::setInfo(roverInfo i)
{
	info = i;
}
void Rover::setAvailable(bool a)
{
	available = a;
}
void Rover::setMissionsCompleted(int m)
{
	missionsCompleted = m;
}
TYPE Rover::getType()
{
	return type;
}
roverInfo Rover::getInfo()
{
	return info;
}
bool Rover::getAvailable()
{
	return available;
}
int Rover::getMissionsCompleted()
{
	return missionsCompleted;
}

void Rover::Setcheckupday(int d)
{
	checkupday = d;
}

int Rover::getcheckupday()
{
	return checkupday;
}
