#include "Graph.h"
#include <vector>


class MovementHeuristic {
private:
    Graph<int> m_graph;

public:
    MovementHeuristic(Graph<int>& graph);
    void printResultsByMethod(std::string method, std::vector<int> route);
    std::vector<int> randomDescentMethod(std::vector<std::vector<int>> &allRoutes,
                                     std::vector<int> &g_distance_array, int i,
                                     std::vector<int> mainRoute);
    std::vector<int> two_opt_change(std::vector<int> route, int index, int k_index);
    std::vector<int> two_opt(std::vector<int> mainRoute);
    std::vector<int> swapMethod(std::vector<int> mainRoute);
    std::vector<int> vnd(std::vector<int> rdm, std::vector<int> opt, std::vector<int> swap);
    std::vector<int> buildRoutesByMethod(std::vector<std::vector<int>> mainRoute);
};