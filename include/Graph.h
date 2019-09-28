#ifndef GRAPH_H
#define GRAPH_H

#include <iomanip>
#include <iostream>
#include <ostream>
#include <vector>
#include "Demand.h"

template<typename T> class Graph {
private:
    std::vector<std::vector<T>> m_adj_matrix;
    std::vector<Demand> m_demands;

public:
    void addRow(std::vector<T>& row) { this->m_adj_matrix.push_back(row); }

    std::vector<T> fetchRow(const unsigned row) { return this->m_adj_matrix[row]; }

    int fetchEdge(const unsigned row, const unsigned col) { return this->m_adj_matrix[row][col]; }

    void addEdge(const unsigned row, const unsigned col, const T& weight) {
        this->m_adj_matrix[row][col] = weight;
        this->m_adj_matrix[col][row] = weight;
    }
    void createGraph(const unsigned row, const unsigned col, const T& default_value) {
        for (auto i = 0; i < (int)col; ++i) {
            this->m_adj_matrix.push_back(std::vector<T>(row, default_value));
        }
    }

    // This function exists only for test
    void setAdjMatrix(std::vector<std::vector<T>> adj_matrix) { this->m_adj_matrix = adj_matrix; }

    void setDemands(std::vector<Demand> demands) { this->m_demands = demands; }

    std::vector<Demand> getDemands() { return this->m_demands; }

    int fetchDemandByClient(int client) { return this->m_demands[client].getClientDemand(); }

    friend std::ostream& operator<<(std::ostream& stream, const Graph& matrix) {
        auto col = matrix.m_adj_matrix.size();
        auto row = matrix.m_adj_matrix[0].size();

        stream << "Dimensions: " << row << "x" << col << "\n";
        stream << "--------------------------------------------\n\n";

        stream << "       ";

        for (auto j = 0; j < (int)row; ++j) {
            stream << std::setfill('0') << std::setw(2) << j << " ";
        }

        stream << "\n\n";

        for (auto i = 0; i < (int)col; ++i) {
            stream << std::setfill('0') << std::setw(2) << i << "   | ";
            for (auto j = 0; j < (int)row; ++j) {
                stream << matrix.m_adj_matrix[i][j] << " ";
            }
            stream << "|\n";
        }

        return stream;
    }
};

#endif