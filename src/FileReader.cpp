#include "../include/FileReader.h"
#include "../include/Helper.h"
#include "../include/Graph.h"
#include "../include/ConstructiveHeuristic.h"
#include "../include/MovementHeuristic.h"
#include "../include/Grasp.h"
#include <time.h>
#include <cmath>

int DIMENSION;
int VEHICLE;
int CAPACITY;
int g_distance = 0;
std::ifstream instanceFile;
std::vector<Demand> demands;
std::vector<std::vector<int>> g_weightMatrix;

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

            break;
        case 1:
            VEHICLE = std::stoi(token);

            break;
        case 2:
            CAPACITY = std::stoi(token);

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

        auto graph = Graph<int>();
        graph.setAdjMatrix(g_weightMatrix);
        graph.setDemands(demands);
        auto ch = ConstructiveHeuristic(graph);

        float time1 = 0.0;
        auto sum1 = 0;

        std::cout << "Greedy\n\n";
        clock_t t1;
        t1 = clock();

        for(auto i = 0; i < 10; i++){
            auto routess = ch.nearestNeighbor(CAPACITY, DIMENSION, VEHICLE);
            auto mh = MovementHeuristic(graph);
            auto route = mh.buildRoutesByMethod(splitRoutes(routess));
            auto dis = getDistance(route, graph, false);
            printRouteAndDistance(route, dis);
            sum1 += dis;
        }

        t1 = clock() - t1;
        time1 = ((float)t1)/CLOCKS_PER_SEC;

        printf ("It took me (%f ms).\n", (time1/10.0)*1000);
        printf("Media de distancia: %d\n", sum1/10);

        /*
            ===================================================================
        */

        float time = 0.0;
        auto sum = 0;
        auto grasp = Grasp(graph, CAPACITY);
    
        std::cout << "\n\n\nGRASP\n\n";
        clock_t t;
        t = clock();

        for(auto i = 0; i < 10; i++)
            sum += grasp.buildGrasp(DIMENSION);

        t = clock() - t;
        time = ((float)t)/CLOCKS_PER_SEC;

        printf ("It took me (%f ms).\n", (time/10.0)*1000);
        printf("Media de distancia: %d\n", sum/10);

        //Nunca apagar
        demands.clear();
        instanceFile.close();
    }
    else
    {
        std::cout << "Error opening file." << std::endl;
    }
}