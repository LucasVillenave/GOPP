#include <iostream>
#include <vector>
#include <ctime>
#include "parser.hpp"
#include "Instance.hpp"
#include "Solution.hpp"
#include "MIP.hpp"

using namespace std;



int main(int   argc,char *argv[])
{
    Instance* i = load("instMulti_I10_J50_T3_1");
    std::vector<std::vector<int>> Y;
    std::vector<std::vector<std::vector<int>>> X;
    Solution S(Y,X);
    S = solve(*i, 1000);
}
