#include "../include/Swap.h"
#include "../include/Helper.h"

Swap::Swap(Graph<int>& graph) : Movement(graph) {}
Swap::~Swap() {}

int Swap::getMovement(std::vector<int>& mainRoute) {
    auto bestDistance = getDistance(mainRoute, this->m_graph, false);
    auto changed      = true;

    while (changed) {
        changed = false;

        for (auto i = 0; i < (int)mainRoute.size(); i++) {
            bestDistance = getDistance(mainRoute, this->m_graph, false);

            for (auto j = 0; j < (int)mainRoute.size(); j++) {
                if (i != j) {
                    std::swap(mainRoute[i], mainRoute[j]);
                    auto new_distance = getDistance(mainRoute, this->m_graph, false);

                    if (new_distance < bestDistance) {
                        bestDistance = new_distance;
                        changed      = true;
                    } else {
                        std::swap(mainRoute[i], mainRoute[j]);
                    }
                }
            }
        }
    }

    return bestDistance;
}
