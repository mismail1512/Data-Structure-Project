#include <iostream>
#include "LinkedList.h"
#include"LinkedQueue.h"
#include"Node.h"
#include "Station.h"
#include "Formulation.h"
using namespace std;
int main()
{
    Station Station;
    int mode;
    cout << "Choose program mode (Interactive = 1, Step-by-step = 2, Silent = 3): ";
    cin >> mode;
    if(mode = 1)
    {
        Station.simpleSimulator();
    }
    if(mode = 2)
    {
        Station.stepByStepSimulator();
    }
    if(mode = 3)
    {
        Station.silentSimulator();
    }

    Station.write_Output();
}
