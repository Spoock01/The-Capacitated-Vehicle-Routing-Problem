#include "../include/FileReader.h"
#include "../include/Helper.h"

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

    auto end = route.size() - 1;
    auto count = 0, bestDistance = 0;

    bestDistance += g_weightMatrix[0][route[0]];
    bestDistance += g_weightMatrix[route[end]][0];

    for (auto i = 0; i < (int)route.size() - 1; i++)
    {
        count = count + g_weightMatrix[route[i]][route[i + 1]];
    }

    return count + bestDistance;
}

int rand_int(int range)
{
    return (int)rand() % range;
}

std::vector<int> mountRoute(std::vector<int> route, std::vector<int> routeToAppend)
{
    for (auto i : routeToAppend)
    {
        route.push_back(i);
    }

    route.push_back(0);
    return route;
}

std::vector<int> randomDescentMethod(std::vector<std::vector<int>> &allRoutes,
                                     std::vector<int> &g_distance_array, int i,
                                     std::vector<int> mainRoute)
{
    auto times = allRoutes.size() / 3;
    times = times < 1 ? 0 : times;
    auto bestRoute = mainRoute;

    for (auto index = 0; index < (int)times; index++)
    {
        auto randomNumber = rand_int(allRoutes.size());
        auto currentDistance = getDistance(allRoutes[randomNumber]);

        // std::cout << randomNumber << " Comparando current: " << currentDistance << "com: " << g_distance_array[i] << std::endl;
        if (currentDistance < g_distance_array[i])
        {
            // std::cout << "Truck: #" << i << " This route is better: ";
            // printVector(allRoutes[index], true);
            bestRoute = allRoutes[index];
            // std::cout << "Best current distance: " << currentDistance << std::endl;
            g_distance_array[i] = currentDistance;
            index = 0;
        }
    }
    allRoutes.clear();
    return bestRoute;
}

std::vector<int> two_opt_change(std::vector<int> route, int index, int k_index)
{
    std::vector<int> newRoute;

    // std::cout << "FOR1!!\n";

    for (auto i = 0; i < index; i++)
    {
        // std::cout << "INDICE: " << i << "VALOR: " << route[i] << "\n";
        newRoute.push_back(route[i]);
    }

    // std::cout << "FOR2!!\n";
    for (auto i = k_index; i >= index; i--)
    {
        // std::cout << "INDICE: " << i << "VALOR: " << route[i] << "\n";
        newRoute.push_back(route[i]);
    }
    // std::cout << "FOR3!!\n";
    // std::cout << "K+1: " << (k_index + 1) << " routeSize: " << route.size() << "\n";
    for (auto i = k_index + 1; i < (int)route.size(); i++)
    {
        newRoute.push_back(route[i]);
    }

    // std::cout << "\n\nindex = " << index << " k_index: " << k_index << "\n\n";
    // std::cout << "two opt route: ";
    // printVector(newRoute, true);
    return newRoute;
}

std::vector<int> two_opt(std::vector<int> mainRoute)
{
    auto bestRoute = mainRoute;
    auto bestDistance = getDistance(bestRoute);
    auto changed = true;

    while (changed)
    {
        changed = false;
        // std::cout << "Best Route size: " << bestRoute.size() << "\n";
        for (auto j = 0; j < (int)bestRoute.size(); j++)
        {
            bestDistance = getDistance(bestRoute);
            for (auto k = j + 1; k < (int)bestRoute.size(); k++)
            {
                // std::cout << "Calling change with: J{" << j << "} and K{" << k << "}\n";
                // std::cout << "=={{ROUTE}}== ";
                // printVector(bestRoute, true);
                auto new_route = two_opt_change(bestRoute, j, k);
                auto new_distance = getDistance(new_route);

                if (new_distance < bestDistance)
                {
                    // std::cout << "\n\nNEWROUTE: ";
                    // printVector(new_route, true);
                    // std::cout << "\n\n";
                    changed = true;
                    bestRoute = new_route;
                    bestDistance = new_distance;
                    break;
                }
            }
        }
    }
    g_opt.push_back(getDistance(bestRoute));

    return bestRoute;
}

void changingRoutes(std::vector<std::vector<int>> mainRoute)
{
    std::vector<std::vector<int>> allRoutes;
    std::vector<int> newRoute;
    std::vector<int> optRoute;
    newRoute.push_back(0);
    optRoute.push_back(0);

    // std::cout << "MainRoute Size: " << mainRoute.size() << "\n";
    std::vector<int> g_distance_array(mainRoute.size(), 999);

    // Alterando Rotas
    for (auto i = 0; i < (int)mainRoute.size(); i++)
    {
        std::vector<int> route = mainRoute[i];
        auto bestDistance = getDistance(route);
        g_distance_array[i] = bestDistance;

        if (route.size() == 1)
            allRoutes.push_back(route);

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

        auto result = randomDescentMethod(allRoutes, g_distance_array, i, route);
        newRoute = mountRoute(newRoute, result);
        // std::cout << "BEST ROUTE RECEIVED: ";
        // printVector(result, true);
        optRoute = mountRoute(optRoute, two_opt(route));
    }

    std::cout << "RDM:    ";
    printVector(newRoute, true);

    auto count = 0;

    for (auto var : g_distance_array)
    {
        // std::cout << var << " ";
        count += var;
    }

    std::cout << "\nResultado RDM: " << count << std::endl;
    count = 0;

    std::cout << "OPT:    ";
    printVector(optRoute, true);

    for (auto var : g_opt)
    {
        // std::cout << var << " ";
        count += var;
    }

    std::cout << "\nResultado OPT: " << count << std::endl;
    count = 0;
}

auto separateRoutes(std::vector<int> &route)
{

    std::vector<std::vector<int>> allRoutes;
    std::vector<int> currentRoute;

    for (auto i = 1; i < (int)route.size() - 1; ++i)
    {

        if (route[i] == 0)
        {
            // std::cout << "End \n";
            allRoutes.push_back(currentRoute);
            currentRoute.erase(currentRoute.begin(), currentRoute.end());
            // ++i;
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
            // route.push_back(0);
            load = 0;
        }
    }
    route.push_back(0);

    distance += g_weightMatrix[vertex][0];
    g_distance = distance;
    printRouteAndDistance(route, distance);
    std::cout << "BEFORE: ";
    printVector(route, true);
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
        g_opt.clear();
    }
    else
    {
        std::cout << "Error opening file." << std::endl;
    }
}