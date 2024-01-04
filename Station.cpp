#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
//This is for a cross-platform sleep

#include "Station.h"
#include <fstream>
#include <iostream>
#include <string>
#include"LinkedList.h"

using namespace std;

void Station::fileLoader()
{
    string filename;
    cout << "Specify Input File Name (testinput1.txt,testinput2.txt,testinput3.txt) ";
    cin >> filename;

    ifstream input(filename.c_str());
    /*
   if (input.fail())
    {
        cout << filename << " was not found!";
       exit(0);
    }
   */


    int numM, numP, numE, SM, SP, SE, N, CM, CP, CE, AutoP, NumEvents;

    input >> numM >> numP >> numE >> SM >> SP >> SE >> N >> CM >> CP >> CE >> AutoP >> NumEvents;


    //Now a sanity check! Call it a unit test if you wanna be fancy :)
    //cout << numM << " " << numP << " " << numE << " " << SM << " " << SP << " " << SE << " " << N << " " << CM << " " << CP << " " << CE << " " << AutoP << " " << NumEvents << "\n";

    //Coming up is parsing the multiple lines of events information.
    //I'll do those before constructing the rovers and handling the constructor calls for all the other stuff I read in.
    //Really only because I want to close the file a little sooner.

    //We'll add more temporary variables, just so the naming looks neat and consistent.
    char EventType, CharRoverType;
    TYPE TYP;
    double TLOC;
    int ED, ID, MDUR, SIG;


    //Now a loop over Events
    for (int i = 0; i < NumEvents; i++)
    {
        input >> EventType;
        if (EventType == 'F')
        {
            input >> CharRoverType >> ED >> ID >> TLOC >> MDUR >> SIG;
            switch (CharRoverType)
            {
                case 'M':
                    TYP = M;
                    break;
                case 'P':
                    TYP = P;
                    break;
                case 'E':
                    TYP = E;
                    break;
                case 'S':
                    TYP = S;
                    break;
                case 'Y':
                    TYP = Y;
                    break;
            }
            //Another sanity check:
//            cout << TYP << " " << ED  << " " << ID << " " << TLOC  << " " << MDUR << " " << SIG << "\n";
            //Now the core logic:

            FormEvents->enqueueMission(Formulation(TYP, ED, ID, MDUR, SIG, TLOC), SIG); //Anonymous object getting passed.
            countFormulationEvent++;
        }
        if (EventType == 'X')
        {
            input >> ED >> ID;
            //Sanity check:
            //cout << ED << " " << ID << "\n";
            //Now the core logic:
            CancelEvents->enqueue(Cancellation(ED, ID));
            countCancellationEvent++;

        }
        if (EventType == 'P')
        {
            input >> ED >> ID;
            //Sanity check:
            //cout << ED << " " << ID << "\n";
            //Now the core logic:
            PromEvents->enqueue(Promotion(ED, ID));
            countPromotionEvent++;
            Promotion::SetAutoP(AutoP);
        }
    }
    input.close();

    //Let's set up how the rover info struct should look for each type of rover
    struct roverInfo mountainousStruct = { SM, N, CM };
    struct roverInfo polarStruct = { SP, N, CP };
    struct roverInfo emergencyStruct = { SE, N, CE };

    //Now a loop to create the mountainous rovers.
    for (int i = 0; i < numM; i++)
    {
        MRovers->InsertEnd(Rover(M, mountainousStruct));
        countMountRovers++;
        TotalRovers++;
    }
    //Now a loop to create the polar rovers.
    for (int i = 0; i < numP; i++)
    {
        PRovers->InsertEnd(Rover(P, polarStruct));
        countPolarRovers++;
        TotalRovers++;
    }
    //Now a loop to create the polar rovers.
    for (int i = 0; i < numE; i++)
    {
        ERovers->InsertEnd(Rover(E, emergencyStruct));
        countEmergencytRovers++;
        TotalRovers++;
    }
}

void Station::simpleSimulator()
{
    countWaitingMountMissions = 0;
    countWaitingPolarMissions = 0;
    countWaitingEmrgMissions = 0;
    countInServMountMissions = 0;
    countInServPolarMissions = 0;
    countInServEmrgMissions = 0;
    countDoneMountMissions = 0;
    countDonePolarMissions = 0;
    countDoneEmrgMissions = 0;
    countMountRovers = 0;
    countPolarRovers = 0;
    countEmergencytRovers = 0;
    TotalRovers = 0;
    countFormulationEvent = 0;
    countCancellationEvent = 0;
    countPromotionEvent = 0;
    countAvailableEmergencyRovers = 0;
    countAvailableMountRovers = 0;
    countAvailablePolarRovers = 0;
    countInServiceEmergencyRovers = 0;
    countInServiceMountRovers = 0;
    countInServicePolarRovers = 0;
    countAutoProm = 0;

    countWaitingsearchingMissions=0;
    countWaitingScientificExperimentsMissions=0;
    countInServsearchingMissions=0;
    countInServScientificExperimentsMissions=0;
    countDonesearchingMissions=0;
    countDoneScientificExperimentsMissions=0;
    countSearchRovers=0;
    countScientificExperimentstRovers=0;
    countAvailablesearchingRovers=0;
    countAvailableScientificExperimentsRovers=0;
    countInServicesearchingRovers=0;
    countInServiceScientificExperimentsRovers=0;
    countCheckupsearchingRovers=0;

    int countCheckupScientificExperimentsRovers;

    fileLoader();
    cout << "# Of Rovers of Each Type (M P E) : " << countMountRovers << " " << countPolarRovers << " " << countEmergencytRovers << endl;
    cout << "# Formulation Events " << countFormulationEvent << " # Cancellation Events " << countCancellationEvent << " # Promotion Events " << countPromotionEvent << endl;

    int day = 0;
    int userInput;
    cout << "Enter 1 to increment days." << "\n  ";
    cin >> userInput;

    while (userInput == 1)
    {
        if (WaitingEmrgMissions->isEmpty() && WaitingPolarMissions->isEmpty() && WaitingMountMissions->isEmpty() && InServEmrgMissions->isEmpty() && InServPolarMissions->isEmpty() && InServMountMissions->isEmpty() && FormEvents->isEmpty() && CancelEvents->isEmpty())
        {
            cout << "All Done! No Active Missions or Events (Except For Promotion events Which we ignored this phase)" << endl;
            break;
        }
        day++;
        cout << "Today is Day " << day << endl;
        Formulation F;
        Cancellation C;
        Promotion Pr;
        Node<Mission>* p = nullptr;
        for (int i = 0; i < countFormulationEvent; i++)
        {

            FormEvents->peek(F);
            if (F.getED() == day && FormEvents->peek(F))
            {
                //cout << F.getED() << " " << day << endl;
                FormEvents->dequeue(F);
                switch (F.getType())
                {
                    case M: WaitingMountMissions->InsertEnd(Mission(M, F.getID(), F.getTLOC(), F.getSIG(), day, F.getMDUR(), F.getED()));
                        countWaitingMountMissions++;


                        break;
                    case P: WaitingPolarMissions->InsertEnd(Mission(P, F.getID(), F.getTLOC(), F.getSIG(), day, F.getMDUR(), F.getED()));
                        countWaitingPolarMissions++;
                        // countpromotionwaitingdays++;
                        break;
                    case E:
                        WaitingEmrgMissions->InsertEnd(Mission(E, F.getID(), F.getTLOC(), F.getSIG(), day, F.getMDUR(), F.getED()));
                        countWaitingEmrgMissions++;
                        break;
                    case S:
                        WaitingsearchingMissions->InsertEnd(Mission(E, F.getID(), F.getTLOC(), F.getSIG(), day, F.getMDUR(), F.getED()));
                        countWaitingsearchingMissions++;
                        break;
                    case Y:
                        WaitingScientificExperimentsMissions->InsertEnd(Mission(E, F.getID(), F.getTLOC(), F.getSIG(), day, F.getMDUR(), F.getED()));
                        countWaitingScientificExperimentsMissions++;
                        break;

                }

            }
            else
            {
                break;
            }
        }


        for (int i = 0; i < countCancellationEvent; i++)
        {
            CancelEvents->peek(C);
            p = WaitingMountMissions->getHead();
            if (C.getED() == day && CancelEvents->peek(C))
            {
                CancelEvents->dequeue(C);
                while (p)
                {
                    if (C.getID() == p->getItem().getID())
                    {
                        WaitingMountMissions->DeleteNode(p->getItem());
                        countWaitingMountMissions--;
                        break;
                    }
                    p = p->getNext();
                }
            }
            else
            {
                break;
            }
        }

        for (int i = 0; i < countPromotionEvent; i++)
        {
            PromEvents->peek(Pr);
            p = WaitingMountMissions->getHead();
            if (Pr.getED() == day && PromEvents->peek(Pr))
            {

                while (p)
                {
                    if (Pr.getID() == p->getItem().getID() && day - p->getItem().getFormDay() == Promotion::GetAutoP())
                    {
                        PromEvents->dequeue(Pr);

                        WaitingEmrgMissions->InsertEnd(Mission(E, Pr.getID(), p->getItem().getDistance(), p->getItem().getSignificance(), day, p->getItem().getDurationAtLocation(), F.getED()));
                        countWaitingEmrgMissions++;
                        WaitingMountMissions->DeleteNode(p->getItem());
                        countWaitingMountMissions--;
                        countAutoProm++;
                        break;
                    }
                    p = p->getNext();
                }
            }
            else
            {
                break;
            }
        }

        if (!WaitingEmrgMissions->isEmpty() && (!PRovers->isEmpty() || !MRovers->isEmpty() || !ERovers->isEmpty()))
        {
            Mission eMp = WaitingEmrgMissions->FindMaxPrioritNode()->getItem();
            if (eMp.getED() == day)
            {
                InServEmrgMissions->InsertEnd(eMp);
                WaitingEmrgMissions->DeleteNode(WaitingEmrgMissions->FindMaxPrioritNode()->getItem());
                countWaitingEmrgMissions--;
                countInServEmrgMissions++;

                ////////////////////////////////////////////////////////////////////////////////////////////////

                if (!ERovers->isEmpty())
                {
                    Rover R = ERovers->getHead()->getItem();
                    InServiceERovers->InsertEnd(R);
                    ERovers->DeleteFirst();
                    countAvailableEmergencyRovers--;
                    countInServiceEmergencyRovers++;
                }
                else if (!MRovers->isEmpty())
                {

                    Rover R = MRovers->getHead()->getItem();
                    InServiceMRovers->InsertEnd(R);
                    MRovers->DeleteFirst();
                    countAvailableMountRovers--;
                    countInServiceEmergencyRovers++;
                }
                else if (!PRovers->isEmpty())
                {
                    Rover R = PRovers->getHead()->getItem();
                    InServicePRovers->InsertEnd(R);
                    PRovers->DeleteFirst();
                    countAvailablePolarRovers--;
                    countInServiceEmergencyRovers++;
                }
            }
        }

        if (!WaitingMountMissions->isEmpty() && (!MRovers->isEmpty() || !ERovers->isEmpty()))
        {
            Mission mMp = WaitingMountMissions->getHead()->getItem();
            if (mMp.getED() == day)
            {
                InServMountMissions->InsertEnd(mMp);
                WaitingMountMissions->DeleteFirst();
                countWaitingMountMissions--;
                countInServMountMissions++;

                if (!MRovers->isEmpty())
                {
                    Rover R = MRovers->getHead()->getItem();
                    InServiceMRovers->InsertEnd(R);
                    MRovers->DeleteFirst();
                    countAvailableMountRovers--;
                    countInServiceMountRovers++;
                }
                else if (!ERovers->isEmpty())
                {
                    Rover R = ERovers->getHead()->getItem();
                    InServiceERovers->InsertEnd(R);
                    ERovers->DeleteFirst();
                    countAvailableEmergencyRovers--;
                    countInServiceMountRovers++;
                }
            }
        }
        if (!WaitingPolarMissions->isEmpty() && (!PRovers->isEmpty()))
        {
            Mission pMp = WaitingPolarMissions->getHead()->getItem();
            if (pMp.getED() == day)
            {
                InServPolarMissions->InsertEnd(pMp);
                WaitingPolarMissions->DeleteFirst();
                countWaitingPolarMissions--;
                countInServPolarMissions++;

                Rover R = PRovers->getHead()->getItem();
                InServicePRovers->InsertEnd(R);
                PRovers->DeleteFirst();
                countAvailablePolarRovers--;
                countInServicePolarRovers++;
            }
        }

        if (!WaitingsearchingMissions->isEmpty() && (!SRovers->isEmpty()))
        {
            Mission SMp = WaitingPolarMissions->getHead()->getItem();
            if (SMp.getED() == day)
            {
                InServsearchingMissions->InsertEnd(SMp);
                WaitingsearchingMissions->DeleteFirst();
                countWaitingsearchingMissions--;
                countInServsearchingMissions++;

                Rover R = SRovers->getHead()->getItem();
                InServiceSRovers->InsertEnd(R);
                SRovers->DeleteFirst();
                countAvailablesearchingRovers--;
                countInServicesearchingRovers++;
            }
        }

        if (!WaitingScientificExperimentsMissions->isEmpty() && (!YRovers->isEmpty()))
        {
            Mission YMp = WaitingPolarMissions->getHead()->getItem();
            if (YMp.getED() == day)
            {
                InServScientificExperimentsMissions->InsertEnd(YMp);
                WaitingScientificExperimentsMissions->DeleteFirst();
                countWaitingScientificExperimentsMissions--;
                countInServScientificExperimentsMissions++;

                Rover R = YRovers->getHead()->getItem();
                InServiceYRovers->InsertEnd(R);
                YRovers->DeleteFirst();
                countAvailableScientificExperimentsRovers--;
                countInServiceScientificExperimentsRovers++;
            }
        }



        if (!InServMountMissions->isEmpty())
        {
            Mission mMp = InServMountMissions->getHead()->getItem();
            if (mMp.getExecDays() == day - mMp.getED())
            {
                DoneMountMissions->InsertEnd(mMp);
                AllCompletedMissions->enqueueMission(mMp, mMp.getSignificance());
                InServMountMissions->DeleteFirst();
                countInServMountMissions--;
                countDoneMountMissions++;
                Rover R = InServiceMRovers->getHead()->getItem();
                if (R.getMissionsCompleted() == R.getInfo().numMissionsBeforeCheckUp)
                {
                    MountCheckupRovers->enqueue(R);
                    R.Setcheckupday(day);
                    InServiceMRovers->DeleteFirst();
                    countInServiceMountRovers--;
                    countCheckupMountRovers++;
                }
                else if (R.getMissionsCompleted() < R.getInfo().numMissionsBeforeCheckUp)
                {
                    MRovers->InsertEnd(R);
                    InServiceMRovers->DeleteFirst();
                    countInServiceMountRovers--;
                    countCheckupMountRovers++;
                }
            }
        }
        if (!InServPolarMissions->isEmpty())
        {
            Mission pMp = InServPolarMissions->getHead()->getItem();
            if (pMp.getExecDays() == day - pMp.getED())
            {
                DonePolarMissions->InsertEnd(pMp);
                AllCompletedMissions->enqueueMission(pMp, pMp.getSignificance());
                InServPolarMissions->DeleteFirst();
                countInServPolarMissions--;
                countDonePolarMissions++;
                Rover R = InServicePRovers->getHead()->getItem();
                if (R.getMissionsCompleted() == R.getInfo().numMissionsBeforeCheckUp)
                {
                    PolarCheckupRovers->enqueue(R);
                    R.Setcheckupday(day);
                    InServicePRovers->DeleteFirst();
                    countInServicePolarRovers--;
                    countCheckupPolarRovers++;
                }
                else if (R.getMissionsCompleted() < R.getInfo().numMissionsBeforeCheckUp)
                {
                    PRovers->InsertEnd(R);
                    InServicePRovers->DeleteFirst();
                    countInServicePolarRovers--;
                    countCheckupPolarRovers++;
                }
            }
        }

        if (!InServsearchingMissions->isEmpty())
        {
            Mission SMp = InServsearchingMissions->getHead()->getItem();
            if (SMp.getExecDays() == day - SMp.getED())
            {
                DonesearchingMissions->InsertEnd(SMp);
                AllCompletedMissions->enqueueMission(SMp, SMp.getSignificance());
                InServsearchingMissions->DeleteFirst();
                countInServsearchingMissions--;
                countDonesearchingMissions++;
                Rover R = InServiceSRovers->getHead()->getItem();
                if (R.getMissionsCompleted() == R.getInfo().numMissionsBeforeCheckUp)
                {
                    searchingCheckupRovers->enqueue(R);
                    R.Setcheckupday(day);
                    InServiceSRovers->DeleteFirst();
                    countInServicesearchingRovers--;
                    countCheckupsearchingRovers++;
                }
                else if (R.getMissionsCompleted() < R.getInfo().numMissionsBeforeCheckUp)
                {
                    SRovers->InsertEnd(R);
                    InServiceSRovers->DeleteFirst();
                    countInServicesearchingRovers--;
                    countCheckupsearchingRovers++;
                }
            }
        }
        if (!InServScientificExperimentsMissions->isEmpty())
        {
            Mission pMp = InServScientificExperimentsMissions->getHead()->getItem();
            if (pMp.getExecDays() == day - pMp.getED())
            {
                DoneScientificExperimentsMissions->InsertEnd(pMp);
                AllCompletedMissions->enqueueMission(pMp, pMp.getSignificance());
                InServScientificExperimentsMissions->DeleteFirst();
                countInServScientificExperimentsMissions--;
                countDoneScientificExperimentsMissions++;
                Rover R = InServiceYRovers->getHead()->getItem();
                if (R.getMissionsCompleted() == R.getInfo().numMissionsBeforeCheckUp)
                {
                    ScientificExperimentsCheckupRovers->enqueue(R);
                    R.Setcheckupday(day);
                    InServiceYRovers->DeleteFirst();
                    countInServiceScientificExperimentsRovers--;
                    countCheckupScientificExperimentsRovers++;
                }
                else if (R.getMissionsCompleted() < R.getInfo().numMissionsBeforeCheckUp)
                {
                    YRovers->InsertEnd(R);
                    InServiceYRovers->DeleteFirst();
                    countInServiceScientificExperimentsRovers--;
                    countCheckupScientificExperimentsRovers++;
                }
            }
        }
        if (!InServEmrgMissions->isEmpty())
        {
            Mission eMp = InServEmrgMissions->getHead()->getItem();
            if (eMp.getExecDays() == day - eMp.getED())
            {
                DoneEmrgMissions->InsertEnd(eMp);
                AllCompletedMissions->enqueueMission(eMp, eMp.getSignificance());
                InServEmrgMissions->DeleteFirst();
                countInServEmrgMissions--;
                countDoneEmrgMissions++;
                Rover R = InServiceERovers->getHead()->getItem();
                if (R.getMissionsCompleted() == R.getInfo().numMissionsBeforeCheckUp)
                {
                    EmergencyCheckupRovers->enqueue(R);
                    R.Setcheckupday(day);
                    InServiceERovers->DeleteFirst();
                    countInServiceEmergencyRovers--;
                    countCheckupEmergencyRovers++;
                }
                else if (R.getMissionsCompleted() < R.getInfo().numMissionsBeforeCheckUp)
                {
                    ERovers->InsertEnd(R);
                    InServiceERovers->DeleteFirst();
                    countInServiceEmergencyRovers--;
                    countCheckupEmergencyRovers++;
                }
            }
        }
        ///////////Checking The End Of The Check Up Duration Of Each Type Of Rovers \\\\\\\\\\\\\\\\\\\\\\

        if (!EmergencyCheckupRovers->isEmpty())
        {
            Rover R;
            EmergencyCheckupRovers->peek(R);
            if (R.getInfo().checkUpDuration == day - R.getcheckupday())
                EmergencyCheckupRovers->dequeue(R);
            ERovers->InsertEnd(R);
            countAvailableEmergencyRovers++;
            countCheckupEmergencyRovers--;
        }
        if (!MountCheckupRovers->isEmpty())
        {
            Rover R;
            MountCheckupRovers->peek(R);
            if (R.getInfo().checkUpDuration == day - R.getcheckupday())
                MountCheckupRovers->dequeue(R);
            MRovers->InsertEnd(R);
            countAvailableMountRovers++;
            countCheckupMountRovers--;
        }

        if (!EmergencyCheckupRovers->isEmpty())
        {
            Rover R;
            PolarCheckupRovers->peek(R);
            if (R.getInfo().checkUpDuration == day - R.getcheckupday())
                PolarCheckupRovers->dequeue(R);
            PRovers->InsertEnd(R);
            countAvailablePolarRovers++;
            countCheckupPolarRovers--;
        }
        if (!searchingCheckupRovers->isEmpty())
        {
            Rover R;
            searchingCheckupRovers->peek(R);
            if (R.getInfo().checkUpDuration == day - R.getcheckupday())
                searchingCheckupRovers->dequeue(R);
            SRovers->InsertEnd(R);
            countAvailablesearchingRovers++;
            countCheckupsearchingRovers--;
        }
        if (!ScientificExperimentsCheckupRovers->isEmpty())
        {
            Rover R;
            ScientificExperimentsCheckupRovers->peek(R);
            if (R.getInfo().checkUpDuration == day - R.getcheckupday())
                ScientificExperimentsCheckupRovers->dequeue(R);
            YRovers->InsertEnd(R);
            countAvailableScientificExperimentsRovers++;
            countCheckupScientificExperimentsRovers--;
        }


        cout << " Waiting Mountain Missions IDs: ";
        Node<Mission>* a = WaitingMountMissions->getHead();
        while (a != nullptr)
        {
            cout << a->getItem().getID() << " ";
            a = a->getNext();

        }
        cout << endl;
        cout << " Waiting Polar Missions IDs: ";
        Node<Mission>* b = WaitingPolarMissions->getHead();
        while (b != nullptr)
        {
            cout << b->getItem().getID() << " ";
            b = b->getNext();

        }
        cout << endl;

        cout << " Waiting  Emrgecy Missions IDs: ";

        Node<Mission>* c = WaitingEmrgMissions->getHead();
        while (c != nullptr)
        {
            cout << c->getItem().getID() << " ";
            c = c->getNext();

        }
        cout << endl;


        cout << " In Service Mountain Missions IDs: ";
        a = InServMountMissions->getHead();
        while (a != nullptr)
        {
            cout << a->getItem().getID() << " ";
            a = a->getNext();

        }
        cout << endl;
        cout << " In Service Polar Missions IDs: ";
        b = InServPolarMissions->getHead();
        while (b != nullptr)
        {
            cout << b->getItem().getID() << " ";
            b = b->getNext();

        }
        cout << endl;

        cout << " In Service  Emrgecy Missions IDs: ";

        c = InServEmrgMissions->getHead();
        while (c != nullptr)
        {
            cout << c->getItem().getID() << " ";
            c = c->getNext();

        }
        cout << endl;

        cout << " Done Mountain Missions IDs: ";
        a = DoneMountMissions->getHead();
        while (a != nullptr)
        {
            cout << a->getItem().getID() << " ";
            a = a->getNext();

        }
        cout << endl;
        cout << " Done Polar Missions IDs: ";
        b = DonePolarMissions->getHead();
        while (b != nullptr)
        {
            cout << b->getItem().getID() << " ";
            b = b->getNext();
        }
        cout << endl;

        cout << " Done  Emrgecy Missions IDs: ";

        c = DoneEmrgMissions->getHead();
        while (c != nullptr)
        {
            cout << c->getItem().getID() << " ";
            c = c->getNext();

        }
        cout << endl;



        cout << "# of  Waiting Missions (M P E) : " << countWaitingMountMissions << " " << countWaitingPolarMissions << " " << countWaitingEmrgMissions << endl;
        cout << "# of  In Service Missions (M P E) : " << countInServMountMissions << " " << countInServPolarMissions << " " << countInServEmrgMissions << endl;
        cout << "# of  Done Missions (M P E) : " << countDoneMountMissions << " " << countDonePolarMissions << " " << countDoneEmrgMissions << endl;




        cout << "Enter 1 to increment days." << "\n";
        cin >> userInput;
    }
}

void Station::write_Output()
{
    //This function should be called after all missions that are read, are completed , ie. afer everything else is finished.
    //make a file named "Output" , put it in project file.
    //make a counter for Number of Auto promoted Missions named "countAutoProm" that is incremented each time a mission is "automatically" promoted to emergent.
    //make a queue named "AllCompletedMissions" in the station class to be used in the output & enque in it whenever a mission of any type is finished, along with the already made 3 other doneMission queues.
    ofstream outfile;
    outfile.open("Output.txt", ios::out);
    if (outfile.is_open())
    {
        Mission temp;
        int number = countDoneEmrgMissions + countDoneMountMissions + countDonePolarMissions;
        //increment those 3 counters after enqueing any mission of a given type to it's done queue;
        int totalWait = 0;
        int totalExec = 0;
        int avgWait;
        int avgExec;
        int autoProm = (countAutoProm * 100) / number;
        TotalRovers = countMountRovers + countPolarRovers + countEmergencytRovers;

        while (!AllCompletedMissions->isEmpty())
        {
            AllCompletedMissions->dequeue(temp);
            totalWait = totalWait + temp.getWaitDays();
            totalExec = totalExec + temp.getExecDays();
            outfile << temp.getFinishDay() << " " << temp.getID() << " " << temp.getFormDay() << " " << temp.getWaitDays() << " " << temp.getExecDays() << " \n";

        }
        avgWait = totalWait / number;
        avgExec = totalExec / number;
        outfile << number << " [M: " << countDoneMountMissions << " P: " << countDonePolarMissions << " E: " << countDoneEmrgMissions << " ] \n";
        outfile << TotalRovers << " [M: " << countMountRovers << " P: " << countPolarRovers << " E: " << countEmergencytRovers << " ] \n";
        outfile << "AvgWait: " << avgWait << " AvgExec: " << avgExec << "\n";
        outfile << "Auto-Promoted: " << autoProm << "\n";
        outfile.close();
    }
    else cout << "Failed to open file";
}

void Station::stepByStepSimulator()
{
    countWaitingMountMissions = 0;
    countWaitingPolarMissions = 0;
    countWaitingEmrgMissions = 0;
    countInServMountMissions = 0;
    countInServPolarMissions = 0;
    countInServEmrgMissions = 0;
    countDoneMountMissions = 0;
    countDonePolarMissions = 0;
    countDoneEmrgMissions = 0;
    countMountRovers = 0;
    countPolarRovers = 0;
    countEmergencytRovers = 0;
    TotalRovers = 0;
    countFormulationEvent = 0;
    countCancellationEvent = 0;
    countPromotionEvent = 0;
    countAvailableEmergencyRovers = 0;
    countAvailableMountRovers = 0;
    countAvailablePolarRovers = 0;
    countInServiceEmergencyRovers = 0;
    countInServiceMountRovers = 0;
    countInServicePolarRovers = 0;
    countAutoProm = 0;

    countWaitingsearchingMissions=0;
    countWaitingScientificExperimentsMissions=0;
    countInServsearchingMissions=0;
    countInServScientificExperimentsMissions=0;
    countDonesearchingMissions=0;
    countDoneScientificExperimentsMissions=0;
    countSearchRovers=0;
    countScientificExperimentstRovers=0;
    countAvailablesearchingRovers=0;
    countAvailableScientificExperimentsRovers=0;
    countInServicesearchingRovers=0;
    countInServiceScientificExperimentsRovers=0;
    countCheckupsearchingRovers=0;

    int countCheckupScientificExperimentsRovers;

    fileLoader();
    cout << "# Of Rovers of Each Type (M P E) : " << countMountRovers << " " << countPolarRovers << " " << countEmergencytRovers << endl;
    cout << "# Formulation Events " << countFormulationEvent << " # Cancellation Events " << countCancellationEvent << " # Promotion Events " << countPromotionEvent << endl;

    int day = 0;
    int userInput;

    while (true)
    {
        if (WaitingEmrgMissions->isEmpty() && WaitingPolarMissions->isEmpty() && WaitingMountMissions->isEmpty() && InServEmrgMissions->isEmpty() && InServPolarMissions->isEmpty() && InServMountMissions->isEmpty() && FormEvents->isEmpty() && CancelEvents->isEmpty())
        {
            cout << "All Done! No Active Missions or Events (Except For Promotion events Which we ignored this phase)" << endl;
            break;
        }
        day++;
        cout << "Today is Day " << day << endl;
        Formulation F;
        Cancellation C;
        Promotion Pr;
        Node<Mission>* p = nullptr;
        for (int i = 0; i < countFormulationEvent; i++)
        {

            FormEvents->peek(F);
            if (F.getED() == day && FormEvents->peek(F))
            {
                //cout << F.getED() << " " << day << endl;
                FormEvents->dequeue(F);
                switch (F.getType())
                {
                    case M: WaitingMountMissions->InsertEnd(Mission(M, F.getID(), F.getTLOC(), F.getSIG(), day, F.getMDUR(), F.getED()));
                        countWaitingMountMissions++;


                        break;
                    case P: WaitingPolarMissions->InsertEnd(Mission(P, F.getID(), F.getTLOC(), F.getSIG(), day, F.getMDUR(), F.getED()));
                        countWaitingPolarMissions++;
                        // countpromotionwaitingdays++;
                        break;
                    case E:
                        WaitingEmrgMissions->InsertEnd(Mission(E, F.getID(), F.getTLOC(), F.getSIG(), day, F.getMDUR(), F.getED()));
                        countWaitingEmrgMissions++;
                        break;
                    case S:
                        WaitingsearchingMissions->InsertEnd(Mission(E, F.getID(), F.getTLOC(), F.getSIG(), day, F.getMDUR(), F.getED()));
                        countWaitingsearchingMissions++;
                        break;
                    case Y:
                        WaitingScientificExperimentsMissions->InsertEnd(Mission(E, F.getID(), F.getTLOC(), F.getSIG(), day, F.getMDUR(), F.getED()));
                        countWaitingScientificExperimentsMissions++;
                        break;

                }

            }
            else
            {
                break;
            }
        }


        for (int i = 0; i < countCancellationEvent; i++)
        {
            CancelEvents->peek(C);
            p = WaitingMountMissions->getHead();
            if (C.getED() == day && CancelEvents->peek(C))
            {
                CancelEvents->dequeue(C);
                while (p)
                {
                    if (C.getID() == p->getItem().getID())
                    {
                        WaitingMountMissions->DeleteNode(p->getItem());
                        countWaitingMountMissions--;
                        break;
                    }
                    p = p->getNext();
                }
            }
            else
            {
                break;
            }
        }

        for (int i = 0; i < countPromotionEvent; i++)
        {
            PromEvents->peek(Pr);
            p = WaitingMountMissions->getHead();
            if (Pr.getED() == day && PromEvents->peek(Pr))
            {

                while (p)
                {
                    if (Pr.getID() == p->getItem().getID() && day - p->getItem().getFormDay() == Promotion::GetAutoP())
                    {
                        PromEvents->dequeue(Pr);

                        WaitingEmrgMissions->InsertEnd(Mission(E, Pr.getID(), p->getItem().getDistance(), p->getItem().getSignificance(), day, p->getItem().getDurationAtLocation(), F.getED()));
                        countWaitingEmrgMissions++;
                        WaitingMountMissions->DeleteNode(p->getItem());
                        countWaitingMountMissions--;
                        countAutoProm++;
                        break;
                    }
                    p = p->getNext();
                }
            }
            else
            {
                break;
            }
        }

        if (!WaitingEmrgMissions->isEmpty() && (!PRovers->isEmpty() || !MRovers->isEmpty() || !ERovers->isEmpty()))
        {
            Mission eMp = WaitingEmrgMissions->FindMaxPrioritNode()->getItem();
            if (eMp.getED() == day)
            {
                InServEmrgMissions->InsertEnd(eMp);
                WaitingEmrgMissions->DeleteNode(WaitingEmrgMissions->FindMaxPrioritNode()->getItem());
                countWaitingEmrgMissions--;
                countInServEmrgMissions++;

                ////////////////////////////////////////////////////////////////////////////////////////////////

                if (!ERovers->isEmpty())
                {
                    Rover R = ERovers->getHead()->getItem();
                    InServiceERovers->InsertEnd(R);
                    ERovers->DeleteFirst();
                    countAvailableEmergencyRovers--;
                    countInServiceEmergencyRovers++;
                }
                else if (!MRovers->isEmpty())
                {

                    Rover R = MRovers->getHead()->getItem();
                    InServiceMRovers->InsertEnd(R);
                    MRovers->DeleteFirst();
                    countAvailableMountRovers--;
                    countInServiceEmergencyRovers++;
                }
                else if (!PRovers->isEmpty())
                {
                    Rover R = PRovers->getHead()->getItem();
                    InServicePRovers->InsertEnd(R);
                    PRovers->DeleteFirst();
                    countAvailablePolarRovers--;
                    countInServiceEmergencyRovers++;
                }
            }
        }

        if (!WaitingMountMissions->isEmpty() && (!MRovers->isEmpty() || !ERovers->isEmpty()))
        {
            Mission mMp = WaitingMountMissions->getHead()->getItem();
            if (mMp.getED() == day)
            {
                InServMountMissions->InsertEnd(mMp);
                WaitingMountMissions->DeleteFirst();
                countWaitingMountMissions--;
                countInServMountMissions++;

                if (!MRovers->isEmpty())
                {
                    Rover R = MRovers->getHead()->getItem();
                    InServiceMRovers->InsertEnd(R);
                    MRovers->DeleteFirst();
                    countAvailableMountRovers--;
                    countInServiceMountRovers++;
                }
                else if (!ERovers->isEmpty())
                {
                    Rover R = ERovers->getHead()->getItem();
                    InServiceERovers->InsertEnd(R);
                    ERovers->DeleteFirst();
                    countAvailableEmergencyRovers--;
                    countInServiceMountRovers++;
                }
            }
        }
        if (!WaitingPolarMissions->isEmpty() && (!PRovers->isEmpty()))
        {
            Mission pMp = WaitingPolarMissions->getHead()->getItem();
            if (pMp.getED() == day)
            {
                InServPolarMissions->InsertEnd(pMp);
                WaitingPolarMissions->DeleteFirst();
                countWaitingPolarMissions--;
                countInServPolarMissions++;

                Rover R = PRovers->getHead()->getItem();
                InServicePRovers->InsertEnd(R);
                PRovers->DeleteFirst();
                countAvailablePolarRovers--;
                countInServicePolarRovers++;
            }
        }

        if (!WaitingsearchingMissions->isEmpty() && (!SRovers->isEmpty()))
        {
            Mission SMp = WaitingPolarMissions->getHead()->getItem();
            if (SMp.getED() == day)
            {
                InServsearchingMissions->InsertEnd(SMp);
                WaitingsearchingMissions->DeleteFirst();
                countWaitingsearchingMissions--;
                countInServsearchingMissions++;

                Rover R = SRovers->getHead()->getItem();
                InServiceSRovers->InsertEnd(R);
                SRovers->DeleteFirst();
                countAvailablesearchingRovers--;
                countInServicesearchingRovers++;
            }
        }

        if (!WaitingScientificExperimentsMissions->isEmpty() && (!YRovers->isEmpty()))
        {
            Mission YMp = WaitingPolarMissions->getHead()->getItem();
            if (YMp.getED() == day)
            {
                InServScientificExperimentsMissions->InsertEnd(YMp);
                WaitingScientificExperimentsMissions->DeleteFirst();
                countWaitingScientificExperimentsMissions--;
                countInServScientificExperimentsMissions++;

                Rover R = YRovers->getHead()->getItem();
                InServiceYRovers->InsertEnd(R);
                YRovers->DeleteFirst();
                countAvailableScientificExperimentsRovers--;
                countInServiceScientificExperimentsRovers++;
            }
        }



        if (!InServMountMissions->isEmpty())
        {
            Mission mMp = InServMountMissions->getHead()->getItem();
            if (mMp.getExecDays() == day - mMp.getED())
            {
                DoneMountMissions->InsertEnd(mMp);
                AllCompletedMissions->enqueueMission(mMp, mMp.getSignificance());
                InServMountMissions->DeleteFirst();
                countInServMountMissions--;
                countDoneMountMissions++;
                Rover R = InServiceMRovers->getHead()->getItem();
                if (R.getMissionsCompleted() == R.getInfo().numMissionsBeforeCheckUp)
                {
                    MountCheckupRovers->enqueue(R);
                    R.Setcheckupday(day);
                    InServiceMRovers->DeleteFirst();
                    countInServiceMountRovers--;
                    countCheckupMountRovers++;
                }
                else if (R.getMissionsCompleted() < R.getInfo().numMissionsBeforeCheckUp)
                {
                    MRovers->InsertEnd(R);
                    InServiceMRovers->DeleteFirst();
                    countInServiceMountRovers--;
                    countCheckupMountRovers++;
                }
            }
        }
        if (!InServPolarMissions->isEmpty())
        {
            Mission pMp = InServPolarMissions->getHead()->getItem();
            if (pMp.getExecDays() == day - pMp.getED())
            {
                DonePolarMissions->InsertEnd(pMp);
                AllCompletedMissions->enqueueMission(pMp, pMp.getSignificance());
                InServPolarMissions->DeleteFirst();
                countInServPolarMissions--;
                countDonePolarMissions++;
                Rover R = InServicePRovers->getHead()->getItem();
                if (R.getMissionsCompleted() == R.getInfo().numMissionsBeforeCheckUp)
                {
                    PolarCheckupRovers->enqueue(R);
                    R.Setcheckupday(day);
                    InServicePRovers->DeleteFirst();
                    countInServicePolarRovers--;
                    countCheckupPolarRovers++;
                }
                else if (R.getMissionsCompleted() < R.getInfo().numMissionsBeforeCheckUp)
                {
                    PRovers->InsertEnd(R);
                    InServicePRovers->DeleteFirst();
                    countInServicePolarRovers--;
                    countCheckupPolarRovers++;
                }
            }
        }

        if (!InServsearchingMissions->isEmpty())
        {
            Mission SMp = InServsearchingMissions->getHead()->getItem();
            if (SMp.getExecDays() == day - SMp.getED())
            {
                DonesearchingMissions->InsertEnd(SMp);
                AllCompletedMissions->enqueueMission(SMp, SMp.getSignificance());
                InServsearchingMissions->DeleteFirst();
                countInServsearchingMissions--;
                countDonesearchingMissions++;
                Rover R = InServiceSRovers->getHead()->getItem();
                if (R.getMissionsCompleted() == R.getInfo().numMissionsBeforeCheckUp)
                {
                    searchingCheckupRovers->enqueue(R);
                    R.Setcheckupday(day);
                    InServiceSRovers->DeleteFirst();
                    countInServicesearchingRovers--;
                    countCheckupsearchingRovers++;
                }
                else if (R.getMissionsCompleted() < R.getInfo().numMissionsBeforeCheckUp)
                {
                    SRovers->InsertEnd(R);
                    InServiceSRovers->DeleteFirst();
                    countInServicesearchingRovers--;
                    countCheckupsearchingRovers++;
                }
            }
        }
        if (!InServScientificExperimentsMissions->isEmpty())
        {
            Mission pMp = InServScientificExperimentsMissions->getHead()->getItem();
            if (pMp.getExecDays() == day - pMp.getED())
            {
                DoneScientificExperimentsMissions->InsertEnd(pMp);
                AllCompletedMissions->enqueueMission(pMp, pMp.getSignificance());
                InServScientificExperimentsMissions->DeleteFirst();
                countInServScientificExperimentsMissions--;
                countDoneScientificExperimentsMissions++;
                Rover R = InServiceYRovers->getHead()->getItem();
                if (R.getMissionsCompleted() == R.getInfo().numMissionsBeforeCheckUp)
                {
                    ScientificExperimentsCheckupRovers->enqueue(R);
                    R.Setcheckupday(day);
                    InServiceYRovers->DeleteFirst();
                    countInServiceScientificExperimentsRovers--;
                    countCheckupScientificExperimentsRovers++;
                }
                else if (R.getMissionsCompleted() < R.getInfo().numMissionsBeforeCheckUp)
                {
                    YRovers->InsertEnd(R);
                    InServiceYRovers->DeleteFirst();
                    countInServiceScientificExperimentsRovers--;
                    countCheckupScientificExperimentsRovers++;
                }
            }
        }
        if (!InServEmrgMissions->isEmpty())
        {
            Mission eMp = InServEmrgMissions->getHead()->getItem();
            if (eMp.getExecDays() == day - eMp.getED())
            {
                DoneEmrgMissions->InsertEnd(eMp);
                AllCompletedMissions->enqueueMission(eMp, eMp.getSignificance());
                InServEmrgMissions->DeleteFirst();
                countInServEmrgMissions--;
                countDoneEmrgMissions++;
                Rover R = InServiceERovers->getHead()->getItem();
                if (R.getMissionsCompleted() == R.getInfo().numMissionsBeforeCheckUp)
                {
                    EmergencyCheckupRovers->enqueue(R);
                    R.Setcheckupday(day);
                    InServiceERovers->DeleteFirst();
                    countInServiceEmergencyRovers--;
                    countCheckupEmergencyRovers++;
                }
                else if (R.getMissionsCompleted() < R.getInfo().numMissionsBeforeCheckUp)
                {
                    ERovers->InsertEnd(R);
                    InServiceERovers->DeleteFirst();
                    countInServiceEmergencyRovers--;
                    countCheckupEmergencyRovers++;
                }
            }
        }
        ///////////Checking The End Of The Check Up Duration Of Each Type Of Rovers \\\\\\\\\\\\\\\\\\\\\\

        if (!EmergencyCheckupRovers->isEmpty())
        {
            Rover R;
            EmergencyCheckupRovers->peek(R);
            if (R.getInfo().checkUpDuration == day - R.getcheckupday())
                EmergencyCheckupRovers->dequeue(R);
            ERovers->InsertEnd(R);
            countAvailableEmergencyRovers++;
            countCheckupEmergencyRovers--;
        }
        if (!MountCheckupRovers->isEmpty())
        {
            Rover R;
            MountCheckupRovers->peek(R);
            if (R.getInfo().checkUpDuration == day - R.getcheckupday())
                MountCheckupRovers->dequeue(R);
            MRovers->InsertEnd(R);
            countAvailableMountRovers++;
            countCheckupMountRovers--;
        }

        if (!EmergencyCheckupRovers->isEmpty())
        {
            Rover R;
            PolarCheckupRovers->peek(R);
            if (R.getInfo().checkUpDuration == day - R.getcheckupday())
                PolarCheckupRovers->dequeue(R);
            PRovers->InsertEnd(R);
            countAvailablePolarRovers++;
            countCheckupPolarRovers--;
        }
        if (!searchingCheckupRovers->isEmpty())
        {
            Rover R;
            searchingCheckupRovers->peek(R);
            if (R.getInfo().checkUpDuration == day - R.getcheckupday())
                searchingCheckupRovers->dequeue(R);
            SRovers->InsertEnd(R);
            countAvailablesearchingRovers++;
            countCheckupsearchingRovers--;
        }
        if (!ScientificExperimentsCheckupRovers->isEmpty())
        {
            Rover R;
            ScientificExperimentsCheckupRovers->peek(R);
            if (R.getInfo().checkUpDuration == day - R.getcheckupday())
                ScientificExperimentsCheckupRovers->dequeue(R);
            YRovers->InsertEnd(R);
            countAvailableScientificExperimentsRovers++;
            countCheckupScientificExperimentsRovers--;
        }


        cout << " Waiting Mountain Missions IDs: ";
        Node<Mission>* a = WaitingMountMissions->getHead();
        while (a != nullptr)
        {
            cout << a->getItem().getID() << " ";
            a = a->getNext();

        }
        cout << endl;
        cout << " Waiting Polar Missions IDs: ";
        Node<Mission>* b = WaitingPolarMissions->getHead();
        while (b != nullptr)
        {
            cout << b->getItem().getID() << " ";
            b = b->getNext();

        }
        cout << endl;

        cout << " Waiting  Emrgecy Missions IDs: ";

        Node<Mission>* c = WaitingEmrgMissions->getHead();
        while (c != nullptr)
        {
            cout << c->getItem().getID() << " ";
            c = c->getNext();

        }
        cout << endl;


        cout << " In Service Mountain Missions IDs: ";
        a = InServMountMissions->getHead();
        while (a != nullptr)
        {
            cout << a->getItem().getID() << " ";
            a = a->getNext();

        }
        cout << endl;
        cout << " In Service Polar Missions IDs: ";
        b = InServPolarMissions->getHead();
        while (b != nullptr)
        {
            cout << b->getItem().getID() << " ";
            b = b->getNext();

        }
        cout << endl;

        cout << " In Service  Emrgecy Missions IDs: ";

        c = InServEmrgMissions->getHead();
        while (c != nullptr)
        {
            cout << c->getItem().getID() << " ";
            c = c->getNext();

        }
        cout << endl;

        cout << " Done Mountain Missions IDs: ";
        a = DoneMountMissions->getHead();
        while (a != nullptr)
        {
            cout << a->getItem().getID() << " ";
            a = a->getNext();

        }
        cout << endl;
        cout << " Done Polar Missions IDs: ";
        b = DonePolarMissions->getHead();
        while (b != nullptr)
        {
            cout << b->getItem().getID() << " ";
            b = b->getNext();
        }
        cout << endl;

        cout << " Done  Emrgecy Missions IDs: ";

        c = DoneEmrgMissions->getHead();
        while (c != nullptr)
        {
            cout << c->getItem().getID() << " ";
            c = c->getNext();

        }
        cout << endl;



        cout << "# of  Waiting Missions (M P E) : " << countWaitingMountMissions << " " << countWaitingPolarMissions << " " << countWaitingEmrgMissions << endl;
        cout << "# of  In Service Missions (M P E) : " << countInServMountMissions << " " << countInServPolarMissions << " " << countInServEmrgMissions << endl;
        cout << "# of  Done Missions (M P E) : " << countDoneMountMissions << " " << countDonePolarMissions << " " << countDoneEmrgMissions << endl;

        sleep(1);
    }
}

void Station::silentSimulator() {
    countWaitingMountMissions = 0;
    countWaitingPolarMissions = 0;
    countWaitingEmrgMissions = 0;
    countInServMountMissions = 0;
    countInServPolarMissions = 0;
    countInServEmrgMissions = 0;
    countDoneMountMissions = 0;
    countDonePolarMissions = 0;
    countDoneEmrgMissions = 0;
    countMountRovers = 0;
    countPolarRovers = 0;
    countEmergencytRovers = 0;
    TotalRovers = 0;
    countFormulationEvent = 0;
    countCancellationEvent = 0;
    countPromotionEvent = 0;
    countAvailableEmergencyRovers = 0;
    countAvailableMountRovers = 0;
    countAvailablePolarRovers = 0;
    countInServiceEmergencyRovers = 0;
    countInServiceMountRovers = 0;
    countInServicePolarRovers = 0;
    countAutoProm = 0;

    countWaitingsearchingMissions = 0;
    countWaitingScientificExperimentsMissions = 0;
    countInServsearchingMissions = 0;
    countInServScientificExperimentsMissions = 0;
    countDonesearchingMissions = 0;
    countDoneScientificExperimentsMissions = 0;
    countSearchRovers = 0;
    countScientificExperimentstRovers = 0;
    countAvailablesearchingRovers = 0;
    countAvailableScientificExperimentsRovers = 0;
    countInServicesearchingRovers = 0;
    countInServiceScientificExperimentsRovers = 0;
    countCheckupsearchingRovers = 0;

    int countCheckupScientificExperimentsRovers;

    fileLoader();

    int day = 0;
    int userInput;

    while (true) {
        if (WaitingEmrgMissions->isEmpty() && WaitingPolarMissions->isEmpty() && WaitingMountMissions->isEmpty() &&
            InServEmrgMissions->isEmpty() && InServPolarMissions->isEmpty() && InServMountMissions->isEmpty() &&
            FormEvents->isEmpty() && CancelEvents->isEmpty()) {
            break;
        }
        day++;
        Formulation F;
        Cancellation C;
        Promotion Pr;
        Node<Mission> *p = nullptr;
        for (int i = 0; i < countFormulationEvent; i++) {

            FormEvents->peek(F);
            if (F.getED() == day && FormEvents->peek(F)) {
                //cout << F.getED() << " " << day << endl;
                FormEvents->dequeue(F);
                switch (F.getType()) {
                    case M:
                        WaitingMountMissions->InsertEnd(
                                Mission(M, F.getID(), F.getTLOC(), F.getSIG(), day, F.getMDUR(), F.getED()));
                        countWaitingMountMissions++;


                        break;
                    case P:
                        WaitingPolarMissions->InsertEnd(
                                Mission(P, F.getID(), F.getTLOC(), F.getSIG(), day, F.getMDUR(), F.getED()));
                        countWaitingPolarMissions++;
                        // countpromotionwaitingdays++;
                        break;
                    case E:
                        WaitingEmrgMissions->InsertEnd(
                                Mission(E, F.getID(), F.getTLOC(), F.getSIG(), day, F.getMDUR(), F.getED()));
                        countWaitingEmrgMissions++;
                        break;
                    case S:
                        WaitingsearchingMissions->InsertEnd(
                                Mission(E, F.getID(), F.getTLOC(), F.getSIG(), day, F.getMDUR(), F.getED()));
                        countWaitingsearchingMissions++;
                        break;
                    case Y:
                        WaitingScientificExperimentsMissions->InsertEnd(
                                Mission(E, F.getID(), F.getTLOC(), F.getSIG(), day, F.getMDUR(), F.getED()));
                        countWaitingScientificExperimentsMissions++;
                        break;

                }

            } else {
                break;
            }
        }


        for (int i = 0; i < countCancellationEvent; i++) {
            CancelEvents->peek(C);
            p = WaitingMountMissions->getHead();
            if (C.getED() == day && CancelEvents->peek(C)) {
                CancelEvents->dequeue(C);
                while (p) {
                    if (C.getID() == p->getItem().getID()) {
                        WaitingMountMissions->DeleteNode(p->getItem());
                        countWaitingMountMissions--;
                        break;
                    }
                    p = p->getNext();
                }
            } else {
                break;
            }
        }

        for (int i = 0; i < countPromotionEvent; i++) {
            PromEvents->peek(Pr);
            p = WaitingMountMissions->getHead();
            if (Pr.getED() == day && PromEvents->peek(Pr)) {

                while (p) {
                    if (Pr.getID() == p->getItem().getID() &&
                        day - p->getItem().getFormDay() == Promotion::GetAutoP()) {
                        PromEvents->dequeue(Pr);

                        WaitingEmrgMissions->InsertEnd(
                                Mission(E, Pr.getID(), p->getItem().getDistance(), p->getItem().getSignificance(), day,
                                        p->getItem().getDurationAtLocation(), F.getED()));
                        countWaitingEmrgMissions++;
                        WaitingMountMissions->DeleteNode(p->getItem());
                        countWaitingMountMissions--;
                        countAutoProm++;
                        break;
                    }
                    p = p->getNext();
                }
            } else {
                break;
            }
        }

        if (!WaitingEmrgMissions->isEmpty() && (!PRovers->isEmpty() || !MRovers->isEmpty() || !ERovers->isEmpty())) {
            Mission eMp = WaitingEmrgMissions->FindMaxPrioritNode()->getItem();
            if (eMp.getED() == day) {
                InServEmrgMissions->InsertEnd(eMp);
                WaitingEmrgMissions->DeleteNode(WaitingEmrgMissions->FindMaxPrioritNode()->getItem());
                countWaitingEmrgMissions--;
                countInServEmrgMissions++;

                ////////////////////////////////////////////////////////////////////////////////////////////////

                if (!ERovers->isEmpty()) {
                    Rover R = ERovers->getHead()->getItem();
                    InServiceERovers->InsertEnd(R);
                    ERovers->DeleteFirst();
                    countAvailableEmergencyRovers--;
                    countInServiceEmergencyRovers++;
                } else if (!MRovers->isEmpty()) {

                    Rover R = MRovers->getHead()->getItem();
                    InServiceMRovers->InsertEnd(R);
                    MRovers->DeleteFirst();
                    countAvailableMountRovers--;
                    countInServiceEmergencyRovers++;
                } else if (!PRovers->isEmpty()) {
                    Rover R = PRovers->getHead()->getItem();
                    InServicePRovers->InsertEnd(R);
                    PRovers->DeleteFirst();
                    countAvailablePolarRovers--;
                    countInServiceEmergencyRovers++;
                }
            }
        }

        if (!WaitingMountMissions->isEmpty() && (!MRovers->isEmpty() || !ERovers->isEmpty())) {
            Mission mMp = WaitingMountMissions->getHead()->getItem();
            if (mMp.getED() == day) {
                InServMountMissions->InsertEnd(mMp);
                WaitingMountMissions->DeleteFirst();
                countWaitingMountMissions--;
                countInServMountMissions++;

                if (!MRovers->isEmpty()) {
                    Rover R = MRovers->getHead()->getItem();
                    InServiceMRovers->InsertEnd(R);
                    MRovers->DeleteFirst();
                    countAvailableMountRovers--;
                    countInServiceMountRovers++;
                } else if (!ERovers->isEmpty()) {
                    Rover R = ERovers->getHead()->getItem();
                    InServiceERovers->InsertEnd(R);
                    ERovers->DeleteFirst();
                    countAvailableEmergencyRovers--;
                    countInServiceMountRovers++;
                }
            }
        }
        if (!WaitingPolarMissions->isEmpty() && (!PRovers->isEmpty())) {
            Mission pMp = WaitingPolarMissions->getHead()->getItem();
            if (pMp.getED() == day) {
                InServPolarMissions->InsertEnd(pMp);
                WaitingPolarMissions->DeleteFirst();
                countWaitingPolarMissions--;
                countInServPolarMissions++;

                Rover R = PRovers->getHead()->getItem();
                InServicePRovers->InsertEnd(R);
                PRovers->DeleteFirst();
                countAvailablePolarRovers--;
                countInServicePolarRovers++;
            }
        }

        if (!WaitingsearchingMissions->isEmpty() && (!SRovers->isEmpty())) {
            Mission SMp = WaitingPolarMissions->getHead()->getItem();
            if (SMp.getED() == day) {
                InServsearchingMissions->InsertEnd(SMp);
                WaitingsearchingMissions->DeleteFirst();
                countWaitingsearchingMissions--;
                countInServsearchingMissions++;

                Rover R = SRovers->getHead()->getItem();
                InServiceSRovers->InsertEnd(R);
                SRovers->DeleteFirst();
                countAvailablesearchingRovers--;
                countInServicesearchingRovers++;
            }
        }

        if (!WaitingScientificExperimentsMissions->isEmpty() && (!YRovers->isEmpty())) {
            Mission YMp = WaitingPolarMissions->getHead()->getItem();
            if (YMp.getED() == day) {
                InServScientificExperimentsMissions->InsertEnd(YMp);
                WaitingScientificExperimentsMissions->DeleteFirst();
                countWaitingScientificExperimentsMissions--;
                countInServScientificExperimentsMissions++;

                Rover R = YRovers->getHead()->getItem();
                InServiceYRovers->InsertEnd(R);
                YRovers->DeleteFirst();
                countAvailableScientificExperimentsRovers--;
                countInServiceScientificExperimentsRovers++;
            }
        }


        if (!InServMountMissions->isEmpty()) {
            Mission mMp = InServMountMissions->getHead()->getItem();
            if (mMp.getExecDays() == day - mMp.getED()) {
                DoneMountMissions->InsertEnd(mMp);
                AllCompletedMissions->enqueueMission(mMp, mMp.getSignificance());
                InServMountMissions->DeleteFirst();
                countInServMountMissions--;
                countDoneMountMissions++;
                Rover R = InServiceMRovers->getHead()->getItem();
                if (R.getMissionsCompleted() == R.getInfo().numMissionsBeforeCheckUp) {
                    MountCheckupRovers->enqueue(R);
                    R.Setcheckupday(day);
                    InServiceMRovers->DeleteFirst();
                    countInServiceMountRovers--;
                    countCheckupMountRovers++;
                } else if (R.getMissionsCompleted() < R.getInfo().numMissionsBeforeCheckUp) {
                    MRovers->InsertEnd(R);
                    InServiceMRovers->DeleteFirst();
                    countInServiceMountRovers--;
                    countCheckupMountRovers++;
                }
            }
        }
        if (!InServPolarMissions->isEmpty()) {
            Mission pMp = InServPolarMissions->getHead()->getItem();
            if (pMp.getExecDays() == day - pMp.getED()) {
                DonePolarMissions->InsertEnd(pMp);
                AllCompletedMissions->enqueueMission(pMp, pMp.getSignificance());
                InServPolarMissions->DeleteFirst();
                countInServPolarMissions--;
                countDonePolarMissions++;
                Rover R = InServicePRovers->getHead()->getItem();
                if (R.getMissionsCompleted() == R.getInfo().numMissionsBeforeCheckUp) {
                    PolarCheckupRovers->enqueue(R);
                    R.Setcheckupday(day);
                    InServicePRovers->DeleteFirst();
                    countInServicePolarRovers--;
                    countCheckupPolarRovers++;
                } else if (R.getMissionsCompleted() < R.getInfo().numMissionsBeforeCheckUp) {
                    PRovers->InsertEnd(R);
                    InServicePRovers->DeleteFirst();
                    countInServicePolarRovers--;
                    countCheckupPolarRovers++;
                }
            }
        }

        if (!InServsearchingMissions->isEmpty()) {
            Mission SMp = InServsearchingMissions->getHead()->getItem();
            if (SMp.getExecDays() == day - SMp.getED()) {
                DonesearchingMissions->InsertEnd(SMp);
                AllCompletedMissions->enqueueMission(SMp, SMp.getSignificance());
                InServsearchingMissions->DeleteFirst();
                countInServsearchingMissions--;
                countDonesearchingMissions++;
                Rover R = InServiceSRovers->getHead()->getItem();
                if (R.getMissionsCompleted() == R.getInfo().numMissionsBeforeCheckUp) {
                    searchingCheckupRovers->enqueue(R);
                    R.Setcheckupday(day);
                    InServiceSRovers->DeleteFirst();
                    countInServicesearchingRovers--;
                    countCheckupsearchingRovers++;
                } else if (R.getMissionsCompleted() < R.getInfo().numMissionsBeforeCheckUp) {
                    SRovers->InsertEnd(R);
                    InServiceSRovers->DeleteFirst();
                    countInServicesearchingRovers--;
                    countCheckupsearchingRovers++;
                }
            }
        }
        if (!InServScientificExperimentsMissions->isEmpty()) {
            Mission pMp = InServScientificExperimentsMissions->getHead()->getItem();
            if (pMp.getExecDays() == day - pMp.getED()) {
                DoneScientificExperimentsMissions->InsertEnd(pMp);
                AllCompletedMissions->enqueueMission(pMp, pMp.getSignificance());
                InServScientificExperimentsMissions->DeleteFirst();
                countInServScientificExperimentsMissions--;
                countDoneScientificExperimentsMissions++;
                Rover R = InServiceYRovers->getHead()->getItem();
                if (R.getMissionsCompleted() == R.getInfo().numMissionsBeforeCheckUp) {
                    ScientificExperimentsCheckupRovers->enqueue(R);
                    R.Setcheckupday(day);
                    InServiceYRovers->DeleteFirst();
                    countInServiceScientificExperimentsRovers--;
                    countCheckupScientificExperimentsRovers++;
                } else if (R.getMissionsCompleted() < R.getInfo().numMissionsBeforeCheckUp) {
                    YRovers->InsertEnd(R);
                    InServiceYRovers->DeleteFirst();
                    countInServiceScientificExperimentsRovers--;
                    countCheckupScientificExperimentsRovers++;
                }
            }
        }
        if (!InServEmrgMissions->isEmpty()) {
            Mission eMp = InServEmrgMissions->getHead()->getItem();
            if (eMp.getExecDays() == day - eMp.getED()) {
                DoneEmrgMissions->InsertEnd(eMp);
                AllCompletedMissions->enqueueMission(eMp, eMp.getSignificance());
                InServEmrgMissions->DeleteFirst();
                countInServEmrgMissions--;
                countDoneEmrgMissions++;
                Rover R = InServiceERovers->getHead()->getItem();
                if (R.getMissionsCompleted() == R.getInfo().numMissionsBeforeCheckUp) {
                    EmergencyCheckupRovers->enqueue(R);
                    R.Setcheckupday(day);
                    InServiceERovers->DeleteFirst();
                    countInServiceEmergencyRovers--;
                    countCheckupEmergencyRovers++;
                } else if (R.getMissionsCompleted() < R.getInfo().numMissionsBeforeCheckUp) {
                    ERovers->InsertEnd(R);
                    InServiceERovers->DeleteFirst();
                    countInServiceEmergencyRovers--;
                    countCheckupEmergencyRovers++;
                }
            }
        }
        ///////////Checking The End Of The Check Up Duration Of Each Type Of Rovers \\\\\\\\\\\\\\\\\\\\\\

        if (!EmergencyCheckupRovers->isEmpty()) {
            Rover R;
            EmergencyCheckupRovers->peek(R);
            if (R.getInfo().checkUpDuration == day - R.getcheckupday())
                EmergencyCheckupRovers->dequeue(R);
            ERovers->InsertEnd(R);
            countAvailableEmergencyRovers++;
            countCheckupEmergencyRovers--;
        }
        if (!MountCheckupRovers->isEmpty()) {
            Rover R;
            MountCheckupRovers->peek(R);
            if (R.getInfo().checkUpDuration == day - R.getcheckupday())
                MountCheckupRovers->dequeue(R);
            MRovers->InsertEnd(R);
            countAvailableMountRovers++;
            countCheckupMountRovers--;
        }

        if (!EmergencyCheckupRovers->isEmpty()) {
            Rover R;
            PolarCheckupRovers->peek(R);
            if (R.getInfo().checkUpDuration == day - R.getcheckupday())
                PolarCheckupRovers->dequeue(R);
            PRovers->InsertEnd(R);
            countAvailablePolarRovers++;
            countCheckupPolarRovers--;
        }
        if (!searchingCheckupRovers->isEmpty()) {
            Rover R;
            searchingCheckupRovers->peek(R);
            if (R.getInfo().checkUpDuration == day - R.getcheckupday())
                searchingCheckupRovers->dequeue(R);
            SRovers->InsertEnd(R);
            countAvailablesearchingRovers++;
            countCheckupsearchingRovers--;
        }
        if (!ScientificExperimentsCheckupRovers->isEmpty()) {
            Rover R;
            ScientificExperimentsCheckupRovers->peek(R);
            if (R.getInfo().checkUpDuration == day - R.getcheckupday())
                ScientificExperimentsCheckupRovers->dequeue(R);
            YRovers->InsertEnd(R);
            countAvailableScientificExperimentsRovers++;
            countCheckupScientificExperimentsRovers--;
        }
    }
}
