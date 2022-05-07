#include <iostream>
#include "definitions.h"
#include "functions.h"
#include <vector>
#include<bits/stdc++.h>
using namespace std;

int main()
{
    //The global variables are declared like in the documentation:
    int b; //Number of batteries on the field (the battery of the robot is included)
    int k; //Length of the board
    int l; //sum of the charges of all batteries

    battery* batteries; //Array for the batteries (the battery of the robot has the highest index
    init_vars(&b, &k, &l, batteries); //Initializes the variables

    vector<vector<bool>> board(k, vector<bool> (k)); //array standing for the board, every field is true if a battery is lying on it
    init_board(batteries, &board, k, b); //Initialize the board-variable
    eva_paths(batteries, b, board, l, k); //evaluate the lengths of the shortest paths between the batteries

    string solution = ""; //This string stores the solution if such a solution exists
    bool found_solution = false; //Do we already have a solution?
    cout<<endl<<"Die Berechnung der Loesung wird gestartet..."<<endl;
    solve(solution, &solution, batteries[b-1].l, b-1, batteries, b, l, &found_solution); //Evaluates the solution
    cout<<"Die Loesung wurde berechnet."<<endl;
    cout<<"Geben Sie den Namen der Ausgabedatei an: ";
    string name_output_file;
    cin>>name_output_file;
    ofstream output_file;
    output_file.open(name_output_file, ios_base::out);
    output_to_file(&output_file, found_solution, solution, batteries);

    return 0;
}
