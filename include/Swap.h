#ifndef HEURISTICS_MOVEMENT_SWAP_H
#define HEURISTICS_MOVEMENT_SWAP_H

#include "../include/Graph.h"
#include "../include/Movement.h"

class Swap final : public Movement {
public:
    Swap(Graph<int>&);
    ~Swap();

    int getMovement(std::vector<int>&) override;
};

#endif // define HEURISTICS_MOVEMENT_TWO_OPT_H