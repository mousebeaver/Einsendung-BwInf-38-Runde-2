#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED

//In this file, the new data types needed for the algorithm are declared .

struct field //This object represents a field of the board
{
    bool battery; //Is true for a battery lying on the field
    int charge; //Charge of the battery
};

struct position //Stores the position of a battery
{
    int x;
    int y; //coordinates between 0 and k-1
};

enum direction {Up, Down, Left, Right}; //Data type for the possible directions on the board

#endif // DEFINITIONS_H_INCLUDED
