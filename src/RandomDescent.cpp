#include "../include/RandomDescent.h"
#include "../include/Helper.h"

RandomDescent::RandomDescent(Graph<int>& graph) : Movement(graph) {}

RandomDescent::~RandomDescent(){}

int RandomDescent::getMovement(std::vector<int>& route){

    std::vector<std::vector<int>> allRoutes;
    auto bestDistance = getDistance(route, this->m_graph, false);

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

    auto times = allRoutes.size() / 3;
    times = times < 1 ? 0 : times;

    for (auto index = 0; index < (int)times; index++)
    {
        auto randomNumber = rand_int(allRoutes.size());
        auto currentDistance = getDistance(allRoutes[randomNumber], this->m_graph, false);

        if (currentDistance < bestDistance)
        {
            route = allRoutes[randomNumber];
            bestDistance = currentDistance;
            index = 0;
        }
    }
    allRoutes.clear();
    return bestDistance;
}