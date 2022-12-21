using namespace std;

#include "gurobi_c++.h"
#include <vector>
#include <forward_list>
#include <sstream>


#include "FLPHeuristic.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// MAIN


FLPSolution FLPHeuristic::solve(FLPData instance){

	FLPSolution returnSol;

	try{

		///////////////////////////////////////
		// creation random
		// int graine = time(NULL);
		// cout << "graine : " << graine << endl << endl << endl;
		// srand(graine);


		///////////////////////////////////////
		// creation environment
		GRBEnv env(true);
        env.start();
		GRBModel model(env);


		///////////////////////////////////////
		// creation variables
		GRBVar ** Z = new GRBVar*[instance.nbPeriode];
		GRBVar ** Y = new GRBVar*[instance.nbPeriode];
        GRBVar *** X = new GRBVar**[instance.nbPeriode];
		std::stringstream name;
        for (int t=0; t<instance.nbPeriode; ++t){
            Z[t] = new GRBVar[instance.nbClient];
			for (int j=0; j<instance.nbClient; ++j){
                name << "Z[" << t << "][" << j << "]";
                Z[t][j] = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, name.str());
                name.str("");
			}
            Y[t] = new GRBVar[instance.nbDepotPotentiel];
            X[t] = new GRBVar*[instance.nbDepotPotentiel];
            for (int i=0; i<instance.nbDepotPotentiel; ++i){
                name << "Y[" << t << "][" << i << "]";
                Y[t][i] = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, name.str());
                name.str("");

                X[t][i] = new GRBVar[instance.nbClient];
                for (int j=0; j<instance.nbClient; ++j){
                    name << "X[" << t << "][" << i << "][" << j << "]";
                    X[t][i][j] = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS, name.str());
                    name.str("");
                }
            }
        }


		///////////////////////////////////////
		// constraint continuation flot

		// t = 0
		for (int j=0 ; j < instance.nbClient ; ++j){
			GRBLinExpr expe = Z[0][j];
			GRBLinExpr exps = 0;
			for (int i=0 ; i < instance.nbDepotPotentiel ; ++i){
				exps += X[0][i][j];
			}
			name << "flot[0][" << j << "]";
            model.addConstr(expe==exps,name.str());
            name.str("");
		}

		// 1 <- t -> T-1
		for (int t=1 ; t < instance.nbPeriode ; ++t){
			for (int j=0 ; j < instance.nbClient ; ++j){
				GRBLinExpr exp = Z[t][j];
				for (int i=0 ; i < instance.nbDepotPotentiel ; ++i){
					exp += X[t-1][i][j];
					exp -= X[t][i][j];
				}
				name << "flot[" << t << "][" << j << "]";
				model.addConstr(exp==0,name.str());
				name.str("");
			}
		}


		///////////////////////////////////////
		// constraint start / end flot		

		// sum Z
		for (int t=0 ; t < instance.nbPeriode ; ++t){
			GRBLinExpr exp = 0;
			for (int j=0 ; j < instance.nbClient ; ++j){
				exp += Z[t][j];
			}
			name << "sum_Z[" << t << "]";
            model.addConstr(exp==instance.n[t],name.str());
            name.str("");
		}

		// t = T
		for (int j=0 ; j < instance.nbClient ; ++j){
			GRBLinExpr exp = 0;
			for (int i=0 ; i < instance.nbDepotPotentiel ; ++i){
				exp += X[instance.nbPeriode-1][i][j];
			}
			name << "end_flot[" << j << "]";
            model.addConstr(exp==1,name.str());
            name.str("");
		}


		///////////////////////////////////////
		// constraint capacity
		for (int t=0; t<instance.nbPeriode; ++t){
            for (int i=0; i<instance.nbDepotPotentiel; ++i){
                for (int j=0; j<instance.nbClient; ++j){
                    GRBLinExpr exp = X[t][i][j];
                    for(int t2=0; t2<=t; ++t2){
                        exp -= Y[t2][i];
                    }
                    name << "capacity_X[" << t << "][" << i << "][" << j << "]";
                    model.addConstr(exp<=0,name.str());
                    name.str("");
                }
            }
        }


		///////////////////////////////////////
		// constraint Y

		// sum Y[t][.]
		for (int t=0; t<instance.nbPeriode; ++t){
            GRBLinExpr exp = 0;
            for (int i=0; i<instance.nbDepotPotentiel; ++i){
                exp += Y[t][i];
            }
            name << "Y[" << t << "][.]";
            model.addConstr(exp==instance.p[t],name.str());
            name.str("");
        }

		// sum Y[.][i]
		for (int i=0; i<instance.nbDepotPotentiel; ++i){
            GRBLinExpr exp = 0;
            for (int t=0; t<instance.nbPeriode; ++t){
                exp += Y[t][i];
            }
            name << "Y[.][" << i << "]";
            model.addConstr(exp<=1,name.str());
            name.str("");
        }


		///////////////////////////////////////
		// objectif
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


		///////////////////////////////////////
		// parameter


		///////////////////////////////////////
		// heuristic

		// initialisation des données utiles : si le centre i est deja ouvert (true) ou non (false)
		vector<bool> ouvert;
		ouvert.resize(instance.nbDepotPotentiel);
		for(int i = 0 ; i < instance.nbDepotPotentiel ; ++i){
			ouvert[i] = false;
		}
		
		// pour chaque periode de temps
		for(int t = 0 ; t < instance.nbPeriode ; ++t){
			
			// on doit ouvrir "instance.p[t]" centre , ouvertures garde en memoir le nombre actuel
			int ouvertures = 0;
			while ( ouvertures < instance.p[t] ){
				
				// on resout le modele
        		model.optimize();

				// on cherche les centre qui sont les plus suceptibles a etre ouvert selon la relaxation lineaire
				forward_list<int>  a_ouvrir;
				a_ouvrir.clear();
				int taille = 0;
				double maxi = 0.0000001;
				for(int i = 0 ; i < instance.nbDepotPotentiel ; ++i){
					if((maxi <= Y[t][i].get(GRB_DoubleAttr_X)) && (!ouvert[i])){
						if (maxi == Y[t][i].get(GRB_DoubleAttr_X)){
							a_ouvrir.push_front(i);
							++taille;
						}
						else{
							a_ouvrir.clear();
							maxi == Y[t][i].get(GRB_DoubleAttr_X);
							a_ouvrir.push_front(i);
							taille = 1;
						}
					}
				}

				// on les ouvres sans depacer le nombre a ouvrir (ce seront les dernier qui seront selectione en priorite)
				// ici on peut aussi les choisir de maniere aleatoire si il y en a trop
				forward_list<int>::iterator it = a_ouvrir.begin();
				int vvv=1;
				while(vvv==1){
					model.addConstr(Y[t][*it] >= 1);
					++ouvertures;
					ouvert[*it] = true;
					++it;
					if (((it==a_ouvrir.end())||(ouvertures>=instance.p[t]))){
						vvv=0;
					}
				}

			// si on en pas ouvert asses, on reresout le model pour mieux prendre en compte les ouvertures effectuees
			}

		}

		// on resout une derniere fois pour s assurer d avoir des valeurs de X entieres
		model.optimize();


		///////////////////////////////////////
		// extraction de la solution
		std::vector<std::vector<std::vector<int>>> SX(instance.nbPeriode);
        std::vector<std::vector<int>> SY(instance.nbPeriode);
        for (int t=0; t<instance.nbPeriode; ++t){
            SX[t] = std::vector<std::vector<int>>(instance.nbDepotPotentiel);
            SY[t] = std::vector<int>(instance.nbDepotPotentiel);
            for (int i=0; i<instance.nbDepotPotentiel; ++i){
                if (Y[t][i].get(GRB_DoubleAttr_X)>0.5){
                    std::cout << "Y[" << t << "][" << i << "] = 1" << std::endl;
                    SY[t][i] = 1;    
                }else{
                    SY[t][i] = 0;
                }
                SX[t][i] = std::vector<int>(instance.nbClient);
                for (int j=0; j<instance.nbClient; ++j){
                    if (X[t][i][j].get(GRB_DoubleAttr_X)>0.5){
                        std::cout << "X[" << t << "][" << i << "][" << j << "] = 1" << std::endl;
                        SX[t][i][j] = 1;
                    }else{
                        SX[t][i][j] = 0;
                    }
                }
            }
        }
        returnSol = FLPSolution(SY,SX);
	
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