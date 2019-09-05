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

void Graph::nearestNeighbor(int capacity) {
    bool visitedVertex[this->_vertexSize];
    int distance = 0;
    int shortestRoute, visitedCount = 0, vertex = 0, aux = 0;
    int load = 0;
    std::vector<int> route;

    for(int i=0; i < this->_vertexSize; i++) {
        visitedVertex[i] = false;
    }

    route.push_back(0);
    while(visitedCount < this->_vertexSize) {
        shortestRoute = 999;
        
        for(int j = 0; j < this->_vertexSize; j++) {
            if(j != vertex && this->_adjMatrix[vertex][j] < shortestRoute && !visitedVertex[j]) {
                if((load + this->_adjMatrix[j][j]) <= capacity){
                    shortestRoute = this->_adjMatrix[vertex][j];
                    aux = j;
                }
            }
        }
        route.push_back(aux);
        vertex = aux;
        distance += shortestRoute;
        load += this->_adjMatrix[vertex][vertex];
        if(aux != 0) {
            visitedVertex[aux] = true;
            visitedCount++;
        } else {
            load = 0;
        }
    }

    route.push_back(0);
    distance += this->_adjMatrix[vertex][0];
}

