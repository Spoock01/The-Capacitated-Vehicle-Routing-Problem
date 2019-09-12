#include <vector>
#include "Graph.h"

class ConstructiveHeuristic {
private:
    Graph<int> m_graph;

public:
    ConstructiveHeuristic(Graph<int> graph);
    int getDistance(std::vector<int> route);
    std::vector<int> nearestNeighbor(int capacity, int dimension);
};