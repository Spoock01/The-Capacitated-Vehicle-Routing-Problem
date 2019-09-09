#include "../include/FileReader.h"
#include <time.h> 

const std::string INSTANCE_PATH = "./instancias_teste/";

int DIMENSION;
int VEHICLE;
int CAPACITY;
int g_distance = 0;
std::ifstream instanceFile;
std::vector<Demand> demands;
std::vector<std::vector<int>> weightMatrix;

int getDimension() { return DIMENSION; }

int getVEHICLE() { return VEHICLE; }

int getCapacity() { return CAPACITY; }

void printVector(std::vector<int> route)
{
    for (auto i : route)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";
}

std::string getFileName()
{
    int fileNumber = -1;
    std::cin >> fileNumber;

    switch (fileNumber)
    {
    case 1:
        return INSTANCE_PATH + "P-n19-k2.txt";
    case 2:
        return INSTANCE_PATH + "P-n20-k2.txt";
    case 3:
        return INSTANCE_PATH + "P-n23-k8.txt";
    case 4:
        return INSTANCE_PATH + "P-n45-k5.txt";
    case 5:
        return INSTANCE_PATH + "P-n50-k10.txt";
    case 6:
        return INSTANCE_PATH + "P-n51-k10.txt";
    case 7:
        return INSTANCE_PATH + "P-n55-k7.txt";
    case 0:
        exit(0);
    default:
        std::cout << "Invalid option! File 'P-n19-k2.txt' has been selected as the "
                     "default option."
                  << std::endl;
        return INSTANCE_PATH + "P-n19-k2.txt";
    }
}

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
    weightMatrix = aux;
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

void readMatrix(std::vector<std::vector<int>> &weightMatrix)
{
    std::string line;
    // std::cout << "size: " << weightMatrix.size() << std::endl;
    int aux = -1;
    for (int i = 0; i < DIMENSION; i++)
    {
        for (int j = 0; j < i + 1; j++)
        {
            if (i != j)
            {
                instanceFile >> aux;
                weightMatrix[i][j] = aux;
                weightMatrix[j][i] = aux;
            }
            else
            {
                getline(instanceFile, line);
                // weightMatrix[i][j] = 0;
                weightMatrix[i][j] = demands[i].getClientDemand();
                break;
            }
        }
    }
}

void show(std::vector<std::vector<int>> &weightMatrix)
{
    for (int i = 0; i < weightMatrix.size(); i++)
    {
        for (int j = 0; j < weightMatrix[i].size(); j++)
        {
            std::cout << weightMatrix[i][j] << " ";
        }
        std::cout << std::endl;
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

void getRouteAndDistance(std::vector<int> route, int distance)
{
    std::cout << "ROUTE: { ";
    printVector(route);
    std::cout << "}\nDistance = " << distance << std::endl;
}

int getDistance(std::vector<int> route)
{
    auto count = 0;
    for (auto i = 0; i < route.size() - 1; i++)
    {
        count = count + weightMatrix[route[i]][route[i + 1]];
    }
    return count;
}

int rand_int(int range){
    return (int) rand() % range;
}

void changingRoutes(std::vector<std::vector<int>> mainRoute)
{
    std::vector<std::vector<int>> allRoutes;

    std::cout << "MainRoute Size:" << mainRoute.size() << "\n";
    std::vector<int> g_distance_array(mainRoute.size(), 999);

    // Alterando Rotas
    for (int i = 0; i < mainRoute.size(); i++)
    {
        std::vector<int> route = mainRoute[i];

        auto var = route.size() - 1;
        auto bestDistance = getDistance(route);
        bestDistance += weightMatrix[0][route[0]];
        bestDistance += weightMatrix[route[var]][0];
        g_distance_array[i] = bestDistance;
        // std::cout << "Best distance: " << bestDistance << "i: " << i << "\n";

        if(route.size() == 1) {
            allRoutes.push_back(route);
        }
        for (auto j = 0; j < route.size(); j++)
        {
            for (auto k = 0; k < route.size(); k++)
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
        // show(allRoutes);    
        // std::cout << "_____________\n";

        auto times = allRoutes.size()/3;
        times = times < 1 ? 0 : times;

        // Descida aleatoria
        for (auto index = 0; index < times; index++)
        {
            auto randomNumber = rand_int(allRoutes.size());
            // std::cout << "\n\n" ;
            // printVector(allRoutes[randomNumber]);
            // std::cout << "\n\n" ;
            // std::cout << "allRoutes.size() " << allRoutes.size() << std::endl;
            // std::cout << "Random number: " << randomNumber << std::endl;
            auto currentDistance = getDistance(allRoutes[randomNumber]);
            auto var = allRoutes[randomNumber].size() - 1;

            currentDistance += weightMatrix[0][allRoutes[randomNumber][0]];
            currentDistance += weightMatrix[allRoutes[randomNumber][var]][0];
            // std::cout << randomNumber << " Comparando current: " << currentDistance << "com: " << g_distance_array[i] << std::endl;
            if (currentDistance < g_distance_array[i])
            {
                std::cout << "Truck: #" << i <<" This route is better: ";
                printVector(allRoutes[index]);
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
        std::cout << var << " ";
        count += var;
    }

    std::cout << "\nResultado: " << count << std::endl;
    count = 0;
}

auto separateRoutes(std::vector<int> &route)
{

    std::vector<std::vector<int>> allRoutes;
    std::vector<int> currentRoute;

    for (auto i = 1; i < route.size() - 1; ++i)
    {

        if (route[i] == route[i + 1] && route[i] == 0)
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

    // std::cout << "Separate \n\n\n\n";
    // show(allRoutes);
    // std::cout << "++++++++++++++\n\n\n\n";

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
    std::srand(time(NULL));
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
            if (j != vertex && weightMatrix[vertex][j] < shortestRoute &&
                !visitedVertex[j])
            {
                if ((load + weightMatrix[j][j]) <= CAPACITY)
                {
                    shortestRoute = weightMatrix[vertex][j];
                    aux = j;
                }
            }
        }
        route.push_back(aux);
        vertex = aux;
        distance += shortestRoute;
        load += weightMatrix[vertex][vertex];

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

    distance += weightMatrix[vertex][0];
    g_distance = distance;
    getRouteAndDistance(route, distance);
    changingRoutes(separateRoutes(route));

    // route.clear();
    // show(pauloGuedes);
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
        readMatrix(weightMatrix);
        nearestNeighbor();
        instanceFile.close();
        demands.clear();
    }
    else
    {
        std::cout << "Error opening file." << std::endl;
    }
}