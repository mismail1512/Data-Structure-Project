#ifndef FORMULATION
#define FORMULATION
#include "Events.h"
class Formulation :
	public Events
{
private:

	double TLOC; // TLOC is the mission�s target location(in kilometers from the base station).
	int MDUR;     // MDUR is the number of days needed to fulfill the mission requirements at target location.
	int	SIG;      //SIG is the mission�s significance
	TYPE type;
public:
	Formulation() {}
	Formulation(TYPE TYP1, int ED1, int ID1, double	TLOC1, int MDUR1, int SIG1);
	double getTLOC();
	int getMDUR();
	int getSIG();
	TYPE getType();


};

#endif