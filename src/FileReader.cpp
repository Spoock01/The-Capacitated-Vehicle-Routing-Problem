#include "../include/FileReader.h"
#include "../include/Helper.h"
#include "../include/Graph.h"
#include "../include/ConstructiveHeuristic.h"

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
            // printVector(allRoutes[randomNumber], true);
            bestRoute = allRoutes[randomNumber];
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

std::vector<int> swapMethod(std::vector<int> mainRoute) {
    auto bestRoute = mainRoute;
    auto bestDistance = getDistance(bestRoute);
    auto changed = true;

    while (changed) {
        changed = false;

        for (auto i = 0; i < (int) bestRoute.size(); i++) {
            bestDistance = getDistance(bestRoute);

            for (auto j = 0; j < (int) bestRoute.size(); j++) {
                if (i != j) {
                    std::swap(bestRoute[i], bestRoute[j]);
                    auto new_distance = getDistance(bestRoute);

                    if (new_distance < bestDistance) {
                        bestDistance = new_distance;
                        changed = true;
                    } else {
                        std::swap(bestRoute[i], bestRoute[j]);
                    }
                }
            }
        }
    }

    return bestRoute;
}

void vnd(std::vector<int> rdm, std::vector<int> opt, std::vector<int> swap) {
    auto rdmDistance = getDistance(rdm);
    auto optDistance = getDistance(opt);
    auto swapDistance = getDistance(swap);

    std::cout << "BEST ";
    if (rdmDistance <= optDistance && rdmDistance <= swapDistance) {
        printRouteAndDistance(rdm, rdmDistance);
    } else if (optDistance <= rdmDistance && optDistance <= swapDistance) {
        printRouteAndDistance(opt, optDistance);
    } else {
        printRouteAndDistance(swap, swapDistance);
    }
}

void printResults(std::string method, std::vector<int> route) {
    std::cout << method << ":\t";
    printVector(route, true);
    std::cout << "Resultado " << method << ": " << getDistance(route) << "\n\n";
}

void changingRoutes(std::vector<std::vector<int>> mainRoute)
{
    std::vector<std::vector<int>> allRoutes;
    std::vector<int> newRoute;
    std::vector<int> optRoute;
    std::vector<int> swapRoute;
    newRoute.push_back(0);
    optRoute.push_back(0);
    swapRoute.push_back(0);

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
        swapRoute = mountRoute(swapRoute, swapMethod(route));
    }

    printResults("RDM", newRoute);
    printResults("OPT", optRoute);
    printResults("SWAP", swapRoute);
    vnd(newRoute, optRoute, swapRoute);

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
    bool changed;

    for (auto i = 0; i < DIMENSION; i++)
    {
        visitedVertex[i] = false;
    }

    route.push_back(0);
    visitedVertex[0] = true;

    while (visitedCount < DIMENSION - 1)
    {
        shortestRoute = 999;
        changed = false;

        for (int j = 0; j < DIMENSION; j++)
        {
            if (j != vertex && g_weightMatrix[vertex][j] < shortestRoute &&
                !visitedVertex[j])
            {
                if ((load + g_weightMatrix[j][j]) <= CAPACITY)
                {
                    shortestRoute = g_weightMatrix[vertex][j];
                    aux = j;
                    changed = true;
                }
            }
        }

        if(changed) {
            route.push_back(aux);
            vertex = aux;
            distance += shortestRoute;
            load += g_weightMatrix[vertex][vertex];
            // std::cout << "load sendo incrementado: " << load << "\n";
            visitedVertex[aux] = true;
            ++visitedCount;
        } else {
            std::cout << "load: " << load << "\n";
            route.push_back(0);
            distance += g_weightMatrix[vertex][0];
            vertex = 0;
            load = 0;
        }
    }
    std::cout << "load: " << load << "\n";
    route.push_back(0);

    distance += g_weightMatrix[vertex][0];
    g_distance = distance;
    printRouteAndDistance(route, getDistance(route));
    // std::cout << "BEFORE: ";
    // printVector(route, true);
    changingRoutes(separateRoutes(route));

    // route.clear();
}

void printCountDemand() {
    int count = 0;
    for (unsigned i = 0; i < demands.size(); i++) {
        count += demands[i].getClientDemand();
    }
    std::cout << "Contador: " << count << " -> " << (count/(double)VEHICLE) << "\n";
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
        // instanceFile.close();
        // printCountDemand();
        // demands.clear();
        // g_opt.clear();
        std::cout << "Criado o grafo\n";
        auto graph = Graph<int>();
        graph.setAdjMatrix(g_weightMatrix);
        graph.setDemands(demands);
        // std::cout << graph;
        auto ch = ConstructiveHeuristic(graph);
        ch.nearestNeighbor(CAPACITY, DIMENSION);
    }
    else
    {
        std::cout << "Error opening file." << std::endl;
    }
}