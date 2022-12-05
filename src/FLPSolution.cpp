#include "FLPSolution.hpp"

int FLPSolution::check (FLPData i){
    int isFeasable = checkBinarity(i);
    if (isFeasable!=1){
        return isFeasable;
    }

    isFeasable = checkOuvertureUniqueY(i);
    if (isFeasable!=1){
        return isFeasable;
    }

    isFeasable = checkN(i);
    if (isFeasable!=1){
        return isFeasable;
    }

    isFeasable = checkP(i);
    if (isFeasable!=1){
        return isFeasable;
    }

    isFeasable = checkOuvertureService(i);
    if (isFeasable!=1){
        return isFeasable;
    }

    isFeasable = checkNonDesouverture(i);
    if (isFeasable!=1){
        return isFeasable;
    }

    isFeasable = checkOuvertureUniqueX(i);
    if (isFeasable!=1){
        return isFeasable;
    }

    return 1;
}

int FLPSolution::checkP(FLPData i){
    for (int t=0; t<i.nbPeriode; ++t){
        int sum = 0;
        for (int d=0; d<i.nbDepotPotentiel; ++d){
            sum+=this->Y[t][d];
        }
        if (sum!=i.p[t]){
            return -4;
        }
    }
    return 1;
}

int FLPSolution::checkN(FLPData i){
    for (int t=0; t<i.nbPeriode; ++t){
        int sum = 0;
        for (int d=0; d<i.nbDepotPotentiel; ++d){
            for (int j=0; j<i.nbClient; ++j){
                sum+=this->X[t][d][j];
            }
        }
        if (sum!=i.n[t]){
            return -3;
        }
    }
    return 1;
}

int FLPSolution::checkOuvertureUniqueY(FLPData i){
    for (int d=0; d<i.nbDepotPotentiel; ++d){
        int sum = 0;
        for (int t=0; t<i.nbPeriode; ++t){
            sum+=this->Y[t][d];
        }
        if (sum>1){
            return -2;
        }
    }
    return 1;
}

int FLPSolution::checkOuvertureUniqueX(FLPData i){
    for (int t=0; t<i.nbPeriode; ++t){
        for (int j=0; j<i.nbPeriode; ++j){
            int sum = 0;
            for (int d=0; d<i.nbDepotPotentiel; ++d){
                sum+=this->X[t][d][j];
            }
            if (sum>1){
                return -7;
            }
        }
    }
    return 1;
}

int FLPSolution::checkOuvertureService(FLPData i){
    for (int t=0; t<i.nbPeriode; ++t){
        for (int j=0; j<i.nbClient; ++j){
            for (int d=0; d<i.nbDepotPotentiel; ++d){
                if (this->X[t][d][j]==1){
                    int isOpen = 0;
                    for (int t2=0; t2<=t; ++t2){
                        if (this->Y[t2][d]==1){
                            isOpen = 1;
                        }
                    }
                    if (isOpen==0){
                        return -5;
                    }
                }
            }
        }
    }
    return 1;
}

int FLPSolution::checkNonDesouverture(FLPData i){
    for (int j=0; j<i.nbClient; ++j){
        for (int d=0; d<i.nbDepotPotentiel; ++d){
            for (int t=0; t<i.nbPeriode; ++t){
                if (this->X[t][d][j] == 1){
                    for (int t2=t+1; t2<i.nbPeriode; ++t2){
                        int isAffected = 0;
                        for (int d2=0; d2<i.nbDepotPotentiel; ++d2){
                            if (this->X[t2][d2][j]==1){
                                isAffected = 1;
                            }
                        }
                        if (isAffected==0){
                            return -6;
                        }
                    }
                }
            }
        }
    }
    return 1;
}

int FLPSolution::checkBinarity(FLPData i){
    for (int j=0; j<i.nbClient; ++j){
        for (int d=0; d<i.nbDepotPotentiel; ++d){
            for (int t=0; t<i.nbPeriode; ++t){
                if (this->X[t][d][j]!=0 && this->X[t][d][j]!=1){
                    return -1;
                }

                if (this->Y[t][d]!=0 && this->Y[t][d]!=1){
                    return -1;
                }
            }
        }
    }
    return 1;
}