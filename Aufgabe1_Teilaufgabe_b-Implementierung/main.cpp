#include <iostream>
#include "functions.h"
#include "definitions.h"
#include <cmath>
#include<bits/stdc++.h>
using namespace std;

int main()
{
    int s; //Number of steps, equal to the sum of all charges
    int k; //Length of the board
    int x_robot; //the x-coordinate of the robot
    int y_robot; //the y-coordinate of the robot
    starting_values(&s, &k, &x_robot, &y_robot); //Initialize the values above
    int start_x = x_robot;
    int start_y = y_robot;
    int start_s = s;        //Used for output to file
    int x_actual_battery = x_robot;
    int y_actual_battery = y_robot;
    vector<vector<field>> game_board(k, vector<field> (k));
    init_game_board(&game_board, k); //set the values of the vector above to default values

    position ultimate_battery;
    position penultimate_battery; //These positions are made to handle dead ends
    bool dead_end = false; //True if the robot is in a dead end

    while(s > 0 && !dead_end)
    {
        //In this loop, the batteries are placed on the board
        //Choose the value of the charge which has to the actual field:
        int placed_charge = weighted_probabilities(s);
        //Place the charge on the actual field:
        game_board[x_robot][y_robot].charge = placed_charge;
        game_board[x_robot][y_robot].battery = true;
        s -= placed_charge;
        x_actual_battery = x_robot;
        y_actual_battery = y_robot;
        //How many steps have to be made from now on?
        int steps_to_make = weighted_probabilities(placed_charge-1);
        //How often to visit the next battery field and do we want placing another battery-field between the to visits?
        int event; //0-7 for visit the next battery-field once, 8 for visit twice without another battery-field and 9 otherwise

        if(s <= 3 || steps_to_make <= 3)
        {
            //There is not enough charge any more for visiting a battery-field twice or not enough steps!
           event = 0;
        }
        else
        {
            time_t t;
            srand((unsigned) time(&t));
            event = rand() % 10; //Values from 0-9
        }

        //Treat the result of the initialization of "event"
        int charge_to_place_next, x_twice, y_twice, charge_to_place_between; //For case 9
        switch(event)
        {
        case 9: //Visit the next battery field twice with another battery between
            pathfinder(&x_robot, &y_robot, &game_board, steps_to_make, &ultimate_battery, &penultimate_battery, &dead_end, s, &x_actual_battery, &y_actual_battery, k);
            //Walk to next battery
            charge_to_place_next = weighted_probabilities(s/2); //We need enough charge for the way back
            //Place this charge on the actual field:
            game_board[x_robot][y_robot].charge = charge_to_place_next;
            s -= charge_to_place_next;
            game_board[x_robot][y_robot].battery = true;
            x_twice = x_robot;
            y_twice = y_robot;
            //Walk to the next battery between
            pathfinder(&x_robot, &y_robot, &game_board, steps_to_make, &ultimate_battery, &penultimate_battery, &dead_end, s, &x_actual_battery, &y_actual_battery, k);
            charge_to_place_between = weighted_probabilities((s - charge_to_place_next)/2); //Just increase the charge placed on the actual field
            charge_to_place_between *= 2;
            charge_to_place_between += charge_to_place_next; //The difference between the minimal length of the way and the steps has to be even
            game_board[x_robot][y_robot].charge = charge_to_place_between;
            s -= charge_to_place_between;
            game_board[x_robot][y_robot].battery = true;
            //Placed a charge on this field
            x_robot = x_twice;
            y_robot = y_twice; //Go back to last battery
            pathfinder(&x_robot, &y_robot, &game_board, s - steps_to_make, &ultimate_battery, &penultimate_battery, &dead_end, s, &x_actual_battery, &y_actual_battery, k);
            //Way is completed
            break;
        case 8: //Visit the next battery field twice without another battery between
            pathfinder(&x_robot, &y_robot, &game_board, steps_to_make, &ultimate_battery, &penultimate_battery, &dead_end, s, &x_actual_battery, &y_actual_battery, k); //Walk to next battery
            //The battery on the actual field has to be visited twice!
            //Place battery:
            if((x_robot < k-2 && !game_board[x_robot+1][y_robot].battery && !game_board[x_robot+2][y_robot].battery) ||
               (x_robot > 1 && !game_board[x_robot-1][y_robot].battery && !game_board[x_robot-2][y_robot].battery) ||
               (y_robot > 1 && !game_board[x_robot][y_robot-1].battery && !game_board[x_robot][y_robot-2].battery) ||
               (y_robot < k-2 && !game_board[x_robot][y_robot+1].battery && !game_board[x_robot][y_robot+2].battery) ||
               (x_robot > 0 && y_robot > 0 && !game_board[x_robot-1][y_robot].battery && !game_board[x_robot-1][y_robot-1].battery) ||
               (x_robot > 0 && y_robot > 0 && !game_board[x_robot][y_robot-1].battery && !game_board[x_robot-1][y_robot-1].battery)  ||
               (x_robot < k-1 && y_robot > 0 && !game_board[x_robot+1][y_robot].battery && !game_board[x_robot+1][y_robot-1].battery) ||
               (x_robot < k-1 && y_robot > 0 && !game_board[x_robot][y_robot-1].battery && !game_board[x_robot+1][y_robot-1].battery) ||
               (x_robot > 0 && y_robot < k-1 && !game_board[x_robot-1][y_robot].battery && !game_board[x_robot-1][y_robot+1].battery) ||
               (x_robot > 0 && y_robot < k-1 && !game_board[x_robot][y_robot+1].battery && !game_board[x_robot-1][y_robot+1].battery) ||
               (x_robot < k-1 && y_robot < k-1 && !game_board[x_robot+1][y_robot].battery && !game_board[x_robot+1][y_robot+1].battery) ||
               (x_robot < k-1 && y_robot < k-1 && !game_board[x_robot][y_robot+1].battery && !game_board[x_robot+1][y_robot+1].battery))
            {
                //enough space to discharge the battery
                int charge_to_place = weighted_probabilities(s/2); //We need an even charge
                charge_to_place *= 2; //Now, it is even
                game_board[x_robot][y_robot].charge = charge_to_place;
                s -= charge_to_place;
                game_board[x_robot][y_robot].battery = true; //Place the battery on the field
            }
            //Go on!
            pathfinder(&x_robot, &y_robot, &game_board, placed_charge-steps_to_make, &ultimate_battery, &penultimate_battery, &dead_end, s, &x_actual_battery, &y_actual_battery, k);
            break;
        default: //Do not visit the next battery field twice
            pathfinder(&x_robot, &y_robot, &game_board, placed_charge, &ultimate_battery, &penultimate_battery, &dead_end, s, &x_actual_battery, &y_actual_battery, k);
            break;
        }
    }
    //The starting position is evaluated now

    //Output to file:
    ofstream file;
    string name_of_file;
    cout<<endl<<"Bitte geben Sie den Namen der Ausgabedatei an: ";
    cin>>name_of_file;
    file.open(name_of_file, ios_base::out);
    file<<k<<endl;
    file<<start_x+1<<","<<start_y+1<<","<<game_board[start_x][start_y].charge<<endl;

    //count batteries
    int batteries = -1; //-1 for the battery of the robot
    for(int a = 0; a < k; a++)
    {
        for(int b = 0; b < k; b++)
        {
            if(game_board[a][b].battery)
            {
                batteries++;
            }
        }
    }
    file<<batteries<<endl;

    //print batteries
    for(int a = 0; a < k; a++)
    {
        for(int b = 0; b < k; b++)
        {
            if(!(a == start_x && b == start_y) && game_board[a][b].battery)
            {
                file<<a+1<<","<<b+1<<","<<game_board[a][b].charge<<endl;
            }
        }
    }
    int q = difficulty(start_s, k, batteries+1, game_board);
    cout<<"Die Startposition hat einen Schwierigkeitsgrad von "<<q<<endl;
    file<<"Schwierigkeitsgrad: "<<q<<endl;
    cout<<"Das Schreiben in die Ausgabedatei ist beendet."<<endl;
    return 0;
}
