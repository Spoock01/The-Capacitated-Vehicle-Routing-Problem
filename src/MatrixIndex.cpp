#include "../include/MatrixIndex.h"

MatrixIndex::MatrixIndex(int index, int demand){
    this->m_originalIndex = index;
    this->m_demand = demand;
}

MatrixIndex::MatrixIndex(){
}

int MatrixIndex::getOriginalIndex(){
    return this->m_originalIndex;
}

int MatrixIndex::getDemand(){
    return this->m_demand;
}