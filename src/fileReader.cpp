#include "../include/FileReader.h"

const std::string INSTANCE_PATH = "./instancias_teste/";

int DIMENSION;
int VEHICLE;
int CAPACITY;
std::ifstream instanceFile;
std::vector<Demand> demands;
std::vector<std::vector<int>> weightMatrix(1);

int getDimension()
{
    return DIMENSION;
}

int getVEHICLE()
{
    return VEHICLE;
}

int getCapacity()
{
    return CAPACITY;
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
        std::cout << "Invalid option! File 'P-n19-k2.txt' has been selected as the default option." << std::endl;
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
    for (int i = 0; i < DIMENSION; i++)
    {
        for (int j = 0; j < DIMENSION; j++)
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

void nearestNeighbor()
{
    /*
        visitedVertex: a array to know which vertex has been visited
        distance: to count the distance
        shortestRoute: to know which is the shortest route
        visitedCount: a visited vertex counter to know if all vertex has been visited
        vertex: to know the current vertex
        load: to count how much has been loaded
    */
    bool visitedVertex[DIMENSION];
    int distance = 0;
    int shortestRoute, visitedCount = 0, vertex = 0, aux = 0;
    int load = 0;
    std::vector<int> route;

    for (int i = 0; i < DIMENSION; i++)
    {
        visitedVertex[i] = false;
    }

    route.push_back(0);
    while (visitedCount < DIMENSION - 1)
    {
        shortestRoute = 999;

        for (int j = 0; j < DIMENSION; j++)
        {
            if (j != vertex && weightMatrix[vertex][j] < shortestRoute && !visitedVertex[j])
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
            visitedCount++;
        }
        else
        {
            load = 0;
        }
    }

    route.push_back(0);
    distance += weightMatrix[vertex][0];

    std::cout << "ROUTE: { ";
    for (int i = 0; i < route.size(); i++)
        std::cout << route[i] << " ";
    std::cout << "}\nDistance = " << distance << std::endl;
}

void readFile(std::string file)
{
    std::string line;

    instanceFile.open(file.c_str());

    if (instanceFile.is_open())
    {
        skip(1);        //Skipping NAME
        splitInteger(); // Dimension, VEHICLE, CAPACITY
        skip(1);        // Skipping DEMAND_SECTION line
        readDemands();  // Reading DEMAND_SECTION
        skip(3);        // Skipping DEMAND_SECTION, empty and EDGE_WEIGHT_SECTION
        readMatrix(weightMatrix);
        // show(weightMatrix);
        nearestNeighbor();
        instanceFile.close();
    }
    else
    {
        std::cout << "Error opening file." << std::endl;
    }
}