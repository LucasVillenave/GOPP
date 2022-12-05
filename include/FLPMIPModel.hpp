#ifndef FLPMIPMODEL
#define FLPMIPMODEL

#include "FLPSolution.hpp"

class FLPMIPModel{
    public : 
        int timeLimit;

        FLPMIPModel(int timeout){this->timeLimit = timeout;}

        FLPSolution solve(FLPData instance);
};

#endif