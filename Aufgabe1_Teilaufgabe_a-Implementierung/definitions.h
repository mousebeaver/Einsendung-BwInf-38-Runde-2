#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED
#include <vector>

//Some simple data types are defined in this file

struct path //An object which stores the relation between two batteries
{
    int index; //index of the battery which is the goal of the path
    int min_length; //minimal length
};

struct battery //the batteries
{
    int x;
    int y; //coordinates of the battery
    int l; //charge of the battery
    path* paths; //relations to the other batteries
};

enum direction{Up = 0, Down, Right, Left, Undefined}; //The four possible directions and a state for the first step

struct position //An object storing a position on the board
{
    int x; //Starting with (0,0) up to (b-1, b-1)
    int y;
    direction origin; //There the robot came from
};


#endif // DEFINITIONS_H_INCLUDED
