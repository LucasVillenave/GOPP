#include <iostream>
#include <vector>
#include <ctime>
#include "parser.hpp"
#include "FLPMIPModel.hpp"
#include "FLPHeuristic.hpp"

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

    std::string method = argv[2];

    if (method=="-mip"){
        FLPMIPModel m(timeout);
        S = m.solve(*i);
    }else{
        if (method=="-h"){
            std::cout << "ici connard" << std::endl;
            FLPHeuristic h(timeout);
            S = h.solve(*i);
        }else{
            std::cout << "wrong arguments. Should have run : ./GOPP.exe <nomInstance> <nomMethode> <tempsLimite>" << std::endl;
            std::cout << "methods are -mip for mip and -h for heuristic" << std::endl;
        }
    }
}
