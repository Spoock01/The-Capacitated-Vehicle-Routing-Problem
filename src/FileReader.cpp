#include "../include/FileReader.h"
#include "../include/Helper.h"
#include "../include/Graph.h"
#include "../include/ConstructiveHeuristic.h"
#include "../include/MovementHeuristic.h"
#include <algorithm>

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

void readMatrix()
{
    std::string line;
    auto aux = -1;

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
        readMatrix();
        // nearestNeighbor();
        // printCountDemand();


        auto graph = Graph<int>();
        graph.setAdjMatrix(g_weightMatrix);
        graph.setDemands(demands);
        auto ch = ConstructiveHeuristic(graph);
        auto routess = ch.nearestNeighbor(CAPACITY, DIMENSION, VEHICLE);
        auto mh = MovementHeuristic(graph);
        // mh.swapMethod();
        mh.buildRoutesByMethod(splitRoutes(routess));

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