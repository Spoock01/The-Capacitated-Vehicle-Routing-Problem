#include "../include/MovementHeuristic.h"
#include "../include/Helper.h"

MovementHeuristic::MovementHeuristic(Graph<int>& graph) : m_graph(graph) { }

void MovementHeuristic::printResultsByMethod(std::string method, std::vector<int> route) {
    std::cout << method << ":\t";
    printVector(route, true);
    std::cout << "Resultado " << method << ": " << getDistance(route, this->m_graph) << "\n\n";
}

std::vector<int> MovementHeuristic::randomDescentMethod(std::vector<std::vector<int>> &allRoutes,
                                     std::vector<int> &g_distance_array, int i,
                                     std::vector<int> mainRoute) {
    auto times = allRoutes.size() / 3;
    times = times < 1 ? 0 : times;
    auto bestRoute = mainRoute;

    for (auto index = 0; index < (int)times; index++)
    {
        auto randomNumber = rand_int(allRoutes.size());
        auto currentDistance = getDistance(allRoutes[randomNumber], this->m_graph);

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

std::vector<int> MovementHeuristic::two_opt_change(std::vector<int> route, int index, int k_index) {
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
                    break;
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

std::vector<int> MovementHeuristic::vnd(std::vector<int> rdm, std::vector<int> opt, std::vector<int> swap) {
    auto rdmDistance = getDistance(rdm, this->m_graph);
    auto optDistance = getDistance(opt, this->m_graph);
    auto swapDistance = getDistance(swap, this->m_graph);

    std::cout << "BEST ";
    if (rdmDistance <= optDistance && rdmDistance <= swapDistance) {
        printRouteAndDistance(rdm, rdmDistance);
        return rdm;
    } else if (optDistance <= rdmDistance && optDistance <= swapDistance) {
        printRouteAndDistance(opt, optDistance);
        return opt;
    }
    printRouteAndDistance(swap, swapDistance);
    return swap;
}

// Coloquei retorno pq talvez tenha que usar o resultado aqui pra fazer a metaheuristica
std::vector<int> MovementHeuristic::buildRoutesByMethod(std::vector<std::vector<int>> mainRoute) {
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
        auto bestDistance = getDistance(route, this->m_graph);
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

    printResultsByMethod("RDM", newRoute);
    printResultsByMethod("OPT", optRoute);
    printResultsByMethod("SWAP", swapRoute);
    
    return vnd(newRoute, optRoute, swapRoute);
}