#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#define MaxNumRovers  100
#define MaxNumMissions 1000
enum TYPE
{
    M,
    P,
    E,
    S,   //  that is a searching mission to search for any missing equipments in space
    Y,  // that is a mission to do scientific experiments there

};

enum MISSION_STATUS
{
    WAIT,
    INSRV,
    DONE,
};


enum MODE
{
    INTR,
    STEP,
    SLNT,
};


#endif 