#include "../include/Grasp.h"
#include <algorithm>
#include "../include/Helper.h"
#include "../include/MovementHeuristic.h"

Grasp::Grasp(Graph<int>& graph, int capacity) : m_graph(graph), m_capacity(capacity) {}

bool Grasp::compareDistance(std::pair<int, int> p1, std::pair<int, int> p2) {
    return p1.first < p2.first;
}

int Grasp::construction(float alpha, std::vector<int>& route, int dimension) {
    std::vector<std::vector<std::pair<int, int>>> matrix;
    std::vector<std::pair<int, int>> line;
    std::vector<int> c;
    std::vector<std::pair<int, int>> v;
    std::vector<int> visited(dimension, false);
    std::vector<int> aux_route;
    auto load        = 0;
    int count        = 0;
    int vertex_count = 1;
    auto actual      = 0;
    visited[0]       = true;

    /*
        Inicializando a lista de candidatos a partir do vértice 1
        Inicia a matriz(pair) com número do vértice e a distância 
    */
    for (auto i = 0; i < (int)dimension; i++) {
        if (i >= 1) c.push_back(i); // inicializando com 0,1,2...
        for (auto j = 0; j < dimension; j++) {
            line.push_back(std::make_pair(j, m_graph.fetchEdge(i, j)));
        }
        matrix.push_back(line);
        line.clear();
    }

    /*
        Inicializando a vetor de candidatos não visitados
        Ordena a linha do atual vértice
    */
    for (int i = 0; i < (int)c.size(); i++) {
        std::pair<int, int> aux = std::make_pair(m_graph.fetchEdge(actual, c[i]), c[i]);
        v.push_back(aux);
    }
    sort(v.begin(), v.end(), compareDistance);

    aux_route.push_back(0);
    while (true) {
        actual = -1;
        count  = 0;

        while (!v.empty() && count <= 40) {
            auto gtmin = 0;
            auto gtmax = v.size() - 1;
            auto lcr   = gtmin + (alpha * (gtmax - gtmin));
            auto temp  = rand_int(lcr + 1);
            auto pos   = v[temp].second;

            if (!visited[pos] && (load + m_graph.fetchDemandByClient(pos)) <= m_capacity) {
                visited[pos] = true;
                actual       = pos;
                ++vertex_count;
                load = load + m_graph.fetchDemandByClient(pos);

                for (int i = 0; i < (int)c.size(); i++)
                    if (c[i] == actual) {
                        c.erase(c.begin() + i);
                        break;
                    }
                aux_route.push_back(pos);
                v.clear();
                break;

            } else {
                ++count;
            }
        }

        if (actual == -1) {
            load = 0;
            aux_route.push_back(0);
            v.clear();
        }

        // Todos os vértices foram visitados
        if (vertex_count == dimension) {
            break;
        }

        for (int i = 0; i < (int)c.size(); i++) {
            if (actual == -1) actual = 0;
            std::pair<int, int> aux = std::make_pair(m_graph.fetchEdge(actual, c[i]), c[i]);

            v.push_back(aux);
        }

        sort(v.begin(), v.end(), compareDistance);
    }
    aux_route.push_back(0);
    route = aux_route;
    return getDistance(aux_route, m_graph, false);
}

int Grasp::buildGrasp(int dimension) {
    auto best_distance = 1000000, aux_distance = 0;
    float alphaB = 0.0, alpha = 0.0;
    auto mh = MovementHeuristic(m_graph);
    std::vector<int> aux_route, best_route;

    for (int j = 1; j <= 20; j++) {
        for (int i = 0; i < 50; i++) {
            construction(alpha, aux_route, dimension);
            aux_route    = mh.buildRoutesByMethod(splitRoutes(aux_route));
            aux_distance = getDistance(aux_route, m_graph, false);
            // printRouteAndDistance(aux_route, aux_distance);
            if (aux_distance < best_distance) {
                best_route    = aux_route;
                best_distance = aux_distance;
            }
        }
        // if (getDistance(best_route, m_graph, false) < bestDistance) {
        //     bestDistance = getDistance(best_route, m_graph, false);
        // }
        alpha = alphaB + (j * 0.02);
    }

    auto distance = getDistance(best_route, m_graph, false);
    printRouteAndDistance(best_route, distance);
    return distance;
}