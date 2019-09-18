#ifndef MATRIX_INDEX_H
#define MATRIX_INDEX_H

class MatrixIndex
{

private:
    int m_originalIndex;
    int m_demand;

public:
    MatrixIndex();
    MatrixIndex(int, int);
    int getOriginalIndex();
    int getDemand();
};

#endif