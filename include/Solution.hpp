#ifndef SOLUTION
#define SOLUTION

#include <vector>
#include "Instance.hpp"

class Solution {
    public :
        //indice t,i. 1 si i est ouvert à partir du temps t, 0 sinon.
        std::vector<std::vector<int>> Y;

        //indice t,i,j. 1 si j est affecté à i au temps t, 0 sinon.
        std::vector<std::vector<std::vector<int>>> X;

        Solution(){}

        Solution(std::vector<std::vector<int>> Y,std::vector<std::vector<std::vector<int>>> X){
            this->X=X;
            this->Y=Y;
        }

        int check(Instance i);

        int checkP(Instance i);
        int checkN(Instance i);
        int checkOuvertureUniqueY(Instance i);
        int checkOuvertureUniqueX(Instance i);
        int checkOuvertureService(Instance i);
        int checkNonDesouverture(Instance i);
        int checkBinarity(Instance i);
};

#endif