#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include "definitions.h"
using namespace std;

//Here we define some important functions

string ReadToChar(ifstream* file, char border) //writes all chars of the file in a string until the border-char is detected
{
    string s = "";
    string part = "";
    stringstream iss;
    iss.clear();
    char trash;
    file->get(trash);
    while(trash != border)
    {
        iss<<trash;
        iss>>part;
        s += part;
        iss.clear();
        part.clear();
        file->get(trash);
    }
    return s;
}

int amount(int i) //Returns the amount of a number
{
    if(i < 0)
    {
        return -i;
    }
    else
    {
        return i;
    }
}

int ConvertStringToInt(string s)
{
    stringstream iss;
    iss.clear();
    iss<<s;
    int i;
    iss>>i;
    return i;
}

void init_board(battery* batteries, vector<vector<bool>>* board, int k, int b)
{
    for(int n = 0; n < k; n++)
    {
        for(int m = 0; m < k; m++)
        {
            (*board)[m][n] = false; // false = no battery lying on this field
        }
    }

    for(int n = 0; n < b; n++)
    {
        (*board)[batteries[n].x-1][batteries[n].y-1] = true;
    }
}

void init_vars(int* b, int* k, int* l, battery*& batteries)
{
    cout<<"Name der zu verarbeitenden Datei: ";
    string name_of_input_file;
    cin>>name_of_input_file;
    ifstream file; //the input file
    file.open(name_of_input_file, ios_base::in);
    if(file.fail())
    {
        cout<<"Die angegebene Datei konnte leider nicht gefunden werden."<<endl;
        exit(0);
    }
    string input; //Contains the input
    int x_robot;
    int y_robot;
    int l_robot; //The variables of the robot can not be saved instantly after reading

    input = ReadToChar(&file, '\n'); //Read k (the variables are named like in the documentation)
    *k = ConvertStringToInt(input); //Set k
    cout<<"\nDie Brettlaenge betraegt:\t"<<*k<<endl;
    input = ReadToChar(&file, ','); //Read x of the robot
    x_robot = ConvertStringToInt(input);
    cout<<"Die X-Koordinate der Startposition des Roboters ist:\t"<<x_robot<<endl;
    input = ReadToChar(&file, ','); //Read y of the robot
    y_robot = ConvertStringToInt(input);
    cout<<"Die Y-Koordinate der Startposition des Roboters ist:\t"<<y_robot<<endl;
    input = ReadToChar(&file, '\n'); //Read l of the robot
    l_robot = ConvertStringToInt(input);
    cout<<"Die Ladung der Bordbatterie des Roboters ist zu Beginn:\t"<<l_robot<<endl;
    input = ReadToChar(&file, '\n'); //Read b
    *b = ConvertStringToInt(input)+1; //+1 for the battery of the robot
    cout<<"Anzahl der auf dem Feld liegenden Batterien (inklusive Bordbatterie):\t"<<*b<<endl;
    batteries = new battery[(*b)];

    for(int n = 0; n < (*b)-1; n++)
    {
        cout<<"Werte von Batterie Nr."<<n+1<<":\t";
        input = ReadToChar(&file, ','); //x-coordinate
        batteries[n].x = ConvertStringToInt(input);
        cout<<input<<", ";
        input = ReadToChar(&file, ','); //y-coordinate
        batteries[n].y = ConvertStringToInt(input);
        cout<<input<<", ";
        input = ReadToChar(&file, '\n'); //l
        batteries[n].l = ConvertStringToInt(input);
        cout<<batteries[n].l<<endl;
    }
    batteries[(*b)-1].x = x_robot;
    batteries[(*b)-1].y = y_robot;
    batteries[(*b)-1].l = l_robot; //Now all readable variables are read
    file.close();
    //Evaluate l:
    *l = 0;
    for(int n = 0; n < *b; n++)
    {
        *l += batteries[n].l;
    }
    cout<<"Der Gesamtladungswert der Batterien (inklusive Bordbatterie) betraegt:\t"<<*l<<endl<<endl;
}

int min_length(position goal, vector<position> positions, vector<vector<bool>> board, int length, int s, int k)
{
    //This recursive function evaluates the minimal length of a path between two batteries
    if(length == 1 && goal.x == positions[0].x && goal.y == positions[0].y)
    {
        return 0; //The path begins and ends with the same battery
    }
    if(length == 1 && ((goal.x == positions[0].x+1 && goal.y == positions[0].y) ||
           (goal.x == positions[0].x-1 && goal.y == positions[0].y) ||
           (goal.x == positions[0].x && goal.y == positions[0].y+1) ||
           (goal.x == positions[0].x && goal.y == positions[0].y-1)))
           {
               return 1; //The goal is next to the starting position
           }
    /*for(int n = 0; n < positions.size(); n++)
    {
        cout<<endl<<"Feld Nr. "<<n+1<<": ("<<positions[n].x<<", "<<positions[n].y<<")";
    }
    cin.get();*/
    vector<position> new_positions (0); //Vector for the new positions

    for(int n = 0; n < positions.size(); n++)
    {
        //Adding all reachable fields for the given positions
        if(positions[n].origin != Up && positions[n].y > 1 && !board[positions[n].x-1][positions[n].y-2] )
        {
            position position1;
            position1.x = positions[n].x;
            position1.y = positions[n].y-1;
            board[positions[n].x-1][positions[n].y-2] = true; //Do not visit one field twice
            position1.origin = Down;
            new_positions.push_back(position1);
        }
        if(positions[n].origin != Down && positions[n].y <= k && !board[positions[n].x-1][positions[n].y])
        {
            position position2;
            position2.x = positions[n].x;
            position2.y = positions[n].y+1;
            board[positions[n].x-1][positions[n].y] = true;
            position2.origin = Up;
            new_positions.push_back(position2);
        }
        if(positions[n].origin != Left && positions[n].x > 1 && !board[positions[n].x-2][positions[n].y-1])
        {
            position position3;
            position3.x = positions[n].x-1;
            position3.y = positions[n].y;
            board[positions[n].x-2][positions[n].y-1] = true;
            position3.origin = Right;
            new_positions.push_back(position3);
        }
        if(positions[n].origin != Right && positions[n].x < k && !board[positions[n].x][positions[n].y-1])
        {
            position position4;
            position4.x = positions[n].x+1;
            position4.y = positions[n].y;
            board[positions[n].x][positions[n].y-1] = true;
            position4.origin = Left;
            new_positions.push_back(position4);
        }
    }
    //Now are all fields which can be reached with "length" steps in the vector "new_positions"

    if(new_positions.size() == 0 || length > s)
    {
        //No new field can be reached or the path is too long!
        return 2*s;
    }

    //Do we have reached the goal?
    for(int n = 0; n < new_positions.size(); n++)
    {
        if((goal.x == new_positions[n].x+1 && goal.y == new_positions[n].y) ||
           (goal.x == new_positions[n].x-1 && goal.y == new_positions[n].y) ||
           (goal.x == new_positions[n].x && goal.y == new_positions[n].y+1) ||
           (goal.x == new_positions[n].x && goal.y == new_positions[n].y-1))
        {
            //We have reached the goal!
            return length+1;
        }
    }

    //If we reach this part of the function, the goal is not reached yet
    return min_length(goal, new_positions, board, length+1, s, k);
}

void eva_paths(battery*& batteries, int b, vector<vector<bool>> board, int s, int k) //evaluates the minimal lengths of the paths between the batteries
{
    cout<<endl<<"Berechnen der Mindestlaengen zwischen den Pfaden...";
    for(int n = 0; n < b-1; n++) //goes through all batteries except the battery of robot
    {
        batteries[n].paths = new path[b-1];//New path variable
        for(int a = 0; a < b-1; a++)
        {
            batteries[n].paths[a].index = a; //Index of the goal

            position goal; //The battery we want to reach
            goal.x = batteries[a].x;
            goal.y = batteries[a].y;
            goal.origin = Undefined;
            vector<position> positions(0); //Actual position
            position actual_position;
            actual_position.x = batteries[n].x;
            actual_position.y = batteries[n].y;
            actual_position.origin = Undefined;
            positions.push_back(actual_position);

            batteries[n].paths[a].min_length = min_length(goal, positions, board, 1, s, k);//length of the path
        }
    }
    //For the battery of the robot:
    batteries[b-1].paths = new path[b-1];
    for(int n = 0; n < b-1; n++) //Evaluates the paths for the robot
    {
        batteries[b-1].paths[n].index = n;

        //Works like the initialization of the other paths above
        position goal;
        goal.x = batteries[n].x;
        goal.y = batteries[n].y;
        goal.origin = Undefined;
        vector<position> positions(0);
        position actual_position;
        actual_position.x = batteries[b-1].x;
        actual_position.y = batteries[b-1].y;
        actual_position.origin = Undefined;
        positions.push_back(actual_position);

        batteries[b-1].paths[n].min_length = min_length(goal, positions, board, 1, s, k);
    }//The reason for separating the battery of the robot from the others is the fact, that at the fields of the other batteries will be a battery every time.
    cout<<endl<<"Die Mindestlaengen der Pfade wurden den Batterien sind berechnet."<<endl;
}

string index_to_4digitstring(int index) //makes an index to a string containing for digits
{
    string digit4_index = to_string(index);
    switch(digit4_index.length())
    {
    case 1:
        digit4_index = "000" + digit4_index;
        break;
    case 2:
        digit4_index = "00" + digit4_index;
        break;
    case 3:
        digit4_index = "0" + digit4_index;
        break;
    case 4:
        //Nothing has to be done
        break;
    }
    //digit4_index += " ";
    return digit4_index;
}

void solve(string solution, string* StringSavingSolution, int charge, int index, battery* batteries, int b, int l, bool* found_solution) //This recursive function tries to find a solution
{
    /*
    For storing the path of the robot, we encode this path in a string.
    This string stores every visited battery field, represented by the battery lying on it.
    Every field is clearly named after its index in the batteries-array. The index of a battery
    is written as a number with 4 digits, situations with more than 9999 batteries are not expected.
    After the index of a battery, a 4-digit-number stores the number of steps added to the minimal length of
    the path between the two batteries.
    The last 4-digit-number in the string stores the charge of the battery of the robot after the last battery exchange.
    If the situation can not be solved, the string remains empty.
    */

    if(l > charge) //There are still loaded batteries the battery of the robot
    {
        //The functions goes through the possible paths
        for(int n = 0; n < b-1 && !*found_solution; n++)
        {
            //The functions goes through all possible lengths of paths
            for(int a = batteries[index].paths[n].min_length; a <= charge && !*found_solution; a += 2)
            {
                if(a == 0 && charge > 1)
                {
                    a = 2; //Avoid remaining on one field
                }
                else if((a == 0 && charge == 1) || charge == 0)
                {
                    break; //Avoids -1 as charge
                }
                solution += index_to_4digitstring(batteries[index].paths[n].index);
                solution += index_to_4digitstring(a); //adds the necessary data to the string
                int new_charge = batteries[batteries[index].paths[n].index].l; // new charge of the battery of the robot
                //cout<<"l: "<<l<<", charge: "<<charge<<", index: "<<index<<", n: "<<n<<", a: "<<a<<", new_charge: "<<new_charge<<", charge-a: "<<charge-a<<endl;
                int l_beforeexchange = new_charge; //Saves the charge of the visited battery
                batteries[batteries[index].paths[n].index].l = charge - a; //Exchange the battery
                //Now the function calls herself:
                solve(solution, StringSavingSolution, new_charge, batteries[index].paths[n].index, batteries, b, l-a, found_solution);
                solution = solution.substr(0, solution.length()-8);
                batteries[batteries[index].paths[n].index].l = l_beforeexchange; //cancel the changes of the steps before

            }
        }
    }
    else
    {
        //We found a solution!!!
        solution += index_to_4digitstring(charge);
        cout<<"Solution: "<<solution<<endl;
        *found_solution = true;
        *StringSavingSolution = solution;
    }
}

void output_to_file(ofstream* file, bool solution_existing, string solution, battery* batteries)
{
    if(!solution_existing)
    {
        *file<<"Für die gegebene Batterieverteilung gibt es keine Lösung."<<endl;
    }
    else
    {
        *file<<"Für die gegeben Batterieverteilung gibt es eine Lösung!"<<endl<<endl;
        *file<<"Von seinem Startfeld aus bewegt sich der Roboter folgendermaßen: "<<endl;
        while(solution.size() > 4)
        {
            //Number of steps:
            stringstream iss;
            iss.clear();
            string number_as_a_string = solution.substr(0, 4);
            int number_as_integer;
            iss<<number_as_a_string;
            iss>>number_as_integer;
            iss.clear();
            *file<<"Der Roboter läuft zur Batterie auf dem Feld ("<<batteries[number_as_integer].x<<","<<batteries[number_as_integer].y<<") mit ";
            solution.erase(0, 4);
            //To the battery:
            number_as_a_string = solution.substr(0, 4);
            iss<<number_as_a_string;
            iss>>number_as_integer;
            iss.clear();
            *file<<number_as_integer<<" Schritten."<<endl;
            solution.erase(0, 4);
        }
        stringstream iss;
        iss.clear();
        string number_as_a_string = solution.substr(0, 4);
        int number_as_integer;
        iss<<number_as_a_string;
        iss>>number_as_integer;
        iss.clear();
        *file<<"Die verbleibende Restladung beträgt "<<number_as_integer<<"."<<endl;
    }
}

#endif // FUNCTIONS_H_INCLUDED
