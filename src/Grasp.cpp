#include "../include/Grasp.h"
#include "../include/Helper.h"
#include "../include/MovementHeuristic.h"
#include <algorithm>

Grasp::Grasp(Graph<int>& graph, int capacity) : m_graph(graph), m_capacity(capacity) { }

bool Grasp::compareDemands(Demand& d1, Demand& d2) {
    return d1.getClientDemand() > d2.getClientDemand();
}

// int Grasp::construction(float alpha, std::vector<int>& route) {
    // std::vector<Demand> demands_copy = m_graph.getDemands();
    // std::vector<int> aux_route;
    // int vector_size = demands_copy.size();
    // // int best = 1000000;
    // int load = 0;
    // int count = 0;
    // int vertex_count = 0;
    // // route.push_back(1);
    // // std::cout << alpha << "\n";
    // // std::vector<int> visited_vertex(demands_copy.size(), false);
    // // std::sort(demands_copy.begin(), demands_copy.end(), compareDemands);
    // demands_copy.pop_back();

    // // so pra saber se tava ordenado
    // // for(unsigned i=0; i < demands_copy.size(); i++) {
    // //     std::cout << "Vertice[" << i << "] = " << demands_copy[i].getClient();
    // //     std::cout << "\tDemanda = " << demands_copy[i].getClientDemand() << "\n";
    // // }

    // int current_pos = 0;
    // std::vector<bool> pqp_meu_gato_botou_um_ovo(demands_copy.size(), false);

    // aux_route.push_back(current_pos);
    // pqp_meu_gato_botou_um_ovo[0] = true;
    // while (vertex_count < vector_size - 1) {

    //     count = 0;
    //     while (!demands_copy.empty() && count <= 20) {
            // int gtmin = 0;
            // int gtmax = pqp_meu_gato_botou_um_ovo.size() - 1;
            // int lcr = gtmin + alpha * (gtmax - gtmin);
            // // if(vertex_count == 16)  std::cout << "lcr = " << lcr << "\n";
            // int pos = rand_int(lcr + 1);

            
    //         // break;
    //         if (!pqp_meu_gato_botou_um_ovo[pos] && (load + demands_copy[pos].getClientDemand() <= m_capacity)) {
    //             load += demands_copy[pos].getClientDemand();
    //             aux_route.push_back(demands_copy[pos].getClient());
    //             ++vertex_count;
    //             pqp_meu_gato_botou_um_ovo[pos] = true;
    //             // std::cout << "lcr = " << lcr << "\tpos = " << pos << "\n";
    //             // std::cout << "demands_copy size = " << demands_copy.size();
    //             // std::cout << "\tdemands_copy[" << pos << "] = " << demands_copy[pos].getClient() << "\n";
    //             // std::cout << "vertex_count = " << vertex_count << "\n";
    //             demands_copy.erase(demands_copy.begin() + pos);
    //         } else {
    //             ++count;
    //         }
    //     }

    //     // std::cout << "-----------------SAIU----------------------\n";
    //     load = 0;
    //     current_pos = 0;
    //     aux_route.push_back(current_pos);
    //     // break;

    // }
    // route = aux_route;
    // // printRouteAndDistance(route, getDistance(route, m_graph));
    // return getDistance(route, m_graph);
// }

int Grasp::construction(float alpha, std::vector<int>& route, int dimension) {
    std::vector<int> aux_route;
    std::vector<int> pqp(dimension);
    int load = 0;
    int count = 0;
    int vertex_count = 0;
    std::vector<bool> visited_vertex(dimension, false);

    aux_route.push_back(0);
    visited_vertex[0] = true;
    pqp.pop_back();

    while(vertex_count < (dimension - 2)) {
        count = 0;
        while(!pqp.empty() && count <= 40) {
            int gtmin = 0;
            int gtmax = dimension - 1;
            int lcr = gtmin + alpha * (gtmax - gtmin);
            int pos = rand_int(lcr + 1);

            if(!visited_vertex[pos] && (load + m_graph.fetchDemandByClient(pos)) <= m_capacity) {
                pqp.pop_back();
                load += m_graph.fetchDemandByClient(pos);
                visited_vertex[pos] = true;
                aux_route.push_back(pos);
                ++vertex_count;
            } else {
                ++count;
            }
        }

        aux_route.push_back(0);
        load = 0;
    }

    route = aux_route;
    int route_distance = getDistance(route, m_graph);
    printRouteAndDistance(route, route_distance);

    return route_distance;
}

void Grasp::buildGrasp(int dimension) {
    int best_distance = 1000000, aux_distance = 0;
    auto mh = MovementHeuristic(m_graph);
    std::vector<int> aux_route, best_route;
    for(int i = 0; i < 100; i++) {
        construction(0.85, aux_route, dimension);
        mh.vnd(aux_route);
        aux_distance = getDistance(aux_route, m_graph);
        if(aux_distance < best_distance) {
            best_route = aux_route;
            best_distance = aux_distance;
        }
    }

    printRouteAndDistance(best_route, getDistance(best_route, m_graph));
}