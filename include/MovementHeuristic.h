#include <vector>
#include "Graph.h"

class MovementHeuristic {
private:
    Graph<int> m_graph;

public:
    MovementHeuristic(Graph<int>&);
    void printResultsByMethod(std::vector<int>);
    std::vector<int> randomDescentMethod(std::vector<int>);
    std::vector<int> two_opt_change(std::vector<int>, int, int);
    std::vector<int> two_opt(std::vector<int>);
    std::vector<int> swapMethod(std::vector<int>);
    std::vector<int> vnd(std::vector<int>);
    std::vector<int> buildRoutesByMethod(std::vector<std::vector<int>>);
};