#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "definitions.h"
using namespace std;

//The functions used to perform the algorithm as described in the documentation are defined here.

void starting_values(int* s, int* k, int* x_robot, int* y_robot)
{
    time_t t;
    srand((unsigned) time(&t)); //randomize the next numbers

    //Get s:
    *s = rand() % 101; //A value between 0 and 100
    *s += 50; //Now we have a value between 50 and 150
    cout<<"Anzahl der Schritte bzw. Gesamtladungszahl: "<<*s<<endl;

    //Get k:
    int lowest_possible_value = sqrt(*s) + 1; //Lowest value allowed for k
    *k = rand() % (*s/5-lowest_possible_value+1); //Same algorithm as above
    *k += lowest_possible_value; //Now the have a value between the two borders
    cout<<"Brettlaenge: "<<*k<<endl;

    //Get x_robot:
    *x_robot = (rand() % *k); //Value between 0 and k-1 - the allowed coordinates
    cout<<"X-Koordinate der Startposition des Roboters: "<<*x_robot+1<<endl;

    //Get y_robot:
    *y_robot = (rand() % *k);
    cout<<"Y-Koordinate der Startposition des Roboters: "<<*y_robot+1<<endl;
}

void init_game_board(vector<vector<field>>* game_board, int k) //This function initializes the game_board vector at the beginning
{
    for(unsigned int n = 0; n < k; n++)
    {
        for(unsigned int a = 0; a < k; a++)
        {
            //The variables are set on default values
            (*game_board)[n][a].battery = false;
            (*game_board)[n][a].charge = 0;
        }
    }
}

int weighted_probabilities(int the_upper_bound)
{
    time_t t;
    srand((unsigned) time(&t)); //Randomize the following numbers

    if(the_upper_bound == 0)
    {
        return 1;
    }

    int g_1_75 = the_upper_bound * 1.75; //casts the product into an integer
    int g_1_5 = the_upper_bound * 1.5;
    int number = (rand() % g_1_75) + 1; //Chooses a number

    if(number <= the_upper_bound)
    {
        return number;
    }
    else if(the_upper_bound == number)
    {
        return 1; //Avoids returning 0
    }
    else if(the_upper_bound < number && number < g_1_5)
    {
        return number-the_upper_bound;
    }
    else if(the_upper_bound == g_1_5)
    {
        return 2; //Avoids returning 0
    }
    else
    {
        return number-g_1_5;
    }
    return 0;
}

void pathfinder(int* x_robot, int* y_robot, vector<vector<field>>* game_board, int steps_to_make, position* ultimate_position, position* penultimate_position,
                bool* dead_end, int s, int* x_actual_battery, int* y_actual_battery, int k)
{
    //This function tries to go a way with a given number of steps
    time_t t;
    srand((unsigned) time(&t));
    *dead_end = false;

    vector<direction> possible_directions(0); //vector for the 0-4 the robot can use
    while(steps_to_make > 0 && !*dead_end) //There are not made enough steps yet
    {
        steps_to_make--;
        if(*y_robot >= 1 && !((*game_board)[*x_robot][*y_robot-1].battery))
        {
            possible_directions.push_back(Up); //The field above the actual position is free
        }
        if(*y_robot <= k - 2 && !((*game_board)[*x_robot][*y_robot+1].battery))
        {
            possible_directions.push_back(Down); //The field beneath the actual position is free
        }
        if(*x_robot <= k - 2 && !((*game_board)[*x_robot+1][*y_robot].battery))
        {
            possible_directions.push_back(Right); //The field to the right of the actual position is free
        }
        if(*x_robot >= 1 && !((*game_board)[*x_robot-1][*y_robot].battery))
        {
            possible_directions.push_back(Left); //The field to the left the actual position is free
        }

        if(possible_directions.size() == 0)
        {
            //The robot is in an dead end!!!
            int rest_charge = s; //The charge which has to be placed on the penultimate battery
            if(!(*game_board)[*x_robot][*y_robot].battery) //A battery is lying on the actual position
            {
                rest_charge += (*game_board)[*x_robot][*y_robot].charge;
                (*game_board)[*x_robot][*y_robot].charge = 0;
                (*game_board)[*x_robot][*y_robot].battery = false;
            }
            rest_charge += (*game_board)[ultimate_position->x][ultimate_position->y].charge;
            (*game_board)[ultimate_position->x][ultimate_position->y].charge = 0;
            (*game_board)[ultimate_position->x][ultimate_position->y].battery = false;

            (*game_board)[penultimate_position->x][penultimate_position->y].charge += rest_charge;
            //The rest of the charge is now placed on the penultimate battery field
            *dead_end = true;
        }
        else //A step can be made
        {
            //The direction the robot has to use:
            int direction_to_use = rand() % possible_directions.size();
            switch(possible_directions[direction_to_use])
            {
            case Up:
                *y_robot -= 1;
                break;
            case Down:
                *y_robot += 1;
                break;
            case Left:
                *x_robot -= 1;
                break;
            case Right:
                *x_robot += 1;
                break;
            }
        }
        possible_directions.clear();
    }
    //The right number of steps is completed now
    penultimate_position->x = ultimate_position->x;
    penultimate_position->y = ultimate_position->y;
    ultimate_position->x = *x_actual_battery;
    ultimate_position->y = *y_actual_battery;
}

int difficulty(int s, int k, int b, vector<vector<field>> game_board) //This function evaluates the difficulty as described in the documentation
{
    int sum_rho = 0;
    int rho = 0;
    for(int a = 0; a < k; a++)
    {
        for(int b = 0; b < k; b++)
        {
            if(game_board[a][b].battery)
            {
                rho = 1;
                if(a >= 1)
                {
                    if(game_board[a-1][b].battery)
                    {
                        rho++;
                    }
                }
                if(a <= k-2)
                {
                    if(game_board[a+1][b].battery)
                    {
                        rho++;
                    }
                }
                if(b >= 1)
                {
                    if(game_board[a][b-1].battery)
                    {
                        rho++;
                    }
                }
                if(b <= k-2)
                {
                    if(game_board[a][b+1].battery)
                    {
                        rho++;
                    }
                }
            }
            sum_rho += rho;
            rho = 0;
        }
    }
    int q = (3*s*sum_rho)/k;
    return q;
}

#endif // FUNCTIONS_H_INCLUDED
