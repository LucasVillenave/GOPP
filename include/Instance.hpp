#ifndef INSTANCE
#define INSTANCE

#include <vector>

//miam on balance toute les données dans une seule classe en public. c'est dégeulasse mais on s'en tappe
class Instance {
    public :
        //indices i,j,t. cost of assigning j to i at time t.
        std::vector<std::vector<std::vector<int>>> c;

        //indices i,t. cost of opening i starting at time t.
        std::vector<std::vector<int>> f;
        std::vector<int> n;
        std::vector<int> p;

        int nbClient;
        int nbDepotPotentiel;
        int nbPeriode;

        Instance(
            std::vector<std::vector<std::vector<int>>> c,
            std::vector<std::vector<int>> f,
            std::vector<int> n,
            std::vector<int> p,
            int nbClient,
            int nbDepotPotentiel,
            int nbPeriode)
            {
                this->c = c;
                this->p = p;
                this->n = n;
                this->f = f;
                this->nbClient = nbClient;
                this->nbDepotPotentiel = nbDepotPotentiel;
                this->nbPeriode = nbPeriode;
            }

};

#endif