using namespace std;

#include <ilcplex/ilocplex.h>
#include <ilcplex/ilocplexi.h>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <limits>
#include <queue>
#include <algorithm>
#include <utility>
#include <tuple>


#include "FLPHeuristic.hpp"


ILOSTLBEGIN


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// MAIN


FLPSolution FLPHeuristic::solve(FLPData instance){

	FLPSolution returnSol;

	try{

		///////////////////////////////////////
		// creation random
		int graine = time(NULL);
		cout << "graine : " << graine << endl << endl << endl;
		srand(graine);


		///////////////////////////////////////
		// creation environment
		IloEnv env;
		IloModel model ( env );
		IloCplex cplex ( model );


		///////////////////////////////////////
		// creation variables
		vector<IloNumVar> z;
		vector<vector<vector<IloNumVar>>> x;
		z.resize(instance.nbPeriode);
		y.resize(instance.nbPeriode);
		x.resize(instance.nbPeriode);
		for (int t=0 ; t < instance.nbPeriode ; ++t){
			char name[20];
			sprintf(name,"z_%d",t);
			z[t] = IloNumVar(env, 0.0, 1.0,IloNumVar::Float,name);
			model.add(z[t]);
			y.resize(instance.nbDepotPotentiel);
			x.resize(instance.nbDepotPotentiel);
			for (int i=0 ; i < instance.nbDepotPotentiel ; ++i){
				char name[20];
				sprintf(name,"y_%d_%d",t,i);
				y[t] = IloNumVar(env, 0.0, 1.0,IloNumVar::Float,name);
				model.add(y[t]);
				x.resize(instance.nbDepotPotentiel);
				for (int j=0 ; j < instance.nbClient ; ++j){
					char name[20];
					sprintf(name,"x_%d_%d_%d",t,i,j);
					x[t][i][j] = IloNumVar(env, 0.0, 1.0,IloNumVar::Float,name);
					model.add(x[t][i][j]);
				}
			}
		}


		///////////////////////////////////////
		// constraint continuation flot

		// t = 0
		for (int j=0 ; j < instance.nbClient ; ++j){
			IloExpr expe(env);
			IloExpr exps(env);
			expe = z[0][j];
			exps = 0;
			for (int i=0 ; i < instance.nbDepotPotentiel ; ++i){
				exps += x[t+1][i][j]
			}
			model.add(expe == exps);
		}

		// 1 <- t -> T-1
		for (int t=1 ; t < instance.nbPeriode-1 ; ++t){
			for (int j=0 ; j < instance.nbClient ; ++j){
				IloExpr expe(env);
				IloExpr exps(env);
				expe = z[t][j];
				exps = 0;
				for (int i=0 ; i < instance.nbDepotPotentiel ; ++i){
					expe += x[t][i][j];
					exps += x[t+1][i][j]
				}
				model.add(expe == exps);
			}
		}


		///////////////////////////////////////
		// constraint start and end flot		

		// sum z
		for (int t=0 ; t < instance.nbPeriode-1 ; ++t){
			IloExpr exp(env);
			exp = 0;
			for (int j=0 ; j < instance.nbClient ; ++j){
				exp += z[t][j];
			}
			model.add(exp == instance.n[t]);
		}

		// t = T
		for (int j=0 ; j < instance.nbClient ; ++j){
			IloExpr exp(env);
			exp = 0;
			for (int i=0 ; i < instance.nbDepotPotentiel ; ++i){
				exp += x[instance.nbPeriode-1][i][j];
			}
			model.add(exp == 1);
		}


		///////////////////////////////////////
		// constraint capacity
		for (int t=0 ; t < instance.nbPeriode ; ++t){
			for (int i=0 ; i < instance.nbDepotPotentiel ; ++i){
				for (int j=0 ; j < instance.nbClient ; ++j){
					IloExpr exp(env);
					exp = 0;
					for (int t_prime=0 ; t_prime <= t ; ++t_prime){
						exp += y[t_prime][i][j];
					}
					model.add(x[t][i][j] <= exp);
				}
			}
		}


		///////////////////////////////////////
		// objectif
		IloExpr obj(env);
		obj = 0;
		for (int t=0 ; t < instance.nbPeriode ; ++t){
			for (int i=0 ; i < instance.nbDepotPotentiel ; ++i){
				obj += y[t][i] * instance.f[t][i];
				for (int j=0 ; j < instance.nbClient ; ++j){
					obj += x[t][i][j] * instance.c[t][i][j];
				}
			}
		}
		model.add(IloMaximize(env,obj));


		///////////////////////////////////////
		// parameter
		cplexTF.setParam(IloCplex::Param::MIP::Display,0);



		///////////////////////////////////////
		// heuristic
		
		for(int t = 0, t < instance.nbPeriode, ++t){
			
			cplexTF.solve();  


		}
	
	}

	return returnSol;
}