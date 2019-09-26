#include "../include/FileReader.h"
#include "../include/Helper.h"
#include "../include/Graph.h"
#include "../include/ConstructiveHeuristic.h"
#include "../include/MovementHeuristic.h"
#include "../include/Grasp.h"
// #include <algorithm>
#include <cmath>

#define METHOD_1 0
#define METHOD_2 1
#define METHOD_3 2

int DIMENSION;
int VEHICLE;
int CAPACITY;
int g_distance = 0;
std::ifstream instanceFile;
std::vector<Demand> demands;
std::vector<std::vector<int>> g_weightMatrix;
std::vector<std::vector<int>> g_routes;
std::vector<int> g_opt;

int getDimension() { return DIMENSION; }

int getVEHICLE() { return VEHICLE; }

int getCapacity() { return CAPACITY; }

void splitInteger()
{
    std::string line;
    for (int i = 0; i < 3; i++)
    {
        getline(instanceFile, line);
        char *token = strtok((char *)line.c_str(), " ");
        token = strtok(NULL, " ");

        switch (i)
        {
        case 0:
            DIMENSION = std::stoi(token);
            // std::cout << "DIMENSION: " << DIMENSION << std::endl;
            break;
        case 1:
            VEHICLE = std::stoi(token);
            // std::cout << "VEHICLE: " << VEHICLE << std::endl;
            break;
        case 2:
            CAPACITY = std::stoi(token);
            // std::cout << "CAPACITY: " << CAPACITY << std::endl;
            break;
        default:
            return;
        }
    }
    std::vector<std::vector<int>> aux(DIMENSION, std::vector<int>(DIMENSION, -1));
    g_weightMatrix = aux;
}

void readDemands()
{
    int client, client_demand;
    for (int i = 0; i < DIMENSION; i++)
    {
        instanceFile >> client;
        instanceFile >> client_demand;
        demands.push_back(Demand(client, client_demand));
    }
}

void readMatrix(int type)
{
    std::string line;
    auto aux = -1;

    if(type){
        for (int i = 0; i < DIMENSION; i++)
        {
            for (int j = 0; j < i + 1; j++)
            {
                if (i != j)
                {
                    instanceFile >> aux;
                    g_weightMatrix[i][j] = aux;
                    g_weightMatrix[j][i] = aux;
                }
                else
                {
                    getline(instanceFile, line);
                    g_weightMatrix[i][j] = 0;
                    // g_weightMatrix[i][j] = demands[i].getClientDemand();
                    break;
                }
            }
        }
    }else{
        int vertex, x, y;
        std::vector<std::pair<int, int>> node;

        for (int i = 0; i < DIMENSION; i++)
        {
            instanceFile >> vertex >> x >> y;
            node.push_back(std::make_pair(x, y));
            // instanceFile >> vertex1 >> x1 >> y1;
            // std::cout << "Vertex: " << vertex << " x: " << x << " y: " << y << "\n";
            // std::cout << "Vertex1: " << vertex1 << " x1: " << x1 << " y1: " << y1 << "\n";
        }

        for(auto i = 0; i < DIMENSION; i++){
            for(auto j = 0; j < DIMENSION - 1; j++){
                auto distancia = (int)hypot(node[i].first-node[j].first,node[i].second-node[j].second);

                if(i != j){
                    g_weightMatrix[i][j] = distancia;
                    g_weightMatrix[j][i] = distancia;
                }else
                    g_weightMatrix[i][j] = 0;
            }
        }
        // print2dVector(g_weightMatrix);
        // std::cout << "Acabou";
    }

    
}

void skip(int times)
{
    std::string line;
    for (int i = 0; i < times; i++)
    {
        getline(instanceFile, line);
    }
}

void readFile(std::string file)
{
    std::string line;

    instanceFile.open(file.c_str());

    if (instanceFile.is_open())
    {
        skip(1);        // Skipping NAME
        splitInteger(); // Dimension, VEHICLE, CAPACITY
        skip(1);        // Skipping DEMAND_SECTION line
        readDemands();  // Reading DEMAND_SECTION
        skip(3);        // Skipping DEMAND_SECTION, empty and EDGE_WEIGHT_SECTION

        readMatrix(file.size() == 28 ? 0 : 1); // 28 = 19 + 9

        // nearestNeighbor();
        // printCountDemand();


        auto graph = Graph<int>();
        graph.setAdjMatrix(g_weightMatrix);
        graph.setDemands(demands);
        auto ch = ConstructiveHeuristic(graph);
        auto routess = ch.nearestNeighbor(CAPACITY, DIMENSION, VEHICLE);
        auto mh = MovementHeuristic(graph);
        auto grasp = Grasp(graph, CAPACITY);
        std::vector<int> p; //serve pra nada. BLZ?
        // mh.swapMethod();
        mh.buildRoutesByMethod(splitRoutes(routess));
        // grasp.construction(0.85, p);
        grasp.buildGrasp(DIMENSION);

        //Nunca apagar
        demands.clear();
        g_opt.clear();
        instanceFile.close();
    }
    else
    {
        std::cout << "Error opening file." << std::endl;
    }
}