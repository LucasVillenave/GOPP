// using namespace std;

// #include <ilcplex/ilocplex.h>
// #include <ilcplex/ilocplexi.h>
// #include <fstream>
// #include <string>
// #include <vector>
// #include <list>
// #include <limits>
// #include <queue>
// #include <algorithm>
// #include <utility>
// #include <tuple>


// #include "FLPHeuristic.hpp"


// ILOSTLBEGIN


// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// // MAIN


// FLPSolution FLPHeuristic::solve(FLPData instance){

// 	FLPSolution returnSol;

// 	try{

// 		///////////////////////////////////////
// 		// creation random
// 		// int graine = time(NULL);
// 		// cout << "graine : " << graine << endl << endl << endl;
// 		// srand(graine);


// 		///////////////////////////////////////
// 		// creation environment
// 		IloEnv env;
// 		IloModel model ( env );
// 		IloCplex cplex ( model );


// 		///////////////////////////////////////
// 		// creation variables
// 		vector<IloNumVar> z;
// 		vector<vector<vector<IloNumVar>>> x;
// 		z.resize(instance.nbPeriode);
// 		y.resize(instance.nbPeriode);
// 		x.resize(instance.nbPeriode);
// 		for (int t=0 ; t < instance.nbPeriode ; ++t){
// 			char name[20];
// 			sprintf(name,"z_%d",t);
// 			z[t] = IloNumVar(env, 0.0, 1.0,IloNumVar::Float,name);
// 			model.add(z[t]);
// 			y.resize(instance.nbDepotPotentiel);
// 			x.resize(instance.nbDepotPotentiel);
// 			for (int i=0 ; i < instance.nbDepotPotentiel ; ++i){
// 				char name[20];
// 				sprintf(name,"y_%d_%d",t,i);
// 				y[t] = IloNumVar(env, 0.0, 1.0,IloNumVar::Float,name);
// 				model.add(y[t]);
// 				x.resize(instance.nbDepotPotentiel);
// 				for (int j=0 ; j < instance.nbClient ; ++j){
// 					char name[20];
// 					sprintf(name,"x_%d_%d_%d",t,i,j);
// 					x[t][i][j] = IloNumVar(env, 0.0, 1.0,IloNumVar::Float,name);
// 					model.add(x[t][i][j]);
// 				}
// 			}
// 		}


// 		///////////////////////////////////////
// 		// constraint continuation flot

// 		// t = 0
// 		for (int j=0 ; j < instance.nbClient ; ++j){
// 			IloExpr expe(env);
// 			IloExpr exps(env);
// 			expe = z[0][j];
// 			exps = 0;
// 			for (int i=0 ; i < instance.nbDepotPotentiel ; ++i){
// 				exps += x[t+1][i][j]
// 			}
// 			model.add(expe == exps);
// 		}

// 		// 1 <- t -> T-1
// 		for (int t=1 ; t < instance.nbPeriode-1 ; ++t){
// 			for (int j=0 ; j < instance.nbClient ; ++j){
// 				IloExpr expe(env);
// 				IloExpr exps(env);
// 				expe = z[t][j];
// 				exps = 0;
// 				for (int i=0 ; i < instance.nbDepotPotentiel ; ++i){
// 					expe += x[t][i][j];
// 					exps += x[t+1][i][j]
// 				}
// 				model.add(expe == exps);
// 			}
// 		}


// 		///////////////////////////////////////
// 		// constraint start / end flot		

// 		// sum z
// 		for (int t=0 ; t < instance.nbPeriode-1 ; ++t){
// 			IloExpr exp(env);
// 			exp = 0;
// 			for (int j=0 ; j < instance.nbClient ; ++j){
// 				exp += z[t][j];
// 			}
// 			model.add(exp == instance.n[t]);
// 		}

// 		// t = T
// 		for (int j=0 ; j < instance.nbClient ; ++j){
// 			IloExpr exp(env);
// 			exp = 0;
// 			for (int i=0 ; i < instance.nbDepotPotentiel ; ++i){
// 				exp += x[instance.nbPeriode-1][i][j];
// 			}
// 			model.add(exp == 1);
// 		}


// 		///////////////////////////////////////
// 		// constraint capacity
// 		for (int t=0 ; t < instance.nbPeriode ; ++t){
// 			for (int i=0 ; i < instance.nbDepotPotentiel ; ++i){
// 				for (int j=0 ; j < instance.nbClient ; ++j){
// 					IloExpr exp(env);
// 					exp = 0;
// 					for (int t_prime=0 ; t_prime <= t ; ++t_prime){
// 						exp += y[t_prime][i];
// 					}
// 					model.add(x[t][i][j] <= exp);
// 				}
// 			}
// 		}


// 		///////////////////////////////////////
// 		// constraint y

// 		// sum y[t][.]
// 		for (int t=0 ; t < instance.nbPeriode ; ++t){
// 			IloExpr exp(env);
// 			exp = 0;
// 			for (int i=0 ; i < instance.nbDepotPotentiel ; ++i){
// 				exp += y[t][i];
// 			}
// 			model.add(exp == instance.p[t]);
// 		}

// 		// sum y[.][i]
// 		for (int i=0 ; i < instance.nbDepotPotentiel ; ++i){
// 			IloExpr exp(env);
// 			exp = 0;
// 			for (int t=0 ; t < instance.nbPeriode ; ++t){
// 				exp += y[t][i];
// 				}
// 			model.add(exp <= 1);
// 		}


// 		///////////////////////////////////////
// 		// objectif
// 		IloExpr obj(env);
// 		obj = 0;
// 		for (int t=0 ; t < instance.nbPeriode ; ++t){
// 			for (int i=0 ; i < instance.nbDepotPotentiel ; ++i){
// 				obj += y[t][i] * instance.f[t][i];
// 				for (int j=0 ; j < instance.nbClient ; ++j){
// 					obj += x[t][i][j] * instance.c[t][i][j];
// 				}
// 			}
// 		}
// 		model.add(IloMaximize(env,obj));


// 		///////////////////////////////////////
// 		// parameter
// 		cplex.setParam(IloCplex::Param::MIP::Display,0);


// 		///////////////////////////////////////
// 		// heuristic

// 		// initialisation des données utiles : si le centre i est deja ouvert (true) ou non (false)
// 		vector<bool> ouvert;
// 		ouvert.resize(instance.nbDepotPotentiel);
// 		for(int i = 0 ; i < instance.nbDepotPotentiel ; ++i){
// 			ouvert[i] = false;
// 		}
		
// 		// pour chaque periode de temps
// 		for(int t = 0 ; t < instance.nbPeriode ; ++t){
			
// 			// on doit ouvrir "instance.p[t]" centre , ouvertures garde en memoir le nombre actuel
// 			int ouvertures = 0;
// 			while ( ouvertures < instance.p[t] ){
				
// 				// on resout le modele
// 				cplex.solve();  

// 				// on cherche les centre qui sont les plus suceptibles a etre ouvert selon la relaxation lineaire
// 				forward_list<int>  a_ouvrir;
// 				a_ouvrir.clear();
// 				int taille = 0;
// 				double maxi = 0.0000001;
// 				for(int i = 0 ; i < instance.nbDepotPotentiel ; ++i){
// 					if((maxi <= cplex.getValue(y[t][i])) && (!ouvert[i])){
// 						if (maxi == cplex.getValue(y[t][i])){
// 							a_ouvrir.push_front(i);
// 							++taille;
// 						}
// 						else{
// 							a_ouvrir.clear();
// 							maxi == cplex.getValue(y[t][i]);
// 							a_ouvrir.push_front(i);
// 							taille = 1;
// 						}
// 					}
// 				}

// 				// on les ouvres sans depacer le nombre a ouvrir (ce seront les dernier qui seront selectione en priorite)
// 				// ici on peut aussi les choisir de maniere aleatoire si il y en a trop
// 				for(forward_list<int>::iterator it = a_ouvrir.begin() ; ((it!=a_ouvrir.end())&&(ouvertures<instance.p[t])) ; ++it){
// 					model.add(y[t][*it] >= 1);
// 					++ouvertures;
// 					ouvert[*it] = true;
// 				}

// 			// si on en pas ouvert asses, on reresout le model pour mieux prendre en compte les ouvertures effectuees
// 			} 

// 		}

// 		// on resout une derniere fois pour s assurer d avoir des valeurs de X entieres
// 		cplex.solve(); 


// 		///////////////////////////////////////
// 		// extraction de la solution
// 		std::vector<std::vector<std::vector<int>>> SX(instance.nbPeriode);
//         std::vector<std::vector<int>> SY(instance.nbPeriode);
// 		for (int t=0; t<instance.nbPeriode; ++t){
//             SX[t] = std::vector<std::vector<int>>(instance.nbDepotPotentiel);
//             SY[t] = std::vector<int>(instance.nbDepotPotentiel);
//             for (int i=0; i<instance.nbDepotPotentiel; ++i){
//                 if (cplex.getValue(y[t][i])>0.5){
//                     //std::cout << "Y[" << t << "][" << i << "] = 1" << std::endl;
//                     SY[t][i] = 1;    
//                 }else{
//                     SY[t][i] = 0;
//                 }
//                 SX[t][i] = std::vector<int>(instance.nbClient);
//                 for (int j=0; j<instance.nbClient; ++j){
//                     if (cplex.getValue(x[t][i][j])>0.5){
//                         //std::cout << "X[" << t << "][" << i << "][" << j << "] = 1" << std::endl;
//                         SX[t][i][j] = 1;
//                         if (t==2)
//                             counter++;
//                     }else{
//                         SX[t][i][j] = 0;
//                     }
//                 }
//             }
//         }
//         returnSol = FLPSolution(SY,SX);
	
// 	}

// 	return returnSol;
// }