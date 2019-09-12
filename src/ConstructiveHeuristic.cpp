#include "../include/ConstructiveHeuristic.h"
#include "../include/Helper.h"
#include <iostream>

ConstructiveHeuristic::ConstructiveHeuristic(Graph<int> graph) {
    this->m_graph = graph;
}

int ConstructiveHeuristic::getDistance(std::vector<int> route) {

    auto end = route.size() - 1;
    auto count = 0, bestDistance = 0;

    bestDistance += this->m_graph.fetchEdge(0, route[0]);
    bestDistance += this->m_graph.fetchEdge(route[end], 0);

    for (auto i = 0; i < (int)route.size() - 1; i++)
    {
        count = count + this->m_graph.fetchEdge(route[i], route[i + 1]);
    }

    return count + bestDistance;
}

std::vector<int> ConstructiveHeuristic::nearestNeighbor(int capacity, int dimension) {
    std::cout << this->m_graph;
    auto visitedVertex = new bool[dimension];
    auto shortestRoute = 0, visitedCount = 0, vertex = 0, aux = 0, load = 0;
    std::vector<int> route;
    bool changed;

    for (auto i = 0; i < dimension; i++)
    {
        visitedVertex[i] = false;
    }

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
                if ((load + this->m_graph.fetchDemand(j)) <= capacity)
                {
                    shortestRoute = this->m_graph.fetchEdge(vertex, j);
                    aux = j;
                    changed = true;
                }
            }
        }

        if(changed) {
            route.push_back(aux);
            vertex = aux;
            load += this->m_graph.fetchDemand(vertex);
            // std::cout << "load sendo incrementado: " << load << "\n";
            visitedVertex[aux] = true;
            ++visitedCount;
        } else {
            std::cout << "load: " << load << "\n";
            route.push_back(0);
            vertex = 0;
            load = 0;
        }
    }
    std::cout << "load: " << load << "\n";
    route.push_back(0);

    printRouteAndDistance(route, getDistance(route));
    // std::cout << "BEFORE: ";
    // printVector(route, true);
    // changingRoutes(separateRoutes(route));

    // route.clear();
    return route;
}