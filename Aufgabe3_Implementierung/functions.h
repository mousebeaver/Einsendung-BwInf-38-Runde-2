#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include "definitions.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
using namespace std;

int ConvertStringToInt(string s)
{
    stringstream iss;
    iss.clear();
    iss<<s;
    int i;
    iss>>i;
    return i;
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

string ConvertCharToString(char c)
{
    stringstream iss;
    iss.clear();
    iss<<c;
    string s;
    iss>>s;
    return s;
}

int index_of_crossing(crossing node, vector<crossing>* nodes) //returns index of the crossing "node" n the container "nodes"
{
    for(unsigned int n = 0; n < nodes->size(); n++)
    {
        if(node.x == (*nodes)[n].x && node.y == (*nodes)[n].y)
        {
            return n;
        }
    }
    return -1;
}

//Overload the function above for coordinates instead of a crossing:
int index_of_crossing(int x_coordinate, int y_coordinate, vector<crossing>* nodes) //returns index of the crossing "node" n the container "nodes"
{
    crossing node;
    node.x = x_coordinate;
    node.y = y_coordinate;
    for(unsigned int n = 0; n < nodes->size(); n++)
    {
        if(node.x == (*nodes)[n].x && node.y == (*nodes)[n].y)
        {
            return n;
        }
    }
    return -1;
}


void initialize_graph(vector<crossing>* nodes, double**& edges, crossing* start, crossing* goal, int* number_of_edges_pointer, int* x_max, int* y_max) //Initializes the graph which is represented by edges and nodes
{
    cout<<"Geben Sie den Namen der Ausgabedatei an: ";
    string filename = "";
    cin>>filename;
    ifstream input_file;
    input_file.open(filename, ios_base::in);
    if(input_file.fail()) //The input_file does not exist
    {
        cout<<"Die angegebene Datei konnte nicht gefunden werden!"<<endl;
        exit(0);
    }

    string part; //Needed for reading out of the file
    char trash; //Needed for reading out of the file

    int number_of_edges; //Number of the edges which has to be read out of the file
    string number_of_edges_as_string = ""; //Stores the number above as string
    input_file.get(trash);
    while(trash != '\n')
    {
        //Reading the first line
        part = ConvertCharToString(trash);
        number_of_edges_as_string += part;
        input_file.get(trash);
    }
    //The number in the first line is now stored in a string
    number_of_edges = ConvertStringToInt(number_of_edges_as_string);
    *number_of_edges_pointer = number_of_edges;
    //The number of edges is now given

    //Now the coordinates of the first and the last crossing have to be read:
    string input = ""; //Stores a certain input

    //Reading the first node:
    input_file.get(trash); //Getting the "("
    input_file.get(trash);
    while(trash != ',')
    {
        part = ConvertCharToString(trash);
        input += part;
        input_file.get(trash);
    }
    start->x = ConvertStringToInt(input);
    input.clear();
    input_file.get(trash);
    while(trash != ')')
    {
        part = ConvertCharToString(trash);
        input += part;
        input_file.get(trash);
    }
    start->y = ConvertStringToInt(input);
    input.clear();
    input_file.get(trash); //"\n"
    input_file.get(trash); //"("
    input_file.get(trash);
    while(trash != ',')
    {
        part = ConvertCharToString(trash);
        input += part;
        input_file.get(trash);
    }
    goal->x = ConvertStringToInt(input);
    input.clear();
    input_file.get(trash);
    while(trash != ')')
    {
        part = ConvertCharToString(trash);
        input += part;
        input_file.get(trash);
    }
    goal->y = ConvertStringToInt(input);
    input_file.get(trash); //"\n"

    //Read all of the streets:
    //But with this, we have a problem: we do not no the number of nodes!
    //So we have to deal with the streets in another way:
    vector<vector<bool>> streets_existing (0, vector<bool> (0));
    //This 2d-vector represents the matrix which will be saved in the 2d-array "edges" later
    //Instead of integers for the lengths of the streets, it just stores bool-values: true, if a street exists, and false else
    for(int n = 0; n < number_of_edges; n++)
    {
        crossing one_end;
        crossing other_end; //The two nodes of the edge

        //Initialize the two nodes:
        input_file.get(trash); //"("
        input_file.get(trash);
        input.clear();
        while(trash != ',')
        {
            part = ConvertCharToString(trash);
            input += part;
            input_file.get(trash);
        }
        one_end.x = ConvertStringToInt(input);
        input.clear();
        input_file.get(trash);
        while(trash != ')')
        {
            part = ConvertCharToString(trash);
            input += part;
            input_file.get(trash);
        }
        one_end.y = ConvertStringToInt(input);
        input.clear();
        input_file.get(trash); //" "
        input_file.get(trash); //"("
        input_file.get(trash);
        while(trash != ',')
        {
            part = ConvertCharToString(trash);
            input += part;
            input_file.get(trash);
        }
        other_end.x = ConvertStringToInt(input);
        input.clear();
        input_file.get(trash);
        while(trash != ')')
        {
            part = ConvertCharToString(trash);
            input += part;
            input_file.get(trash);
        }
        other_end.y = ConvertStringToInt(input);
        input.clear();
        input_file.get(trash); //"\n"
        //The two nodes are initialized now!

        //Are the already contained in'the array "nodes"?)
        bool one_end_contained = false;
        bool other_end_contained = false; //false = not contained in the array, true = they are contained
        for(unsigned int a = 0; a < nodes->size(); a++)
        {
            if(one_end.x == (*nodes)[a].x && one_end.y == (*nodes)[a].y)
            {
                one_end_contained = true;
            }
            if(other_end.x == (*nodes)[a].x && other_end.y == (*nodes)[a].y)
            {
                other_end_contained = true;
            }
        }
        if(!one_end_contained) //This node is not contained yet
        {
            nodes->push_back(one_end); //Save the node
            streets_existing.push_back(vector<bool> (streets_existing.size())); //Add additional space to the vector
            for(unsigned int b = 0; b < streets_existing.size()-1; b++)
            {
                streets_existing[streets_existing.size()-1][b] = false; //Set all values on false
            }
            for(unsigned int b = 0; b < streets_existing.size(); b++)
            {
                streets_existing[b].push_back(false); //Add needed space per node
            }
        }

        if(!other_end_contained)
        {
            nodes->push_back(other_end); //Save the node
            streets_existing.push_back(vector<bool> (streets_existing.size())); //Add additional space to the vector
            for(unsigned int b = 0; b < streets_existing.size()-1; b++)
            {
                streets_existing[streets_existing.size()-1][b] = false; //Set all values on false
            }
            for(unsigned int b = 0; b < streets_existing.size(); b++)
            {
                streets_existing[b].push_back(false); //Add needed space per node
            }
        }

        //Needed space is added now, now the streets can be saved in the vector
        streets_existing[index_of_crossing(one_end, nodes)][index_of_crossing(other_end, nodes)] = true;
        streets_existing[index_of_crossing(other_end, nodes)][index_of_crossing(one_end, nodes)] = true;

    }
    //With the vector of nodes and the vector "streets_existing", we can initialize the matrix:
    edges = new double*[nodes->size()];
    for(unsigned int a = 0; a < nodes->size(); a++)
    {
        edges[a] = new double[nodes->size()];
    }

    for(unsigned int a = 0; a < nodes->size(); a++)
    {
        for(unsigned int b = 0; b < nodes->size(); b++)
        {
            if(streets_existing[a][b])
            {
                //Evaluating the distance between the two nodes:
                edges[a][b] = sqrt(amount((*nodes)[a].x - (*nodes)[b].x)*amount((*nodes)[a].x - (*nodes)[b].x) + amount((*nodes)[a].y - (*nodes)[b].y)*amount((*nodes)[a].y - (*nodes)[b].y));
            }
            else
            {
                //no street between the two points
                edges[a][b] = -1;
            }
        }
    }

    //Find the biggest coordinates:
    for(unsigned int n = 0; n < nodes->size(); n++)
    {
        if((*nodes)[n].x > *x_max)
        {
            *x_max = (*nodes)[n].x;
        }
    }
    for(unsigned int n = 0; n < nodes->size(); n++)
    {
        if((*nodes)[n].y > *y_max)
        {
            *y_max = (*nodes)[n].y;
        }
    }
}

bool crossing_in_queue(crossing node, vector<crossing>* nodes, vector<int>* queue_dijkstra) //returns true, if the crossing node is part of the vector queue
{
    for(unsigned int n = 0; n < queue_dijkstra->size(); n++)
    {
        if((*nodes)[(*queue_dijkstra)[n]].x == node.x && (*nodes)[(*queue_dijkstra)[n]].y == node.y)
        {
            return true;
        }
    }
    //If this line is reached, "node" is no part of the queue!
    return false;
}

int number_of_turning_processes(vector<int>* way, vector<crossing>* nodes)
{
    //This function takes a way and the vector of nodes and returns the number of turning processes
    if(way->size() == 1)
    {
        return 0; //Avoid errors for an nearly empty way
    }
    int number_turning_processes = 0;
    for(unsigned int n = 0; n < way->size()-2; n++)
    {
        //Go through the points
        //We pick three in a row in every round. This three points make up two stretches. We compare their angel to the x-axis.
        //If these to angels are equal, there is now turning process. But we do not need to determine itself.
        //It is enough to compare the quotients of the cathets:
        crossing point1, point2, point3; //The three points
        point1.x = (*nodes)[(*way)[n]].x;
        point1.y = (*nodes)[(*way)[n]].y;
        point2.x = (*nodes)[(*way)[n+1]].x;
        point2.y = (*nodes)[(*way)[n+1]].y;
        point3.x = (*nodes)[(*way)[n+2]].x;
        point3.y = (*nodes)[(*way)[n+2]].y;

        double quotient1, quotient2; //The quotients of the stretches
        //We have to avoid a floating point exception (by dividing through 0)
        if((point1.y - point2.y) != 0 && (point2.y - point3.y) != 0)
        {
            quotient1 = (point1.x - point2.x)/(point1.y - point2.y);
            quotient2 = (point2.x - point3.x)/(point2.y - point3.y);
            if(quotient1 != quotient2)
            {
                //A turning process is given!
                number_turning_processes++;
            }
        }
        else if(((point1.y - point2.y) == 0 && (point2.y - point3.y) != 0) || ((point1.y - point2.y) != 0 && (point2.y - point3.y) == 0))
        {
            //The angels are not equal!
            number_turning_processes++;
        }
    }
    return number_turning_processes;
}

double length_of_way(double** edges, vector<crossing>* nodes, vector<int>* way)
{
    //This function returns the length of a given way
    double length = 0;
    for(unsigned int n = 0; n < way->size()-1; n++)
    {
        length += edges[index_of_crossing((*nodes)[(*way)[n]], nodes)][index_of_crossing((*nodes)[(*way)[n+1]], nodes)];
    }
    return length;
}

double dijkstra_shortest(vector<int>* way, double** edges, vector<crossing>* nodes, crossing starting_node, crossing goal_node)
{
    //It returns the length of the way and stores the way itself via pointer
    vector<int> queue_dijkstra (0); //This is needed for the Dijkstra-algorithm as described in the documentation
    double costs[nodes->size()]; //Stores the costs for every node
    int index_starting_node = index_of_crossing(starting_node, nodes);
    //Initialize the array costs:
    for(unsigned int n = 0; n < nodes->size(); n++)
    {
        costs[n] = 1000000000; //Higher values are not expected
    }
    //Now, the way has to be stored as a vector for every node
    vector<int> ways[nodes->size()];
    for(unsigned int n = 0; n < nodes->size(); n++)
    {
        ways[n].erase(ways[n].begin(), ways[n].end()); //Clear the vector
    }
    //Put the starting node in the way of the starting node:
    ways[index_starting_node].push_back(index_starting_node);


    costs[index_starting_node] = 0; //costs for starting node have to be 0
    queue_dijkstra.push_back(index_starting_node); //Put the starting node in the queue
    vector<int>::iterator it; //Used for erasing elements in the container
    //Preparation is done, now start the process:
    while(!queue_dijkstra.empty())
    {
        //Take the first node in the queue:
        crossing first_node;
        first_node.x = (*nodes)[queue_dijkstra[0]].x;
        first_node.y = (*nodes)[queue_dijkstra[0]].y;
        //pushing all of the nodes one place forward:
        for(unsigned int n = 0; n < queue_dijkstra.size()-1; n++)
        {
            queue_dijkstra[n] = queue_dijkstra[n+1];
        }
        it = queue_dijkstra.end(); //Sets the iterator on the last element of the vector
        it--;
        queue_dijkstra.erase(it); //Erase the last (unneeded) element

        for(unsigned int n = 0; n < nodes->size(); n++)
        {
            //Going through the edges to which the node is belonging:
            if(edges[index_of_crossing(first_node, nodes)][n] > -1 && n != ways[index_of_crossing(first_node, nodes)][ways[index_of_crossing(first_node, nodes)].size()-2])
            {
                //An edge to the node n is existing and n is not visited yet in this way
                if(costs[index_of_crossing(first_node, nodes)] + edges[index_of_crossing(first_node, nodes)][n] < costs[n])
                {
                    //Way above the actual edge is lower than the old costs of the node
                    costs[n] = costs[index_of_crossing(first_node, nodes)] + edges[index_of_crossing(first_node, nodes)][n]; //Actualise the costs
                    if(!crossing_in_queue((*nodes)[n], nodes, &queue_dijkstra))
                    {
                        //The node is not in the queue
                        queue_dijkstra.push_back(index_of_crossing((*nodes)[n], nodes));
                    }
                    //Actualize the way:
                    ways[n].erase(ways[n].begin(), ways[n].end());
                    for(unsigned int a = 0; a < ways[index_of_crossing(first_node, nodes)].size(); a++)
                    {
                        ways[n].push_back(ways[index_of_crossing(first_node, nodes)][a]);
                    }
                    ways[n].push_back(n);
                }
            }
        }
    }

    //Now, the costs for every node is evaluated!
    //Store the way:
    way->erase(way->begin(), way->end());
    for(unsigned int n = 0; n < ways[index_of_crossing(goal_node, nodes)].size(); n++)
    {
        way->push_back(ways[index_of_crossing(goal_node, nodes)][n]);
    }
    //Return the costs:
    return costs[index_of_crossing(goal_node, nodes)];
}

double dijkstra_minimal_turning(vector<int>* way, double** edges, vector<crossing>* nodes, crossing starting_node, crossing goal_node)
{
    //This function determines the shortest way on which Bilal has to turn as little as possible
    //It returns the lengths of the way and stores the way itself via pointer
    //All in all, it is the same algorithm as implemented in the function for the shortest way
    //Only the calculation of the costs differs and more ways has to be stored and handled
    vector<int> queue_dijkstra (0); //This is needed for the Dijkstra-algorithm as described in the documentation
    int costs[nodes->size()]; //Stores the costs for every node
    int index_starting_node = index_of_crossing(starting_node, nodes);
    //Initialize the array costs:
    for(unsigned int n = 0; n < nodes->size(); n++)
    {
        costs[n] = 1000000000; //Higher values are not expected
    }
    //Now, the way has to be stored as a vector of vectors for every node.
    vector<vector<int>> ways[nodes->size()];
    for(unsigned int n = 0; n < nodes->size(); n++)
    {
        ways[n].erase(ways[n].begin(), ways[n].end()); //Clear the vector
    }
    //Put the starting node in the way of the starting node:
    ways[index_starting_node].push_back(vector<int> (0));
    ways[index_starting_node][0].push_back(index_starting_node);


    costs[index_starting_node] = 0; //costs for starting node have to be 0
    queue_dijkstra.push_back(index_starting_node); //Put the starting node in the queue
    vector<int>::iterator it; //Used for erasing elements in the container
    //Preparation is done, now start the process:
    while(!queue_dijkstra.empty())
    {
        //Take the first node in the queue:
        crossing first_node;
        first_node.x = (*nodes)[queue_dijkstra[0]].x;
        first_node.y = (*nodes)[queue_dijkstra[0]].y;
        //pushing all of the nodes one place forward:
        for(unsigned int n = 0; n < queue_dijkstra.size()-1; n++)
        {
            queue_dijkstra[n] = queue_dijkstra[n+1];
        }
        it = queue_dijkstra.end(); //Sets the iterator on the last element of the vector
        it--;
        queue_dijkstra.erase(it); //Erase the last (unneeded) element

        for(unsigned int c = 0; c < ways[index_of_crossing(first_node, nodes)].size(); c++)
        {
            //Going through the ways to this point - handling each one as it's own
            for(unsigned int n = 0; n < nodes->size(); n++)
            {
                //Going through the edges to which the node is belonging:
                bool visited_as_last_node = false;
                for(unsigned int b = 0; b < ways[index_of_crossing(first_node, nodes)].size(); b++)
                {
                    if(ways[index_of_crossing(first_node, nodes)][b][ways[index_of_crossing(first_node, nodes)][b].size()-2] == n)
                    {
                        visited_as_last_node = true;
                    }
                }
                if(edges[index_of_crossing(first_node, nodes)][n] > -1 && !visited_as_last_node)
                {
                    //An edge to the node n is existing and n is not visited yet in this way
                    //Evaluating the new costs for a potential way:
                    vector<int> potential_way(0);
                    for(unsigned int a = 0; a < ways[index_of_crossing(first_node, nodes)][c].size(); a++)
                    {
                        potential_way.push_back(ways[index_of_crossing(first_node, nodes)][c][a]);
                    }
                    potential_way.push_back(n); //The potential new way is initialized now!
                    if(number_of_turning_processes(&potential_way, nodes) < costs[n]) //Here seems to be an error!!!
                    {
                        //Costs of the way above the actual edge are lower than the old costs of the node
                        costs[n] = number_of_turning_processes(&potential_way, nodes); //Actualise the costs
                        if(!crossing_in_queue((*nodes)[n], nodes, &queue_dijkstra))
                        {
                            //The node is not in the queue
                            queue_dijkstra.push_back(index_of_crossing((*nodes)[n], nodes));
                        }
                        //Actualize the way:
                        ways[n].erase(ways[n].begin(), ways[n].end());
                        ways[n].push_back(potential_way);
                    }
                    else if(number_of_turning_processes(&potential_way, nodes) == costs[n])
                    {
                        //Both edges have same costs
                        //Add the way to the vector of ways:
                        ways[n].push_back(potential_way);
                    }
                }
            }
        }
    }

    //Now, the costs for every node is evaluated!
    //Store the way:
    way->erase(way->begin(), way->end());
    for(unsigned int n = 0; n < ways[index_of_crossing(goal_node, nodes)][0].size(); n++)
    {
        way->push_back(ways[index_of_crossing(goal_node, nodes)][0][n]);
    }
    //Return the length of the way:
    return length_of_way(edges, nodes, &ways[index_of_crossing(goal_node, nodes)][0]);
}

vector<int> way_between_two_nodes(vector<int>* original_way, int node1, int node2) //the nodes node1 and node2 has to be in correct order
{
    //This function takes a way and returns the peace between to given points
    vector<int> way_to_return (0);

    bool part_of_way = false; //true, if the given node is part of the way to return
    for(unsigned int n = 0; n < original_way->size(); n++)
    {
        if((*original_way)[n] == node1)
        {
            //Begin of the way to return
            part_of_way = true;
        }
        if(part_of_way)
        {
            way_to_return.push_back((*original_way)[n]);
        }
        if((*original_way)[n] == node2)
        {
            //End of the way to return
            part_of_way = false;
        }
    }

    return way_to_return;
}

void prepare_graph(double** edges_original, vector<crossing>* nodes_original, double**& edges_new, double**& matrix_turning_processes, int index_goal_node,
                   int starting_node, int x_max, int y_max, vector<int>**& bridges)
{
    //This function prepares the graph for the next steps
    const int number_of_nodes_original = nodes_original->size(); //This value stores the former number of nodes
    bool checked_nodes[number_of_nodes_original][number_of_nodes_original]; //Store which pairs of points are already checked (just stores the originals)
    for(int n = 0; n < number_of_nodes_original; n++)
    {
        for(int a = 0; a < number_of_nodes_original; a++)
        {
            checked_nodes[n][a] = false; //No pair of points is checked yet
        }
    }
    //Initialize the vector matrix_turning_processes
    matrix_turning_processes = new double*[number_of_nodes_original];
    for(int n = 0; n < number_of_nodes_original; n++)
    {
        matrix_turning_processes[n] = new double[number_of_nodes_original];
    }
    for(int n = 0; n < number_of_nodes_original; n++)
    {
        for(int a = 0; a < number_of_nodes_original; a++)
        {
            matrix_turning_processes[n][a] = 0;
        }
    }
    //For the edges, we need at first an array which is transferred to an array later
    edges_new = new double*[number_of_nodes_original];
    bridges = new vector<int>*[number_of_nodes_original];
    for(int n = 0; n < number_of_nodes_original; n++)
    {
        edges_new[n] = new double[number_of_nodes_original];
        bridges[n] = new vector<int>[number_of_nodes_original];
        for(int  a = 0; a < number_of_nodes_original; a++)
        {
            vector<int> possible_way(0);
            possible_way.push_back(n);
            possible_way.push_back(a);
            bridges[n][a] = possible_way;
        }
    }
    //Initialize the array above
    for(int n = 0; n < number_of_nodes_original; n++)
    {
        for(int a = 0; a < number_of_nodes_original; a++)
        {
            edges_new[n][a] = edges_original[n][a];
        }
    }

    bool part_of_a_chain[nodes_original->size()]; //Is a node a part of a chain
    for(unsigned int n = 0; n < nodes_original->size(); n++)
    {
        part_of_a_chain[n] = false; //No node is identified as a part of a chain yet.
    }
    int time_since_last_node = 1; //for the calculation of the edges
    //The necessary saving structures are defined!!!
    for(int n = 0; n < number_of_nodes_original; n++)
    {
        for(int a = n+1; a < number_of_nodes_original; a++)
        {
            //Go through all pairs of points
            vector<int> pair_points_way_between (0); //The way with smallest number of turning processes between the pair of the points
            dijkstra_minimal_turning(&pair_points_way_between, edges_original, nodes_original, (*nodes_original)[n], (*nodes_original)[a]); //Initialize the vector above
            if(!checked_nodes[n][a] && pair_points_way_between.size() > 2 && number_of_turning_processes(&pair_points_way_between, nodes_original) == 0)
            {
                //The pair is not checked yet, they are not neighbours and they are in a chain
                //Save all nodes of the chain
                //First the nodes between n and a:
                vector<int> chain (0);
                for(unsigned int b = 0; b < pair_points_way_between.size(); b++)
                {
                    chain.push_back(pair_points_way_between[b]);
                    for(unsigned int c = 0; c < chain.size(); c++)
                    {
                        //Mark the pairs as checked
                        checked_nodes[pair_points_way_between[b]][chain[c]] = true;
                        checked_nodes[chain[c]][pair_points_way_between[b]] = true;
                    }
                }
                //Now the nodes which belong to the chain, but are not between the nodes n and a
                //We now that the coordinates has to be whole number
                //Due to this, we evaluate pitch of the chain and check the next points on this straight with whole coordinates
                int delta_x = (*nodes_original)[a].x - (*nodes_original)[n].x;
                int delta_y = (*nodes_original)[a].y - (*nodes_original)[n].y;
                //Look for the orientation of the chain
                if(delta_x < 0 && delta_y < 0)
                {
                    time_since_last_node = 1;
                    //Next point at the n-side:
                    crossing next_node;
                    next_node.x = (*nodes_original)[n].x - delta_x;
                    next_node.y = (*nodes_original)[n].y - delta_y;
                    while(next_node.x <= x_max && next_node.y <= y_max)
                    {
                        //A node exists at the right place
                        if(index_of_crossing(next_node, nodes_original) != -1 && edges_original[index_of_crossing(next_node, nodes_original)][index_of_crossing(next_node.x + delta_x*time_since_last_node, next_node.y + delta_y*time_since_last_node, nodes_original)] != -1)
                        {
                            chain.push_back(index_of_crossing(next_node, nodes_original));
                            for(unsigned int d = 0; d < chain.size(); d++)
                            {
                                checked_nodes[chain[d]][index_of_crossing(next_node, nodes_original)] = true;
                                checked_nodes[index_of_crossing(next_node, nodes_original)][chain[d]] = true; //The pair is checked
                            }
                            time_since_last_node = 1;
                        }
                        else
                        {
                            time_since_last_node++;
                        }
                        //Next (possible) point
                        next_node.x -= delta_x;
                        next_node.y -= delta_y;
                    }
                    //Next point at the a-side:
                    next_node.x = (*nodes_original)[n].x + delta_x;
                    next_node.y = (*nodes_original)[n].y + delta_y;
                    while(next_node.x >= 0 && next_node.y >= 0)
                    {
                        //A node exists at the right place
                        if(index_of_crossing(next_node, nodes_original) != -1 &&
                           edges_original[index_of_crossing(next_node, nodes_original)][index_of_crossing(next_node.x - delta_x*time_since_last_node, next_node.y - delta_y*time_since_last_node, nodes_original)] != -1)
                        {
                            chain.push_back(index_of_crossing(next_node, nodes_original));
                            for(unsigned int d = 0; d < chain.size(); d++)
                            {
                                checked_nodes[chain[d]][index_of_crossing(next_node, nodes_original)] = true;
                                checked_nodes[index_of_crossing(next_node, nodes_original)][chain[d]] = true; //The pair is checked
                                time_since_last_node = 1;
                            }
                        }
                        else
                        {
                            time_since_last_node++;
                        }
                        //Next (possible) point
                        next_node.x += delta_x;
                        next_node.y += delta_y;
                    }
                }
                else if(delta_x > 0 && delta_y < 0)
                {
                    time_since_last_node = 1;
                    //Next point at the n-side:
                    crossing next_node;
                    next_node.x = (*nodes_original)[n].x + delta_x;
                    next_node.y = (*nodes_original)[n].y - delta_y;
                    while(next_node.x <= x_max && next_node.y <= y_max)
                    {
                        //A node exists at the right place
                        if(index_of_crossing(next_node, nodes_original) != -1 && edges_original[index_of_crossing(next_node, nodes_original)][index_of_crossing(next_node.x - delta_x*time_since_last_node, next_node.y + delta_y*time_since_last_node, nodes_original)] != -1)
                        {
                            chain.push_back(index_of_crossing(next_node, nodes_original));
                            for(unsigned int d = 0; d < chain.size(); d++)
                            {
                                checked_nodes[chain[d]][index_of_crossing(next_node, nodes_original)] = true;
                                checked_nodes[index_of_crossing(next_node, nodes_original)][chain[d]] = true; //The pair is checked
                            }
                            time_since_last_node = 1;
                        }
                        else
                        {
                            time_since_last_node++;
                        }
                        //Next (possible) point
                        next_node.x += delta_x;
                        next_node.y -= delta_y;
                    }
                    //Next point at the a-side:
                    next_node.x = (*nodes_original)[n].x - delta_x;
                    next_node.y = (*nodes_original)[n].y + delta_y;
                    while(next_node.x >= 0 && next_node.y >= 0)
                    {
                        //A node exists at the right place
                        if(index_of_crossing(next_node, nodes_original) != -1 &&
                           edges_original[index_of_crossing(next_node, nodes_original)][index_of_crossing(next_node.x + delta_x*time_since_last_node, next_node.y - delta_y*time_since_last_node, nodes_original)] != -1)
                        {
                            chain.push_back(index_of_crossing(next_node, nodes_original));
                            for(unsigned int d = 0; d < chain.size(); d++)
                            {
                                checked_nodes[chain[d]][index_of_crossing(next_node, nodes_original)] = true;
                                checked_nodes[index_of_crossing(next_node, nodes_original)][chain[d]] = true; //The pair is checked
                                time_since_last_node = 1;
                            }
                        }
                        else
                        {
                            time_since_last_node++;
                        }
                        //Next (possible) point
                        next_node.x -= delta_x;
                        next_node.y += delta_y;
                    }
                }
                else if(delta_x > 0 && delta_y > 0)
                {
                    time_since_last_node = 1;
                    //Next point at the n-side:
                    crossing next_node;
                    next_node.x = (*nodes_original)[n].x + delta_x;
                    next_node.y = (*nodes_original)[n].y + delta_y;
                    while(next_node.x <= x_max && next_node.y <= y_max)
                    {
                        //A node exists at the right place
                        if(index_of_crossing(next_node, nodes_original) != -1 && edges_original[index_of_crossing(next_node, nodes_original)][index_of_crossing(next_node.x - delta_x*time_since_last_node, next_node.y - delta_y*time_since_last_node, nodes_original)] != -1)
                        {
                            chain.push_back(index_of_crossing(next_node, nodes_original));
                            for(unsigned int d = 0; d < chain.size(); d++)
                            {
                                checked_nodes[chain[d]][index_of_crossing(next_node, nodes_original)] = true;
                                checked_nodes[index_of_crossing(next_node, nodes_original)][chain[d]] = true; //The pair is checked
                            }
                            time_since_last_node = 1;
                        }
                        else
                        {
                            time_since_last_node++;
                        }
                        //Next (possible) point
                        next_node.x += delta_x;
                        next_node.y += delta_y;
                    }
                    //Next point at the a-side:
                    next_node.x = (*nodes_original)[n].x - delta_x;
                    next_node.y = (*nodes_original)[n].y - delta_y;
                    while(next_node.x >= 0 && next_node.y >= 0)
                    {
                        //A node exists at the right place
                        if(index_of_crossing(next_node, nodes_original) != -1 &&
                           edges_original[index_of_crossing(next_node, nodes_original)][index_of_crossing(next_node.x + delta_x*time_since_last_node, next_node.y + delta_y*time_since_last_node, nodes_original)] != -1)
                        {
                            chain.push_back(index_of_crossing(next_node, nodes_original));
                            for(unsigned int d = 0; d < chain.size(); d++)
                            {
                                checked_nodes[chain[d]][index_of_crossing(next_node, nodes_original)] = true;
                                checked_nodes[index_of_crossing(next_node, nodes_original)][chain[d]] = true; //The pair is checked
                                time_since_last_node = 1;
                            }
                        }
                        else
                        {
                            time_since_last_node++;
                        }
                        //Next (possible) point
                        next_node.x -= delta_x;
                        next_node.y -= delta_y;
                    }
                }
                else if(delta_x < 0 && delta_y > 0)
                {
                    time_since_last_node = 1;
                    //Next point at the n-side:
                    crossing next_node;
                    next_node.x = (*nodes_original)[n].x - delta_x;
                    next_node.y = (*nodes_original)[n].y + delta_y;
                    while(next_node.x <= x_max && next_node.y <= y_max)
                    {
                        //A node exists at the right place
                        if(index_of_crossing(next_node, nodes_original) != -1 && edges_original[index_of_crossing(next_node, nodes_original)][index_of_crossing(next_node.x + delta_x*time_since_last_node, next_node.y - delta_y*time_since_last_node, nodes_original)] != -1)
                        {
                            chain.push_back(index_of_crossing(next_node, nodes_original));
                            for(unsigned int d = 0; d < chain.size(); d++)
                            {
                                checked_nodes[chain[d]][index_of_crossing(next_node, nodes_original)] = true;
                                checked_nodes[index_of_crossing(next_node, nodes_original)][chain[d]] = true; //The pair is checked
                            }
                            time_since_last_node = 1;
                        }
                        else
                        {
                            time_since_last_node++;
                        }
                        //Next (possible) point
                        next_node.x -= delta_x;
                        next_node.y += delta_y;
                    }
                    //Next point at the a-side:
                    next_node.x = (*nodes_original)[n].x + delta_x;
                    next_node.y = (*nodes_original)[n].y - delta_y;
                    while(next_node.x >= 0 && next_node.y >= 0)
                    {
                        //A node exists at the right place
                        if(index_of_crossing(next_node, nodes_original) != -1 &&
                           edges_original[index_of_crossing(next_node, nodes_original)][index_of_crossing(next_node.x - delta_x*time_since_last_node, next_node.y + delta_y*time_since_last_node, nodes_original)] != -1)
                        {
                            chain.push_back(index_of_crossing(next_node, nodes_original));
                            for(unsigned int d = 0; d < chain.size(); d++)
                            {
                                checked_nodes[chain[d]][index_of_crossing(next_node, nodes_original)] = true;
                                checked_nodes[index_of_crossing(next_node, nodes_original)][chain[d]] = true; //The pair is checked
                                time_since_last_node = 1;
                            }
                        }
                        else
                        {
                            time_since_last_node++;
                        }
                        //Next (possible) point
                        next_node.x += delta_x;
                        next_node.y -= delta_y;
                    }
                }
                else if(delta_x == 0)
                {
                    time_since_last_node = 1;
                    //The pitch is parallel to the Y-axis
                    crossing higher_node;
                    crossing lower_node; //The two ending nodes of the chain
                    //Which of the two ending nodes is higher?
                    if((*nodes_original)[chain[0]].y > (*nodes_original)[chain[chain.size()-1]].y)
                    {
                        higher_node.x = (*nodes_original)[chain[0]].x;
                        higher_node.y = (*nodes_original)[chain[0]].y + 1;
                        lower_node.x = (*nodes_original)[chain[chain.size()-1]].x;
                        lower_node.y = (*nodes_original)[chain[chain.size()-1]].y - 1;

                    }
                    else
                    {
                        lower_node.x = (*nodes_original)[chain[0]].x;
                        lower_node.y = (*nodes_original)[chain[0]].y-1;
                        higher_node.x = (*nodes_original)[chain[chain.size()-1]].x;
                        higher_node.y = (*nodes_original)[chain[chain.size()-1]].y+1;
                    }
                    //The higher node at first:
                    while(higher_node.y <= y_max)
                    {
                        if(index_of_crossing(higher_node, nodes_original) != -1 &&
                           edges_original[index_of_crossing(higher_node.x, higher_node.y - time_since_last_node, nodes_original)][index_of_crossing(higher_node, nodes_original)] != -1)
                        {
                            //A node exists at the right place
                            chain.push_back(index_of_crossing(higher_node, nodes_original));
                            for(unsigned int d = 0; d < chain.size(); d++)
                            {
                                checked_nodes[chain[d]][index_of_crossing(higher_node, nodes_original)] = true;
                                checked_nodes[index_of_crossing(higher_node, nodes_original)][chain[d]] = true; //The pair is checked
                            }
                            time_since_last_node = 1;
                            //Next (possible) point
                        }
                        else
                        {
                            time_since_last_node++;
                        }
                        higher_node.y += 1;
                    }
                    //After that the lower node:
                    while(lower_node.y >= 0)
                    {
                        if(index_of_crossing(lower_node, nodes_original) != -1 &&
                           edges_original[index_of_crossing(lower_node.x, lower_node.y + time_since_last_node, nodes_original)][index_of_crossing(lower_node, nodes_original)] != -1)
                        {
                            //A node exists at the right place
                            chain.push_back(index_of_crossing(lower_node, nodes_original));
                            for(unsigned int d = 0; d < chain.size(); d++)
                            {
                                checked_nodes[chain[d]][index_of_crossing(lower_node, nodes_original)] = true;
                                checked_nodes[index_of_crossing(lower_node, nodes_original)][chain[d]] = true; //The pair is checked
                            }
                            time_since_last_node = 1;
                            //Next (possible) point
                        }
                        else
                        {
                            time_since_last_node++;
                        }
                        lower_node.y -= 1;
                    }
                }
                else if(delta_y == 0)
                {
                    time_since_last_node = 1;
                    //The pitch is parallel to the X-axis
                    crossing node_right;
                    crossing node_left; //The two ending nodes of the chain
                    //Which of the two ending nodes is higher?
                    if((*nodes_original)[chain[0]].x > (*nodes_original)[chain[chain.size()-1]].x)
                    {
                        node_right.x = (*nodes_original)[chain[0]].x + 1;
                        node_right.y = (*nodes_original)[chain[0]].y;
                        node_left.x = (*nodes_original)[chain[chain.size()-1]].x - 1;
                        node_left.y = (*nodes_original)[chain[chain.size()-1]].y;

                    }
                    else
                    {
                        node_left.x = (*nodes_original)[chain[0]].x-1;
                        node_left.y = (*nodes_original)[chain[0]].y;
                        node_right.x = (*nodes_original)[chain[chain.size()-1]].x+1;
                        node_right.y = (*nodes_original)[chain[chain.size()-1]].y;
                    }
                    //The node at the right at first:
                    while(node_right.x <= x_max)
                    {
                        if(index_of_crossing(node_right, nodes_original) != -1 &&
                           edges_original[index_of_crossing(node_right, nodes_original)][index_of_crossing(node_right.x - time_since_last_node, node_right.y, nodes_original)] != -1)
                        {
                            //A node exists at the right place
                            chain.push_back(index_of_crossing(node_right, nodes_original));
                            for(unsigned int d = 0; d < chain.size(); d++)
                            {
                                checked_nodes[chain[d]][index_of_crossing(node_right, nodes_original)] = true;
                                checked_nodes[index_of_crossing(node_right, nodes_original)][chain[d]] = true; //The pair is checked
                            }
                            //Next (possible) point
                            time_since_last_node = 1;
                        }
                        else
                        {
                            time_since_last_node++;
                        }
                        node_right.x += 1;
                    }
                    //After that the node at the left:
                    while(node_left.x >= 0)
                    {
                        if(index_of_crossing(node_left, nodes_original) != -1 &&
                           edges_original[index_of_crossing(node_left, nodes_original)][index_of_crossing(node_left.x + time_since_last_node, node_left.y, nodes_original)] != -1)
                        {
                            //A node exists at the right place
                            chain.push_back(index_of_crossing(node_left, nodes_original));
                            for(unsigned int d = 0; d < chain.size(); d++)
                            {
                                checked_nodes[chain[d]][index_of_crossing(node_left, nodes_original)] = true;
                                checked_nodes[index_of_crossing(node_left, nodes_original)][chain[d]] = true; //The pair is checked
                            }
                            //Next (possible) point
                            time_since_last_node = 1;
                        }
                        else
                        {
                            time_since_last_node++;
                        }
                        node_left.x -= 1;
                    }
                }
                //All nodes of the chain are found now!!!
                for(unsigned int w = 0; w < chain.size(); w++)
                {
                    part_of_a_chain[chain[w]] = false;
                }

                //Remove all edges between the nodes of the chain:
                for(unsigned int x = 0; x < chain.size(); x++)
                {
                    for(unsigned int y = x; y < chain.size(); y++)
                    {
                        if(edges_new[chain[x]][chain[y]] != -1)
                        {
                            edges_new[chain[x]][chain[y]] = -1;
                            edges_new[chain[y]][chain[x]] = -1;
                        }
                    }
                }
                //The chain is removed now by removing its edges:

                //Go through all pairs of points of the chain:
                for(unsigned int h = 0; h < chain.size(); h++)
                {
                    for(unsigned int i = h+1; i < chain.size(); i++)
                    {
                        //Go through the neighbours of h:
                        for(int j = 0; j < number_of_nodes_original; j++)
                        {
                            if(j != chain[h] && edges_new[chain[h]][j] != -1 && edges_new[chain[i]][j] == -1 && edges_original[chain[i]][j] == -1 &&
                               (matrix_turning_processes[chain[h]][j] == 0 || part_of_a_chain[j]) && j != chain[i])
                            {
                                //The neighbour exists and is not itself's neighbour
                                double new_edge; //The length of the new edge
                                new_edge = edges_new[chain[h]][j] + sqrt(((*nodes_original)[chain[h]].x - (*nodes_original)[chain[i]].x)*((*nodes_original)[chain[h]].x - (*nodes_original)[chain[i]].x) +
                                                                         ((*nodes_original)[chain[h]].y - (*nodes_original)[chain[i]].y)*((*nodes_original)[chain[h]].y - (*nodes_original)[chain[i]].y));
                                //Set the new edges on the new length:
                                edges_new[chain[i]][j] = new_edge;
                                edges_new[j][chain[i]] = new_edge;
                                //Actualise the matrix for the number of turning processes
                                matrix_turning_processes[chain[i]][j] = matrix_turning_processes[chain[h]][j] + 1;
                                matrix_turning_processes[j][chain[i]] = matrix_turning_processes[chain[h]][j] + 1;
                                //Actualise the bridges:
                                vector<int> new_bridge(0); //The bridge from i to h
                                for(unsigned int nn = 0; nn < bridges[chain[i]][chain[h]].size(); nn++)
                                {
                                    new_bridge.push_back(bridges[chain[i]][chain[h]][nn]);
                                }
                                //Now the bridge from h to j
                                for(unsigned int nn = 1; nn < bridges[chain[h]][j].size(); nn++)
                                {
                                    new_bridge.push_back(bridges[chain[h]][j][nn]);
                                }
                                bridges[chain[i]][j] = new_bridge;
                                //Now the bridge from j to i:
                                bridges[j][chain[i]] = vector<int> (bridges[chain[i]][j].size());
                                for(int nn = bridges[chain[i]][j].size()-1; nn >= 0; nn--)
                                {
                                    bridges[j][chain[i]][nn] = bridges[chain[i]][j][bridges[chain[i]][j].size()-nn];
                                }
                            }
                        }
                        //Go through the neighbours of i:
                        for(int j = 0; j < number_of_nodes_original; j++)
                        {
                            if(j != chain[i] && edges_new[chain[i]][j] != -1 && edges_new[chain[h]][j] == -1 &&  edges_original[chain[h]][j] == -1 &&
                               (matrix_turning_processes[chain[i]][j] == 0 || part_of_a_chain[j]) && j != chain[h])
                            {
                                //The neighbour exists and is not itself's neighbour
                                double new_edge; //The length of the new edge
                                new_edge = edges_new[chain[i]][j] + sqrt(((*nodes_original)[chain[h]].x - (*nodes_original)[chain[i]].x)*((*nodes_original)[chain[h]].x - (*nodes_original)[chain[i]].x) +
                                                                         ((*nodes_original)[chain[h]].y - (*nodes_original)[chain[i]].y)*((*nodes_original)[chain[h]].y - (*nodes_original)[chain[i]].y));
                                //Set the new edges on the new length:
                                edges_new[chain[h]][j] = new_edge;
                                edges_new[j][chain[h]] = new_edge;
                                //Actualise the matrix for the number of turning processes
                                matrix_turning_processes[chain[h]][j] = matrix_turning_processes[chain[i]][j] + 1;
                                matrix_turning_processes[j][chain[h]] = matrix_turning_processes[chain[i]][j] + 1;
                                //Actualise the bridges:
                                vector<int> new_bridge(0); //The bridge from i to h
                                for(unsigned int nn = 0; nn < bridges[chain[h]][chain[i]].size(); nn++)
                                {
                                    new_bridge.push_back(bridges[chain[h]][chain[i]][nn]);
                                }
                                //Now the bridge from h to j
                                for(unsigned int nn = 1; nn < bridges[chain[i]][j].size(); nn++)
                                {
                                    new_bridge.push_back(bridges[chain[i]][j][nn]);
                                }
                                bridges[chain[h]][j] = new_bridge;
                                //Now the bridge from j to i:
                                bridges[j][chain[h]] = vector<int> (bridges[chain[h]][j].size());
                                for(int nn = bridges[chain[h]][j].size()-1; nn >= 0; nn--)
                                {
                                    bridges[j][chain[h]][nn] = bridges[chain[h]][j][bridges[chain[h]][j].size()-nn];
                                }
                            }
                        }
                    }
                }

                //Remove (possible) connections between the nodes of a chain and add bridges
                for(unsigned int x = 0; x < chain.size(); x++)
                {
                    for(unsigned int y = 0; y < chain.size(); y++)
                    {
                        edges_new[chain[x]][chain[y]] = -1;
                        edges_new[chain[y]][chain[x]] = -1;
                        matrix_turning_processes[chain[x]][chain[y]] = 0;
                        matrix_turning_processes[chain[y]][chain[x]] = 0;
                    }
                }

                for(unsigned int x = 0; x < chain.size()-1; x++)
                {
                    for(unsigned int y = x+1; y < chain.size(); y++)
                    {
                        //bridges:
                        dijkstra_minimal_turning(&bridges[chain[x]][chain[y]], edges_original, nodes_original,(*nodes_original)[chain[x]], (*nodes_original)[chain[y]]);
                        //counterpart:
                        bridges[chain[y]][chain[x]].clear();
                        for(unsigned int z = 1; z <= bridges[chain[x]][chain[y]].size(); z++)
                        {
                            bridges[chain[y]][chain[x]].push_back(bridges[chain[x]][chain[y]][bridges[chain[x]][chain[y]].size()-z]);
                        }
                    }
                }
                //Add edges to the goal node or the starting node if it is in the chain:
                for(unsigned int x = 0; x < chain.size(); x++)
                {
                    if(chain[x] == index_goal_node || chain[x] == starting_node)
                    {
                        for(unsigned int y = 0; y < chain.size(); y++)
                        {
                            edges_new[chain[x]][chain[y]] = sqrt(((*nodes_original)[chain[x]].x - (*nodes_original)[chain[y]].x)*((*nodes_original)[chain[x]].x - (*nodes_original)[chain[y]].x) +
                                                                 ((*nodes_original)[chain[x]].y - (*nodes_original)[chain[y]].y)*((*nodes_original)[chain[x]].y - (*nodes_original)[chain[y]].y));
                            edges_new[chain[y]][chain[x]] = sqrt(((*nodes_original)[chain[x]].x - (*nodes_original)[chain[y]].x)*((*nodes_original)[chain[x]].x - (*nodes_original)[chain[y]].x) +
                                                                 ((*nodes_original)[chain[x]].y - (*nodes_original)[chain[y]].y)*((*nodes_original)[chain[x]].y - (*nodes_original)[chain[y]].y));
                            matrix_turning_processes[chain[y]][chain[x]] = 0;
                            matrix_turning_processes[chain[x]][chain[y]] = 0;
                        }
                        edges_new[chain[x]][chain[x]] = -1;
                        break;
                    }
                }
            }
        }
    }
    //Everything is initialized now!!!
}

vector<int> find_fitting_way(double** matrix_turning_processes, int recent_step, int number_steps_max, vector<vector<int>>* ways, vector<int>* shortest_way,
                             double** edges, vector<crossing>* nodes, vector<int>* number_steps_way, double* length_resulting_way_max, crossing goal_node)
{
    //At first, we look for a way leading to the goal node in the given ways:
    for(unsigned int n = 0; n < ways->size(); n++)
    {
        if((*number_steps_way)[n] == recent_step)
        {
            //Right number of steps
            if((*ways)[n][(*ways)[n].size()-1] == index_of_crossing(goal_node, nodes) && length_of_way(edges, nodes, &(*ways)[n]) < *length_resulting_way_max)
            {
                //We have found the goal!
                return (*ways)[n];
            }
        }
    }

    vector<int> way_to_return(0);
    if(recent_step < number_steps_max)
    {
        //The ways are not too long now!
        vector<vector<int>> new_ways (0); //vector for the next ways
        vector<int> new_number_steps (0); //vector for the number of steps in the vector above
        for(unsigned int n = 0; n < ways->size(); n++)
        {
            //Go through the ways
            if((*number_steps_way)[n] <= recent_step)
            {
                //The numbers of steps to this node is small enough, so we can add new steps
                for(unsigned int a = 0; a < nodes->size(); a++)
                {
                    if(edges[(*ways)[n][(*ways)[n].size()-1]][a] != -1 && a != (*ways)[n].size()-2)
                    {
                        //An edge exists and is not the last visited!
                        vector<int> way_to_add = (*ways)[n]; //The way to add
                        way_to_add.push_back(a);
                        new_ways.push_back(way_to_add);
                        new_number_steps.push_back((*number_steps_way)[n] + matrix_turning_processes[(*ways)[n][(*ways)[n].size()-1]][a] + 1);
                    }
                }
            }
            else
            {
                //The way has to be handled in one of the next steps
                new_ways.push_back((*ways)[n]);
                new_number_steps.push_back((*number_steps_way)[n]);
            }
        }
        //We have produced the new ways, but no one reaches the goal node
        //Reaching the next step:
        way_to_return = find_fitting_way(matrix_turning_processes, recent_step+1, number_steps_max, &new_ways, shortest_way, edges, nodes, &new_number_steps, length_resulting_way_max, goal_node);
    }
    else
    {
        //The shortest way has to be returned, no way with a smaller number of turning processes is short enough
        return *shortest_way;
    }
    return way_to_return;
}

vector<int> prepare_way(vector<int>* way_to_prepare, vector<int>** bridges)
{
    vector<int> way_to_return (0);
    for(unsigned int n = 0; n < way_to_prepare->size() - 1; n++)
    {
        //Go through the pairs of points
        for(unsigned int a = 0; a < bridges[(*way_to_prepare)[n]][(*way_to_prepare)[n+1]].size()-1; a++)
        {
            way_to_return.push_back(bridges[(*way_to_prepare)[n]][(*way_to_prepare)[n+1]][a]);
        }
    }
    //Last node:
    way_to_return.push_back((*way_to_prepare)[way_to_prepare->size()-1]);
    return way_to_return;
}

void output_to_file(string file_name, vector<int>* resulting_way, double** edges, vector<crossing>* nodes, vector<int>* shortest_way, double max_length,
                    double shortest_length, int percent_extension, crossing start, crossing goal, vector<int>** bridges)
{
    ofstream file;
    file.open(file_name, ios_base::out);
    *resulting_way = prepare_way(resulting_way, bridges);

    file<<"Die durch den Nutzer gegebene maximale Verlängerung beträgt "<<percent_extension<<"%."<<endl;
    file<<"Der kürzeste Weg hat eine Länge von "<<shortest_length<<"."<<endl;
    file<<"Der ausgegeben Weg darf also eine maximale Länge von "<<max_length<<" haben."<<endl<<endl;

    vector<int> way_minimal_turning (0);
    dijkstra_minimal_turning(&way_minimal_turning, edges, nodes, start, goal);
    file<<"Der Weg mit der geringsten Anzahl an Abbiegevorgängen hat eine Länge von "<<length_of_way(edges, nodes, &way_minimal_turning);
    file<<" und "<<number_of_turning_processes(&way_minimal_turning, nodes)<<" Abbiegevorgänge."<<endl;
    file<<"Der kürzeste Weg hat "<<number_of_turning_processes(shortest_way, nodes)<<" Abbiegevorgänge."<<endl<<endl;

    file<<"Beim für Bilal empfohlenen Weg werden folgende Knoten in dieser Reihenfolge besucht: "<<endl;
    for(unsigned int n = 0; n < resulting_way->size(); n++)
    {
        file<<"("<<(*nodes)[(*resulting_way)[n]].x<<","<<(*nodes)[(*resulting_way)[n]].y<<")"<<endl;
    }
}

#endif // FUNCTIONS_H_INCLUDED
