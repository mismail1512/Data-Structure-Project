#include "Events.h"

int Events::getED()
{
	return ED;
}

int Events::getID()
{
	return ID;
}

Events::Events(int ED1, int ID1)
{
	ED = ED1;
	ID = ID1;
}
