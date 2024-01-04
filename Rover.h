#ifndef ROVER_H
#define ROVER_H
#include "Definitions.h"
struct roverInfo
{
	int speed;
	int numMissionsBeforeCheckUp;
	int checkUpDuration;
};

class Rover
{
private:
	TYPE type;
	bool available;
	roverInfo info;
	//The numbers in the last 3 variables are automatically sat given the rover's type//
	int missionsCompleted;
	int checkupday; // the day which the rover is assigned to the checkup queue
public:
	Rover() {}
	Rover(TYPE t, roverInfo i);
	void setType(TYPE t);
	void setInfo(roverInfo i);
	void setAvailable(bool a);
	void setMissionsCompleted(int m);
	TYPE getType();
	roverInfo getInfo();
	bool getAvailable();
	int getMissionsCompleted();
	void Setcheckupday(int d);
	int getcheckupday();


};

#endif