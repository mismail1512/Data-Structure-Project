#ifndef STATION_H
#define STATION_H
#include"Definitions.h"
#include"Formulation.h"
#include"Cancellation.h"
#include"Promotion.h"
#include"LinkedQueue.h"
#include"LinkedList.h"
#include"Node.h"
#include"Rover.h"
#include"Mission.h"
#include"Events.h"
#include<iostream>
class Station
{
private:

    int countWaitingMountMissions;
    int countWaitingPolarMissions;
    int countWaitingEmrgMissions;
    int countWaitingsearchingMissions;
    int countWaitingScientificExperimentsMissions;

    int countInServMountMissions;
    int countInServPolarMissions;
    int countInServEmrgMissions;
    int countInServsearchingMissions;
    int countInServScientificExperimentsMissions;

    int countDoneMountMissions;
    int countDonePolarMissions;
    int countDoneEmrgMissions;
    int countDonesearchingMissions;
    int countDoneScientificExperimentsMissions;

    int countMountRovers;
    int countPolarRovers;
    int countEmergencytRovers;
    int countSearchRovers;
    int countScientificExperimentstRovers;

    int TotalRovers;
    int countFormulationEvent;
    int countCancellationEvent;
    int countPromotionEvent;
    int countAutoProm;
    //static int countpromotionwaitingdays;
    int countAvailableEmergencyRovers;
    int countAvailableMountRovers;
    int countAvailablePolarRovers;
    int countAvailablesearchingRovers;
    int countAvailableScientificExperimentsRovers;

    int countInServiceEmergencyRovers;
    int countInServiceMountRovers;
    int countInServicePolarRovers;
    int countInServicesearchingRovers;
    int countInServiceScientificExperimentsRovers;

    int countCheckupEmergencyRovers;
    int countCheckupMountRovers;
    int countCheckupPolarRovers;
    int countCheckupsearchingRovers;

    int countCheckupScientificExperimentsRovers;

    int countCheckupRovers;



    LinkedQueue<Formulation>* FormEvents = new LinkedQueue<Formulation>;
    LinkedQueue<Cancellation>* CancelEvents = new LinkedQueue<Cancellation>;
    LinkedQueue<Promotion>* PromEvents = new LinkedQueue<Promotion>;
    LinkedList<Mission>* WaitingMountMissions = new LinkedList<Mission>;
    LinkedList<Mission>* WaitingPolarMissions = new LinkedList<Mission>;
    LinkedList<Mission>* WaitingEmrgMissions = new LinkedList<Mission>;
    LinkedList<Mission>* WaitingsearchingMissions = new LinkedList<Mission>;
    LinkedList<Mission>* WaitingScientificExperimentsMissions = new LinkedList<Mission>;
    LinkedList<Mission>* InServMountMissions = new LinkedList<Mission>;
    LinkedList<Mission>* InServPolarMissions = new LinkedList<Mission>;
    LinkedList<Mission>* InServEmrgMissions = new LinkedList<Mission>;
    LinkedList<Mission>* InServsearchingMissions = new LinkedList<Mission>;
    LinkedList<Mission>* InServScientificExperimentsMissions = new LinkedList<Mission>;

    LinkedList<Mission>* DoneMountMissions = new LinkedList<Mission>;
    LinkedList<Mission>* DonePolarMissions = new LinkedList<Mission>;
    LinkedList<Mission>* DoneEmrgMissions = new LinkedList<Mission>;
    LinkedList<Mission>* DonesearchingMissions = new LinkedList<Mission>;
    LinkedList<Mission>* DoneScientificExperimentsMissions = new LinkedList<Mission>;

    LinkedList<Rover>* PRovers = new LinkedList<Rover>;
    LinkedList<Rover>* MRovers = new LinkedList <Rover>;
    LinkedList<Rover>* ERovers = new LinkedList<Rover>;
    LinkedList<Rover>* SRovers = new LinkedList<Rover>;
    LinkedList<Rover>* YRovers = new LinkedList<Rover>;

    LinkedList<Rover>* InServicePRovers = new LinkedList<Rover>;
    LinkedList<Rover>* InServiceERovers = new LinkedList<Rover>;
    LinkedList<Rover>* InServiceMRovers = new LinkedList<Rover>;
    LinkedList<Rover>* InServiceSRovers = new LinkedList<Rover>;
    LinkedList<Rover>* InServiceYRovers = new LinkedList<Rover>;

    LinkedQueue<Mission>* AllCompletedMissions = new LinkedQueue<Mission>;
    LinkedQueue<Rover>* EmergencyCheckupRovers = new LinkedQueue<Rover>;
    LinkedQueue<Rover>* MountCheckupRovers = new LinkedQueue<Rover>;
    LinkedQueue<Rover>* PolarCheckupRovers = new LinkedQueue<Rover>;
    LinkedQueue<Rover>* searchingCheckupRovers = new LinkedQueue<Rover>;
    LinkedQueue<Rover>* ScientificExperimentsCheckupRovers = new LinkedQueue<Rover>;


    // ScientificExperiments     searching


public:

    void fileLoader();
    void simpleSimulator();
    void write_Output();

    void stepByStepSimulator();
    void silentSimulator();



};

#endif