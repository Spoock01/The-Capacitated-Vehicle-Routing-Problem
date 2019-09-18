#ifndef HEURISTICS_MOVEMENT_RANDOMDESCENT_H
#define HEURISTICS_MOVEMENT_RANDOMDESCENT_H

#include "../include/Graph.h"
#include "../include/Movement.h"

class RandomDescent final : public Movement {
public:
    RandomDescent(Graph<int>&);
    ~RandomDescent();

    int getMovement(std::vector<int>&) override;
};

#endif // define HEURISTICS_MOVEMENT_TWO_OPT_H