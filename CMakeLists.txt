cmake_minimum_required(VERSION 3.15)
project(ProjectPhase2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ProjectPhase2
        Cancellation.cpp
        Cancellation.h
        Definitions.h
        Events.cpp
        Events.h
        Formulation.cpp
        Formulation.h
        LinkedList.h
        LinkedQueue.h
        Mission.cpp
        Mission.h
        MMM.cpp
        Node.h
        Promotion.cpp
        Promotion.h
        Rover.cpp
        Rover.h
        Station.cpp
        Station.h
        testinput1.txt
        testinput2.txt
        testinput3.txt)
