#include "gurobi_c++.h"
#include "MIP.hpp"
#include <sstream>

Solution solve(Instance instance, int timeLimit){
    Solution returnSol;
    try{
        std::cout << "--> Creating the Gurobi environment" << std::endl;
        GRBEnv env(true);
        env.start();

        std::cout << "--> Creating the Gurobi model" << std::endl;
        GRBModel model(env);

        std::cout << "--> Creating the Gurobi variables" << std::endl;

        std::stringstream name;

        GRBVar ** Y = new GRBVar*[instance.nbPeriode];
        GRBVar *** X = new GRBVar**[instance.nbPeriode];
        for (int t=0; t<instance.nbPeriode; ++t){
            Y[t] = new GRBVar[instance.nbDepotPotentiel];
            X[t] = new GRBVar*[instance.nbDepotPotentiel];
            for (int i=0; i<instance.nbDepotPotentiel; ++i){
                name << "Y[" << t << "][" << i << "]";
                Y[t][i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, name.str());
                name.str("");

                X[t][i] = new GRBVar[instance.nbClient];
                for (int j=0; j<instance.nbClient; ++j){
                    name << "X[" << t << "][" << i << "][" << j << "]";
                    X[t][i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, name.str());
                    name.str("");
                }
            }
        }

        std::cout << "--> Creating the Objective" << std::endl;

        GRBLinExpr obj = 0;
        for (int t=0; t<instance.nbPeriode; ++t){
            for (int i=0; i<instance.nbDepotPotentiel; ++i){
                obj += Y[t][i]*instance.f[i][t];
                for (int j=0; j<instance.nbClient; ++j){
                    obj += X[t][i][j]*instance.c[i][j][t];
                }
            }
        }
        model.setObjective(obj, GRB_MINIMIZE);

        std::cout << "--> Creating the Constraint P" << std::endl;
        for (int t=0; t<instance.nbPeriode; ++t){
            GRBLinExpr pConst = 0;
            for (int i=0; i<instance.nbDepotPotentiel; ++i){
                pConst += Y[t][i];
            }
            name << "p const of time " << t;
            model.addConstr(pConst==instance.p[t],name.str());
            name.str("");
        }

        std::cout << "--> Creating the Constraint N" << std::endl;
        for (int t=0; t<instance.nbPeriode; ++t){
            GRBLinExpr nConst = 0;
            for (int i=0; i<instance.nbDepotPotentiel; ++i){
                for (int j=0; j<instance.nbClient; ++j){
                    nConst += X[t][i][j];
                }
            }
            name << "n const of time " << t;
            model.addConstr(nConst==instance.n[t],name.str());
            name.str("");
        }

        std::cout << "--> Creating the Ouverture constraint" << std::endl;
        for (int t=0; t<instance.nbPeriode; ++t){
            for (int i=0; i<instance.nbDepotPotentiel; ++i){
                for (int j=0; j<instance.nbClient; ++j){
                    GRBLinExpr ouvertureConst = X[t][i][j];
                    for(int t2=0; t2<=t; ++t2){
                        ouvertureConst -= Y[t2][i];
                    }
                    name << "ouverture const of X[" << t << "][" << i << "][" << j << "]";
                    model.addConstr(ouvertureConst<=0,name.str());
                    name.str("");
                }
            }
        }

        std::cout << "--> Creating the non Desouverture constraint" << std::endl;
        for (int t=0; t<instance.nbPeriode; ++t){
            for (int i=0; i<instance.nbDepotPotentiel; ++i){
                for (int j=0; j<instance.nbClient; ++j){
                    GRBLinExpr desouvertureConst = X[t][i][j] * (instance.nbPeriode-1-t);
                    for(int t2=t+1; t2<instance.nbPeriode; ++t2){
                        for(int i2=0; i2<instance.nbDepotPotentiel; ++i2){
                            desouvertureConst -= X[t2][i2][j];
                        }
                    }
                    name << "non desouverture const of X[" << t << "][" << i << "][" << j << "]";
                    model.addConstr(desouvertureConst<=0,name.str());
                    name.str("");
                }
            }
        }

        std::cout << "--> Creating the unique affectation constraint" << std::endl;
        for (int t=0; t<instance.nbPeriode; ++t){
            for (int j=0; j<instance.nbClient; ++j){
                GRBLinExpr uniqueAffConst = 0;
                for (int i=0; i<instance.nbDepotPotentiel; ++i){
                    uniqueAffConst += X[t][i][j];
                }
                name << "unique affectation constraint of X[" << t << "][" << j << "]";
                model.addConstr(uniqueAffConst<=1,name.str());
                name.str("");
            }
        }

        std::cout << "--> Creating the unique ouverture constraint" << std::endl;
        for (int i=0; i<instance.nbDepotPotentiel; ++i){
            GRBLinExpr uniqueOuvConst = 0;
            for (int t=0; t<instance.nbPeriode; ++t){
                uniqueOuvConst += Y[t][i];
            }
            name << "unique ouverture constraint of Y[...][" << i << "]";
            model.addConstr(uniqueOuvConst<=1,name.str());
            name.str("");
        }

        std::cout <<"----> setting model" << std::endl;
        model.set(GRB_DoubleParam_TimeLimit, timeLimit);

        std::cout <<"----> model solving"<< std::endl;
        model.optimize();

        int status = model.get(GRB_IntAttr_Status);
        if (status == GRB_OPTIMAL || (status== GRB_TIME_LIMIT && model.get(GRB_IntAttr_SolCount)>0)){
            std::cout << "Succes! (Status: " << status << ")" << std::endl;
            std::cout<<"--> Printing results "<< std::endl;
            std::cout << "Runtime : " << model.get(GRB_DoubleAttr_Runtime) << " seconds"<< std::endl;

            model.write("solution.sol");
            std::cout << "Objective value = "<< model.get(GRB_DoubleAttr_ObjVal)  << std::endl;

            int counter = 0;

            std::vector<std::vector<std::vector<int>>> SX(instance.nbPeriode);
            std::vector<std::vector<int>> SY(instance.nbPeriode);
            for (int t=0; t<instance.nbPeriode; ++t){
                SX[t] = std::vector<std::vector<int>>(instance.nbDepotPotentiel);
                SY[t] = std::vector<int>(instance.nbDepotPotentiel);
                for (int i=0; i<instance.nbDepotPotentiel; ++i){
                    if (Y[t][i].get(GRB_DoubleAttr_X)>0){
                        std::cout << "Y[" << t << "][" << i << "] = 1" << std::endl;
                        SY[t][i] = 1;    
                    }else{
                        SY[t][i] = 0;
                    }
                    SX[t][i] = std::vector<int>(instance.nbClient);
                    for (int j=0; j<instance.nbClient; ++j){
                        if (X[t][i][j].get(GRB_DoubleAttr_X)>0){
                            std::cout << "X[" << t << "][" << i << "][" << j << "] = 1" << std::endl;
                            SX[t][i][j] = 1;
                            if (t==2)
                                counter++;
                        }else{
                            SX[t][i][j] = 0;
                        }
                    }
                }
            }
            returnSol = Solution(SY,SX);
            std::cout << "Solution feasability : " << returnSol.check(instance) << std::endl;
            // std::cout << "nmax = " << instance.n[instance.nbPeriode-1] << std::endl;
            // std::cout << counter << std::endl;
            // for (int t=0; t<instance.nbPeriode; ++t){
            //     std::cout << "p = " << instance.p[t] << std::endl;
            // }
        }
    }catch (GRBException e)
    {
        std::cout << "Error code = " << e.getErrorCode() << std::endl;
        std::cout << e.getMessage() << std::endl;
    }
    catch (...)
    {
        std::cout << "Exception during optimization" << std::endl;
    } 
    return returnSol;
}