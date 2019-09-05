#include <iostream>
#include <vector>

class Graph {

private:
    int** _adjMatrix;
    int _vertexSize;

public:
    Graph(int vertexSize);
    ~Graph();
    void addEdge(int u, int v, int weight);
    void addDemand(int v, int demand);
    void printMatrix();
    void nearestNeighbor(int capacity);
};