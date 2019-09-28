#ifndef HEURISTICS_MOVEMENT_TWO_OPT_H
#define HEURISTICS_MOVEMENT_TWO_OPT_H

#include "../include/Graph.h"
#include "../include/Movement.h"

class TwoOpt final : public Movement {
private:
    std::vector<int> twoOptChange(std::vector<int>&, int, int);

public:
    TwoOpt(Graph<int>&);
    ~TwoOpt();

    int getMovement(std::vector<int>&) override;
};

#endif // define HEURISTICS_MOVEMENT_TWO_OPT_H