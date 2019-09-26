#include "../include/MovementHeuristic.h"
#include "../include/Helper.h"
#include "../include/Movement.h"
#include "../include/TwoOpt.h"
#include "../include/RandomDescent.h"
#include "../include/Swap.h"

#define METHOD(x) (x == 0 ? "OPT": x == 1 ? "SWAP" : "RANDOM" )
#define NUMBER_OF_METHODS 3


MovementHeuristic::MovementHeuristic(Graph<int>& graph) : m_graph(graph) { }

void MovementHeuristic::printResultsByMethod(std::vector<int> route) {
    std::cout << "Distancia final: " << getDistance(route, this->m_graph, false) << "\n";
    std::cout << "Rota: ";
    printVector(route, true);
}

std::vector<int> MovementHeuristic::vnd(std::vector<int> route) {

    Movement *methods[NUMBER_OF_METHODS] = {new TwoOpt(this->m_graph),  new Swap(this->m_graph), new RandomDescent(this->m_graph)};
    auto method = 0, currentBestDistance = 0, newDistance = 0;

    currentBestDistance = getDistance(route, this->m_graph, false);

    while(method < NUMBER_OF_METHODS){
 
        newDistance = methods[method]->getMovement(route);

        if(newDistance < currentBestDistance){
            currentBestDistance = newDistance;
            method = 0;
            continue;
        }
        ++method;
    }

    return route;
}


/*
    Chama o VND para cada sub rota
*/

std::vector<int> MovementHeuristic::buildRoutesByMethod(std::vector<std::vector<int>> mainRoute) {

    std::vector<int> newRoute;
    newRoute.push_back(0);

    // Alterando Rotas
    for (auto i = 0; i < (int)mainRoute.size(); i++)
    {
        std::vector<int> route = mainRoute[i];
        newRoute = mountRoute(newRoute, vnd(route));
    }
    
    return newRoute;
}
