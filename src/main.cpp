#include <iostream>
#include <vector>
#include <ctime>
#include "parser.hpp"
#include "FLPMIPModel.hpp"

using namespace std;



int main(int   argc,char *argv[])
{
    if (argc!=4){
        std::cout << "wrong arguments. Should have run : ./GOPP.exe <nomInstance> <nomMethode> <tempsLimite>" << std::endl;
        std::cout << "methods are -mip for mip and -h for heuristic" << std::endl;
    }


    FLPData* i = load(argv[1]);
    std::vector<std::vector<int>> Y;
    std::vector<std::vector<std::vector<int>>> X;
    FLPSolution S(Y,X);
    int timeout = stoi(argv[3]);
    
    std:cout << "'" << argv[2] << "'" << std::endl;

    // if (argv[2]=="-mip"){
        FLPMIPModel m(timeout);
        S = m.solve(*i);
    // }
}
