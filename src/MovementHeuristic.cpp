#include "../include/MovementHeuristic.h"
#include "../include/Helper.h"
#include "../include/Movement.h"
#include "../include/TwoOpt.h"
#include "../include/RandomDescent.h"
#include "../include/Swap.h"

#define METHOD(x) (x == 0 ? "OPT": x == 1 ? "SWAP" : "RANDOM" )


MovementHeuristic::MovementHeuristic(Graph<int>& graph) : m_graph(graph) { }

void MovementHeuristic::printResultsByMethod(std::vector<int> route) {
    std::cout << "Distancia final: " << getDistance(route, this->m_graph) << "\n";
    std::cout << "Rota: ";
    printVector(route, true);
}

std::vector<int> MovementHeuristic::randomDescentMethod(std::vector<int> mainRoute) {

    std::vector<std::vector<int>> allRoutes;
    auto times = allRoutes.size() / 3;
    times = times < 1 ? 0 : times;
    auto bestRoute = mainRoute;
    auto bestDistance = getDistance(mainRoute, this->m_graph);

    if (bestRoute.size() == 1)
        allRoutes.push_back(bestRoute);

    for (auto j = 0; j < (int)bestRoute.size(); j++)
    {
        for (auto k = 0; k < (int)bestRoute.size(); k++)
        {
            auto saveRoute = bestRoute;
            auto aux = saveRoute[j];
            if (k > j)
            {
                saveRoute[j] = saveRoute[k];
                saveRoute[k] = aux;
                allRoutes.push_back(saveRoute);
            }
        }
    }


    for (auto index = 0; index < (int)times; index++)
    {
        auto randomNumber = rand_int(allRoutes.size());
        auto currentDistance = getDistance(allRoutes[randomNumber], this->m_graph);

        if (currentDistance < bestDistance)
        {
            bestRoute = allRoutes[randomNumber];
            bestDistance = currentDistance;
            index = 0;
        }
    }
    allRoutes.clear();
    return bestRoute;
}

std::vector<int> MovementHeuristic::two_opt_change(std::vector<int> route, int index, int k_index) {
    std::vector<int> newRoute;

    for (auto i = 0; i < index; i++)
        newRoute.push_back(route[i]);
    
    for (auto i = k_index; i >= index; i--)
        newRoute.push_back(route[i]);
    
    for (auto i = k_index + 1; i < (int)route.size(); i++)
        newRoute.push_back(route[i]);

    return newRoute;
}

std::vector<int> MovementHeuristic::two_opt(std::vector<int> mainRoute) {
    auto bestRoute = mainRoute;
    auto bestDistance = getDistance(bestRoute, this->m_graph);
    auto changed = true;

    while (changed)
    {
        changed = false;
        // std::cout << "Best Route size: " << bestRoute.size() << "\n";
        for (auto j = 0; j < (int)bestRoute.size(); j++)
        {
            bestDistance = getDistance(bestRoute, this->m_graph);
            for (auto k = j + 1; k < (int)bestRoute.size(); k++)
            {
                // std::cout << "Calling change with: J{" << j << "} and K{" << k << "}\n";
                // std::cout << "=={{ROUTE}}== ";
                // printVector(bestRoute, true);
                auto new_route = two_opt_change(bestRoute, j, k);
                auto new_distance = getDistance(new_route, this->m_graph);

                if (new_distance < bestDistance)
                {
                    // std::cout << "\n\nNEWROUTE: ";
                    // printVector(new_route, true);
                    // std::cout << "\n\n";
                    changed = true;
                    bestRoute = new_route;
                    bestDistance = new_distance;
                    j = 0;
                }
            }
        }
    }

    return bestRoute;
}

std::vector<int> MovementHeuristic::swapMethod(std::vector<int> mainRoute) {
    auto bestRoute = mainRoute;
    auto bestDistance = getDistance(bestRoute, this->m_graph);
    auto changed = true;

    while (changed) {
        changed = false;

        for (auto i = 0; i < (int) bestRoute.size(); i++) {
            bestDistance = getDistance(bestRoute, this->m_graph);

            for (auto j = 0; j < (int) bestRoute.size(); j++) {
                if (i != j) {
                    std::swap(bestRoute[i], bestRoute[j]);
                    auto new_distance = getDistance(bestRoute, this->m_graph);

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

std::vector<int> MovementHeuristic::vnd(std::vector<int> route) {
    // auto rdmDistance = getDistance(rdm, this->m_graph);
    // auto optDistance = getDistance(opt, this->m_graph);
    // auto swapDistance = getDistance(swap, this->m_graph);

    // std::cout << "BEST ";
    // if (rdmDistance <= optDistance && rdmDistance <= swapDistance) {
    //     printRouteAndDistance(rdm, rdmDistance);
    //     return rdm;
    // } else if (optDistance <= rdmDistance && optDistance <= swapDistance) {
    //     printRouteAndDistance(opt, optDistance);
    //     return opt;
    // }
    // printRouteAndDistance(swap, swapDistance);
    // return swap;
    Movement *methods[3] = {new TwoOpt(this->m_graph),  new Swap(this->m_graph), new RandomDescent(this->m_graph)};
    auto method = 0, currentBestDistance = 0, newDistance = 0;

    currentBestDistance = getDistance(route, this->m_graph);
    // unsigned long long int count1 = 0;

    while(method < 3){
        newDistance = methods[method]->getMovement(route);

        if(newDistance < currentBestDistance){
            // std::cout << "MUDOU " << METHOD(method) << ": ";
            // printVector(route, true);
            // std::cout << "Melhorou> " << newDistance <<"\n";
            currentBestDistance = newDistance;
            method = 0;
            continue;
        }
        ++method;
    }

    return route;
}

// Coloquei retorno pq talvez tenha que usar o resultado aqui pra fazer a metaheuristica
std::vector<int> MovementHeuristic::buildRoutesByMethod(std::vector<std::vector<int>> mainRoute) {

    std::vector<int> newRoute;
    newRoute.push_back(0);

    // Alterando Rotas
    for (auto i = 0; i < (int)mainRoute.size(); i++)
    {
        std::vector<int> route = mainRoute[i];
        newRoute = mountRoute(newRoute, vnd(route));
    }

    printResultsByMethod(newRoute);
    
    return mainRoute[0];
}
