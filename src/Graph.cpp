#include "../include/Graph.h"

Graph::Graph(int vertexSize) {
    this->_vertexSize = vertexSize;
    this->_adjMatrix = new int*[vertexSize];
    for(int i = 0; i < vertexSize; i++) {
        this->_adjMatrix = new int*[vertexSize];
    }
}

Graph::~Graph() {
    for(int i = 0; i < this->_vertexSize; i++) {
        delete[] this->_adjMatrix[i];
    }

    delete[] this->_adjMatrix;
}

void Graph::addEdge(int u, int v, int weight) {
    this->_adjMatrix[u][v] = weight;
    this->_adjMatrix[v][u] = weight;
}

void Graph::addDemand(int v, int demand) {
    this->_adjMatrix[v][v] = demand;
}

void Graph::printMatrix() {
    for(int i = 0; i < this->_vertexSize; i++) {
        for(int j = 0; j < this->_vertexSize; i++) {
            std::cout << this->_adjMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Graph::nearestRoute() {
    bool visitedVertex[this->_vertexSize];
    int distance = 0;
    int minor, visitedCount = 0, vertex = 0, aux = 0;
    std::vector<int> route;

    for(int i=0; i < this->_vertexSize; i++) {
        visitedVertex[i] = false;
    }

    route.push_back(0);
    while(visitedCount < this->_vertexSize) {
        minor = 999;
        for(int j = 0; j < this->_vertexSize && j != vertex; j++) {
            if(this->_adjMatrix[vertex][j] < minor) {
                minor = this->_adjMatrix[vertex][j];
                aux = j;
            }
        }
        visitedVertex[aux] = true;
        route.push_back(aux);
        vertex = aux;
        distance += minor;
        visitedCount++;
    }
}

