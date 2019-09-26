#include "../include/Grasp.h"
#include "../include/Helper.h"
#include "../include/MovementHeuristic.h"
#include <algorithm>

Grasp::Grasp(Graph<int>& graph, int capacity) : m_graph(graph), m_capacity(capacity) { }

bool Grasp::compareDemands(Demand& d1, Demand& d2) {
    return d1.getClientDemand() > d2.getClientDemand();
}

bool Grasp::compareDistance(std::pair<int, int> p1, std::pair<int, int> p2){
    return p1.first < p2.first;
}

int Grasp::construction(float alpha, std::vector<int>& route, int dimension) {
    
    // route.push_back(0);
    alpha += 0.0;
    std::vector<std::vector<std::pair<int, int>>> matrix;
    std::vector<std::pair<int, int>> line;
    std::vector<int> c;
    std::vector<std::pair<int, int>> v;
    std::vector<int> visited(dimension, false);

    for(auto i = 0; i < (int) dimension; i++){
        if(i >= 1)
            c.push_back(i); // inicializando com 0,1,2...
        for(auto j = 0; j < dimension; j++){
            line.push_back(std::make_pair(j,m_graph.fetchEdge(i,j)));
        }
        matrix.push_back(line);
        line.clear();
    }

    std::vector<int> aux_route;
    auto load = 0;
    int count = 0;
    int vertex_count = 1;
    auto actual = 0;

    
    visited[0] = true;
    

    for ( int i = 0; i < (int) c.size(); i++ )
    {
        std::pair<int, int> aux = std::make_pair( m_graph.fetchEdge(actual, c[i]), c[i]);
        // std::cout << aux.first << "," << aux.second << " ";
        v.push_back(aux);
    }
    sort(v.begin(), v.end(), compareDistance);

    // for ( int i = 0; i < (int) c.size(); i++ )
    // {

    //     std::cout << v[i].first << "," << v[i].second << " ";

    // }

    aux_route.push_back(0);
    while(true){
        actual = -1;
        count = 0;
        // std::cout << "Count: " << count
        while(!v.empty() && count <= 40){
            auto gtmin = 0;
            auto gtmax = v.size() - 1;
            auto lcr = gtmin + (alpha * (gtmax - gtmin));
            auto temp = rand_int(lcr + 1);
            auto pos = v[temp].second;

            if(!visited[pos] && (load + m_graph.fetchDemandByClient(pos)) <= m_capacity){
                visited[pos] = true;
                actual = pos;
                ++vertex_count;
                load = load + m_graph.fetchDemandByClient(pos);

                for ( int i = 0; i < (int)c.size(); i++ )
                    if ( c[i] == actual )
                    {
                        c.erase(c.begin() + i);
                        break;
                    }
                // std::cout << "Add: " << pos << "\n";
                aux_route.push_back(pos);
                v.clear();
                break;

            }else {
                ++count;
            }
        }
        

        if ( actual == -1 ){
            load = 0;
            aux_route.push_back(0);
            v.clear();
            // std::cout << "Rota: ";
            // printVector(aux_route, true);
        }
        if(vertex_count == dimension){
            break;
        }
        
        // for ( int i = 0; i < (int) c.size(); i++ )
        // {
        //     if(actual == -1)
        //         actual = 0;
            
        //     std::pair<int, int> aux = std::make_pair( m_graph.fetchEdge(actual, c[i]), c[i]);
        //     // std::cout << "AuxFirst: " << aux.first << " AuxSecond: " << aux.second << "\n";
        //     v.push_back(aux);
        // }

        for ( int i = 0; i < (int) c.size(); i++ )
        {
            if(actual == -1)
                actual = 0;
            std::pair<int, int> aux = std::make_pair( m_graph.fetchEdge(actual, c[i]), c[i]);
            // std::cout << aux.first << "," << aux.second << " ";
            v.push_back(aux);
        }

        sort(v.begin(), v.end(), compareDistance);
    }
    aux_route.push_back(0);
    route = aux_route;

    // std::cout << "C: " << c.size() << "\n";
    auto dis = getDistance(aux_route, m_graph);
    // printRouteAndDistance(aux_route, dis);
    return dis;
}

void Grasp::buildGrasp(int dimension) {
    int best_distance = 1000000, aux_distance = 0;
    float alphaB = 0.0;
    float alpha = 0.0;
    float bestAlpha = 0.0;
    auto mh = MovementHeuristic(m_graph);
    auto bestDistance = 10000;
    
    std::vector<int> aux_route, best_route;
    // bool is = false;

    for (int j = 1; j <= 20; j++){
        // is = false;
        
        for(int i = 0; i < 50; i++) {
            construction(alpha, aux_route, dimension);
            // std::cout << "Construiu\n\n";
            // for(auto o = 0; o < (int) aux_route.size() - 1; o ++){
            //     if(aux_route[o] == aux_route[o+1] && aux_route[o] == 0){
            //         aux_route.erase(aux_route.begin() + o);
            //     }
            // }
            // std::cout << "Antes vnd: ";
            // printVector(aux_route, true);
            mh.vnd(aux_route);
            // std::cout << "VND\n\n";

            aux_distance = getDistance(aux_route, m_graph);
            if(aux_distance < best_distance) {

                // std::cout << "Received route: ";
                // printVector(aux_route, true);
                // std::cout << "Changed! Now: " << aux_distance << std::endl;
                // printVector(aux_route, true);
                best_route = aux_route;
                best_distance = aux_distance;
                // is = true;'
            }
        }
        if(getDistance(best_route, m_graph) < bestDistance){
            // std::cout << "Alpha" << alpha << "\n";
            bestAlpha = alpha;
            // printRouteAndDistance(best_route, getDistance(best_route, m_graph));
            bestDistance = getDistance(best_route, m_graph);
        }
        alpha = alphaB + (j * 0.05);   

        // if(is){
            
        // }
        // else{
        //     alpha = alpha + (j * 0.05);
        // }
    }
    std::cout << "Alpha" << bestAlpha << "\n";
    printRouteAndDistance(best_route, getDistance(best_route, m_graph));
    
}