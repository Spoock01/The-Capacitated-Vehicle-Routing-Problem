#include "../include/ConstructiveHeuristic.h"
#include "../include/Helper.h"
#include <iostream>

ConstructiveHeuristic::ConstructiveHeuristic(Graph<int> graph) : m_graph(graph) { }

bool ConstructiveHeuristic::checkNumberRoutes(std::vector<int> route, int vehicles) {
    auto counterRoutes = 0;
    for (unsigned i = 1; i < route.size(); i++)
        if (route[i] == 0)
            ++counterRoutes;

    return counterRoutes == vehicles ? true : false;
}

std::vector<int> ConstructiveHeuristic::buildRoutesByDemand(int capacity, int dimension) {
    auto aux_demands = this->m_graph.getDemands();
    auto aux = 0, higherDemand = 0, visitedCount = 0, load = 0; 
    std::vector<bool> visitedVertex(dimension, false);
    std::vector<int> route;
    bool changed;

    route.push_back(0);
    visitedVertex[0] = true;

    while (visitedCount < dimension -1) {
        higherDemand = 0;
        changed = false;
        for (unsigned i = 0; i < aux_demands.size(); i++) {
            if (!visitedVertex[i] && aux_demands[i].getClientDemand() > higherDemand) {
                if ((load + aux_demands[i].getClientDemand()) <= capacity) {
                    higherDemand = aux_demands[i].getClientDemand();
                    aux = i;
                    changed = true;
                }
            }
        }

        if (changed) {
            route.push_back(aux);
            load += aux_demands[aux].getClientDemand();
            visitedVertex[aux] = true;
            ++visitedCount;
        } else {
            route.push_back(0);
            load = 0;
        }
    }
    route.push_back(0);

    return route;
}

std::vector<int> ConstructiveHeuristic::nearestNeighbor(int capacity, int dimension, int vehicles) {
    // std::cout << this->m_graph;
    std::vector<bool> visitedVertex(dimension, false);
    auto shortestRoute = 0, visitedCount = 0, vertex = 0, aux = 0, load = 0;
    std::vector<int> route;
    bool changed;

    route.push_back(0);
    visitedVertex[0] = true;

    while (visitedCount < dimension - 1)
    {
        shortestRoute = 999;
        changed = false;

        for (int j = 0; j < dimension; j++)
        {
            if (j != vertex && this->m_graph.fetchEdge(vertex, j) < shortestRoute &&
                !visitedVertex[j])
            {
                if ((load + this->m_graph.fetchDemandByClient(j)) <= capacity)
                {
                    shortestRoute = this->m_graph.fetchEdge(vertex, j);
                    aux = j;
                    changed = true;
                }
            }
        }

        if (changed) {
            route.push_back(aux);
            vertex = aux;
            load += this->m_graph.fetchDemandByClient(vertex);
            visitedVertex[aux] = true;
            ++visitedCount;
        } else {
            route.push_back(0);
            vertex = 0;
            load = 0;
        }
    }
    route.push_back(0);

    if (!checkNumberRoutes(route, vehicles))
        route = buildRoutesByDemand(capacity, dimension);

    printRouteAndDistance(route, getDistance(route, this->m_graph));
    // std::cout << "BEFORE: ";
    // printVector(route, true);
    // changingRoutes(separateRoutes(route));

    // route.clear();
    return route;
}