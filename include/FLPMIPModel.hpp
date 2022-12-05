#ifndef FLPMIPMODEL
#define FLPMIPMODEL

#include "FLPSolution.hpp"

class FLPMIPModel{
    FLPSolution solve(FLPData instance, int timeLimit);
};

#endif