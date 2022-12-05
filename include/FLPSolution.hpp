#ifndef FLPSOLUTION
#define FLPSOLUTION

#include <vector>
#include "FLPData.hpp"

class FLPSolution {
    public :
        //indice t,i. 1 si i est ouvert à partir du temps t, 0 sinon.
        std::vector<std::vector<int>> Y;

        //indice t,i,j. 1 si j est affecté à i au temps t, 0 sinon.
        std::vector<std::vector<std::vector<int>>> X;

        FLPSolution(){}

        FLPSolution(std::vector<std::vector<int>> Y,std::vector<std::vector<std::vector<int>>> X){
            this->X=X;
            this->Y=Y;
        }

        int check(FLPData i);

        int checkP(FLPData i);
        int checkN(FLPData i);
        int checkOuvertureUniqueY(FLPData i);
        int checkOuvertureUniqueX(FLPData i);
        int checkOuvertureService(FLPData i);
        int checkNonDesouverture(FLPData i);
        int checkBinarity(FLPData i);
};

#endif