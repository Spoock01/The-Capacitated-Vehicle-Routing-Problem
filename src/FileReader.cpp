#include "../include/FileReader.h"
#include "../include/Helper.h"

int DIMENSION;
int VEHICLE;
int CAPACITY;
int g_distance = 0;
std::ifstream instanceFile;
std::vector<Demand> demands;
std::vector<std::vector<int>> g_weightMatrix;
std::vector<std::vector<int>> g_routes;

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
                // g_weightMatrix[i][j] = 0;
                g_weightMatrix[i][j] = demands[i].getClientDemand();
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

int getDistance(std::vector<int> route)
{
    auto count = 0;
    for (auto i = 0; i < (int)route.size() - 1; i++)
    {
        count = count + g_weightMatrix[route[i]][route[i + 1]];
    }
    return count;
}

int rand_int(int range)
{
    return (int)rand() % range;
}

void changingRoutes(std::vector<std::vector<int>> mainRoute)
{
    std::vector<std::vector<int>> allRoutes;

    std::cout << "MainRoute Size:" << mainRoute.size() << "\n";
    std::vector<int> g_distance_array(mainRoute.size(), 999);

    // Alterando Rotas
    for (auto i = 0; i < (int)mainRoute.size(); i++)
    {
        std::vector<int> route = mainRoute[i];

        auto end = route.size() - 1;
        auto bestDistance = getDistance(route);
        bestDistance += g_weightMatrix[0][route[0]];
        bestDistance += g_weightMatrix[route[end]][0];
        g_distance_array[i] = bestDistance;
        // std::cout << "Best distance: " << bestDistance << "i: " << i << "\n";

        if (route.size() == 1)
        {
            allRoutes.push_back(route);
        }
        for (auto j = 0; j < (int)route.size(); j++)
        {
            for (auto k = 0; k < (int)route.size(); k++)
            {
                auto saveRoute = route;
                auto aux = saveRoute[j];
                if (k > j)
                {
                    saveRoute[j] = saveRoute[k];
                    saveRoute[k] = aux;
                    allRoutes.push_back(saveRoute);
                }
            }
        }
        // std::cout << "_____________\n";
        // print2dVector(allRoutes);
        // std::cout << "_____________\n";

        auto times = allRoutes.size() / 3;
        times = times < 1 ? 0 : times;

        // Descida aleatoria
        for (auto index = 0; index < (int)times; index++)
        {
            auto randomNumber = rand_int(allRoutes.size());
            auto currentDistance = getDistance(allRoutes[randomNumber]);
            auto var = allRoutes[randomNumber].size() - 1;

            currentDistance += g_weightMatrix[0][allRoutes[randomNumber][0]];
            currentDistance += g_weightMatrix[allRoutes[randomNumber][var]][0];
            // std::cout << randomNumber << " Comparando current: " << currentDistance << "com: " << g_distance_array[i] << std::endl;
            if (currentDistance < g_distance_array[i])
            {
                std::cout << "Truck: #" << i << " This route is better: ";
                printVector(allRoutes[index], true);
                // std::cout << "Best current distance: " << currentDistance << std::endl;
                g_distance_array[i] = currentDistance;
                index = 0;
            }
        }
        allRoutes.clear();
    }

    auto count = 0;

    for (auto var : g_distance_array)
    {
        // std::cout << var << " ";
        count += var;
    }

    std::cout << "\nResultado: " << count << std::endl;
    count = 0;
}

auto separateRoutes(std::vector<int> &route)
{

    std::vector<std::vector<int>> allRoutes;
    std::vector<int> currentRoute;

    for (auto i = 1; i < (int)route.size() - 1; ++i)
    {

        if (route[i] == 0 && route[i] == route[i + 1])
        {
            // std::cout << "End \n";
            allRoutes.push_back(currentRoute);
            currentRoute.erase(currentRoute.begin(), currentRoute.end());
            ++i;
        }
        else
        {
            // std::cout << route[i] << " ";
            currentRoute.push_back(route[i]);
        }
    }
    allRoutes.push_back(currentRoute);
    return allRoutes;
}

void nearestNeighbor()
{
    /*
        visitedVertex: a array to know which vertex has been visited
        distance: to count the distance
        shortestRoute: to know which is the shortest route
        visitedCount: a visited vertex counter to know if all vertex has been
        visited vertex: to know the current vertex load: to count how much has been
        loaded
    */

    auto visitedVertex = new bool[DIMENSION];
    auto shortestRoute = 0, visitedCount = 0, vertex = 0, aux = 0, distance = 0,
         load = 0;
    std::vector<int> route;
    std::vector<std::vector<int>> pauloGuedes;

    for (auto i = 0; i < DIMENSION; i++)
    {
        visitedVertex[i] = false;
    }

    route.push_back(0);

    while (visitedCount < DIMENSION - 1)
    {
        shortestRoute = 999;

        for (int j = 0; j < DIMENSION; j++)
        {
            if (j != vertex && g_weightMatrix[vertex][j] < shortestRoute &&
                !visitedVertex[j])
            {
                if ((load + g_weightMatrix[j][j]) <= CAPACITY)
                {
                    shortestRoute = g_weightMatrix[vertex][j];
                    aux = j;
                }
            }
        }
        route.push_back(aux);
        vertex = aux;
        distance += shortestRoute;
        load += g_weightMatrix[vertex][vertex];

        if (aux != 0)
        {
            visitedVertex[aux] = true;
            ++visitedCount;
        }
        else
        {
            route.push_back(0);
            load = 0;
        }
    }
    route.push_back(0);

    distance += g_weightMatrix[vertex][0];
    g_distance = distance;
    printRouteAndDistance(route, distance);
    changingRoutes(separateRoutes(route));

    // route.clear();
    // print2dVector(pauloGuedes);
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
        nearestNeighbor();
        instanceFile.close();
        demands.clear();
    }
    else
    {
        std::cout << "Error opening file." << std::endl;
    }
}