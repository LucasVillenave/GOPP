#include <iostream>
#include <vector>
#include <ctime>
#include "parser.hpp"
#include "FLPMIPModel.hpp"

using namespace std;



int main(int   argc,char *argv[])
{
    FLPData* i = load("instMulti_I10_J50_T3_1");
    std::vector<std::vector<int>> Y;
    std::vector<std::vector<std::vector<int>>> X;
    FLPSolution S(Y,X);
    S = solve(*i, 1000);
}
