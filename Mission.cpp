#include "Mission.h"
Mission::Mission(TYPE t, int id, int dist, int sig, int fday, int duration, int ED1)
{
    ED = ED1;
	type = t;
	ID = id;
	Distance = dist;
	Significance = sig;
	FormDay = fday;
	durationAtLocation = duration;
}
void Mission::setType(TYPE t)
{
	type = t;
}
void Mission::setID(int id)
{
	ID = id;
}
void Mission::setStatus(MISSION_STATUS s)
{
	status = s;
}
void Mission::setDistance(int d)
{
	Distance = d;
}
void Mission::setSignificance(int s)
{
	Significance = s;
}

void Mission::setFormDay(int d)
{
	FormDay = d;
}

void Mission::setWaitDays(int d)
{
	WaitDays = d;
}

void Mission::setExecDays(int d)
{
	ExecDays = d;
}


void Mission::setFinishDay(int d)
{
	FinishDay = d;
}

void Mission::setDurationAtLocation(int d)
{
	durationAtLocation = d;
}

TYPE Mission::getType()
{
	return type;
}

MISSION_STATUS Mission::getStatus()
{
	return status;
}

int Mission::getID()
{
	return ID;
}

int Mission::getDistance()
{
	return Distance;
}

int Mission::getSignificance()
{
	return Significance;
}

int Mission::getFormDay()
{
	return FormDay;
}
int Mission::getWaitDays()
{
	return WaitDays;
}
int Mission::getExecDays()
{
	return ExecDays;
}
int Mission::getFinishDay()
{
	return FinishDay;
}
int Mission::getDurationAtLocation()
{
	return durationAtLocation;
}
int Mission::getED()
{
    return ED;
}



/*int Mission::getEmergencyPriority()
{
	int x = (getDistance()+getSignificance()+getExecDays()+getFormDay());
	return x;
}*/
