#include "Graph.h"
#include <vector>
#include "../include/Demand.h"

class Grasp {
private:
    Graph<int>& m_graph;
    int m_capacity;
    static bool compareDemands(Demand& d1, Demand& d2);
    // std::vector<std::pair<>>
public:
    Grasp(Graph<int>& graph, int capacity);
    int construction(float alpha, std::vector<int>& route, int dimension);
    void buildGrasp(int dimension);
};