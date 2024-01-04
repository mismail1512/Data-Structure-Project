#include "Promotion.h"
 int Promotion:: myAuto = Promotion::GetAutoP();
Promotion::Promotion(int ED1, int ID1) :Events(ED1, ID1)
{
}
void Promotion::SetAutoP(int Auto1)
{
    myAuto = Auto1;
}
int Promotion::GetAutoP()
{
    return myAuto;
}
/*void Promotion::Promot(Promotion* prom)
{

    WaitingEmrgMissions->InsertEnd(Mission(E, Pr.getID(), p->getItem().getDistance(), p->getItem().getSignificance(), day, p->getItem().getDurationAtLocation()));
    countWaitingEmrgMissions++;
    //  Pr.Promot();/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     // promot
    WaitingMountMissions->DeleteNode(p->getItem());
    countWaitingMountMissions--;
	//WaitingEmrgMissions->InsertEnd(Mission(E, prom.getID(), prom.getTLOC(), prom.getSIG(), day, prom.getMDUR()));
	//countWaitingEmrgMissions++;
}*/
