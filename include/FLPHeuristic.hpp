#ifndef FLPHEURISTIC
#define FLPHEURISTIC

#include "FLPData.hpp"
#include "FLPSolution.hpp"

class FLPHeuristic{
    public : 
        int timeLimit;

        FLPHeuristic(int timeout){this->timeLimit = timeout;}

        FLPSolution solve(FLPData instance);
};

#endif