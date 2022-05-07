#include <iostream>
#include "definitions.h"
#include "functions.h"
#include <vector>
using namespace std;

int main()
{
    vector<crossing> nodes; //array for the nodes of the graph
    double** edges; //2D-array for the edges of the graph
    int number_of_edges; //Number of edges
    crossing start; //This node is the start node
    crossing goal; //The goal node
    int x_max = 0;
    int y_max = 0; //The biggest coordinates
    initialize_graph(&nodes, edges, &start, &goal, &number_of_edges, &x_max, &y_max);
    //the variables above are initialized now!
    //Evaluating the shortest way:
    vector<int> shortest_way (0);
    double length_of_shortest_way = dijkstra_shortest(&shortest_way, edges, &nodes, start, goal);
    int shortest_way_number_of_turning_processess = number_of_turning_processes(&shortest_way, &nodes);
    //The shortest way is now evaluated!

    //Preparing the graph for the next steps:
    double** edges_prepared; //Stores the new version of the first matrix
    double** matrix_turning_processes;//stores the second matrix of weights
    vector<int>** bridges; //The way for the abbreviations in the matrix for the turning processes
    prepare_graph(edges, &nodes, edges_prepared, matrix_turning_processes, index_of_crossing(goal, &nodes), index_of_crossing(start, &nodes), x_max, y_max, bridges);

    int tp_minimal[nodes.size()]; //Stores the minimal number of turning processes from every node to the goal node
    //Initialize the array above:
    for(unsigned int n = 0; n < nodes.size(); n++)
    {
        vector<int> way_for_one_point (0);
        dijkstra_minimal_turning(&way_for_one_point, edges, &nodes, nodes[n], goal);
        tp_minimal[n] = number_of_turning_processes(&way_for_one_point, &nodes);
    }
    //No, we are ready for trying the ways through the graph!!!
    //Ask the user for the maximal extension:
    double maximal_length_way; //maximal length of the way in units of length
    int maximal_extension_procent; //maximal extension as given by the user
    cout<<"Geben Sie die maximale Verlaengerung des Weges in Prozent an: ";
    cin>>maximal_extension_procent;
    maximal_length_way = length_of_shortest_way * (maximal_extension_procent+100)/100;
    //Every necessary information is given

    vector<int> resulting_way; //This is the way to give out
    vector<vector<int>> ways (0, vector<int>(0)); //Dummy for the function below
    vector<int> start_of_ways (0);
    start_of_ways.push_back(index_of_crossing(start, &nodes));
    ways.push_back(start_of_ways); //Has initialized the vector "ways"
    vector<int> number_steps_way (1); //Also a dummy for "find_fitting"
    number_steps_way[0] = 0;
    resulting_way = find_fitting_way(matrix_turning_processes, 0, shortest_way_number_of_turning_processess, &ways, &shortest_way, edges_prepared, &nodes, &number_steps_way,
                                     &maximal_length_way, goal);

    //Output of the results:
    cout<<"Geben Sie den Namen der Ausgabedatei an: ";
    string output_file_name;
    cin>>output_file_name;
    output_to_file(output_file_name, &resulting_way, edges, &nodes, &shortest_way, maximal_length_way, length_of_shortest_way, maximal_extension_procent, start, goal, bridges);
    return 0;
}
