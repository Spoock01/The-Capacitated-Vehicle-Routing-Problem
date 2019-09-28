#include "../include/TwoOpt.h"
#include "../include/Helper.h"

TwoOpt::TwoOpt(Graph<int>& graph) : Movement(graph) {}

TwoOpt::~TwoOpt() {}

std::vector<int> TwoOpt::twoOptChange(std::vector<int>& route, const int index_i,
                                      const int index_k) {
    std::vector<int> newRoute;

    for (auto i = 0; i < index_i; i++) newRoute.push_back(route[i]);

    for (auto i = index_k; i >= index_i; i--) newRoute.push_back(route[i]);

    for (auto i = index_k + 1; i < (int)route.size(); i++) newRoute.push_back(route[i]);

    return newRoute;
}

int TwoOpt::getMovement(std::vector<int>& mainRoute) {
    auto bestDistance = getDistance(mainRoute, this->m_graph, false);
    auto changed      = true;

    while (changed) {
        changed = false;

        for (auto j = 0; j < (int)mainRoute.size(); j++) {
            bestDistance = getDistance(mainRoute, this->m_graph, false);
            for (auto k = j + 1; k < (int)mainRoute.size(); k++) {
                auto new_route    = twoOptChange(mainRoute, j, k);
                auto new_distance = getDistance(new_route, this->m_graph, false);

                if (new_distance < bestDistance) {
                    changed      = true;
                    mainRoute    = new_route;
                    bestDistance = new_distance;
                    j            = 0;
                }
            }
        }
    }
    return bestDistance;
}
