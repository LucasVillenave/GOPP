#include "parser.hpp"
#include "FLPData.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
using namespace std;

vector<string> split(string s, char delim) {
    vector<string> words;
    stringstream ss(s);
    string word;
    while (getline(ss, word, delim)) {
        if (word!=""){
            words.push_back(word);
        }
    }
    return words;
}

FLPData* load(string instanceName){
    string prefixe = "../instances/";
    string suffixe = ".txt";
    string path = prefixe + instanceName + suffixe;
    
    string line;
    fstream file;
    vector<string> words;

    int nbClient;
    int nbPeriodes;
    int nbDepotsPotentiels;
    
    vector<vector<vector<int>>> c;
    vector<vector<int>> f;
    vector<int> p;
    vector<int> n;

    cout << "trying to load : " << path << endl;
    file.open(path);
    if (!file){
        cout<<"\ncouldn't load\n"<<endl;
    }else{
        cout<<"\nsuccessful load\n"<<endl;
    }

    //getting sizes of dimentions
    getline(file,line);
    words = split(line,' ');
    nbDepotsPotentiels = stoi(words.at(0));
    nbClient = stoi(words.at(1));
    nbPeriodes = stoi(words.at(2));

    //initializing with sizes
    n.resize(nbPeriodes,0);
    f.resize(nbDepotsPotentiels,n);
    p.resize(nbPeriodes,0);

    vector<vector<int>> token;
    token.resize(nbClient,p);

    c.resize(nbDepotsPotentiels,token);

    cout << "|I| = " << nbDepotsPotentiels << ", |J| = " << nbClient << ", and |T| = " << nbPeriodes << endl << endl;

    getline(file,line);
    words = split(line,' ');

    for (int i=0; i<nbPeriodes; ++i){
        p.at(i) = stoi(words.at(i));
    }

    getline(file,line);
    words = split(line,' ');

    for (int i=0; i<nbPeriodes; ++i){
        n.at(i) = stoi(words.at(i));
    }

    for (int i=0; i<nbDepotsPotentiels; ++i){
        getline(file,line);
        words = split(line,' ');

        for (int t=0; t<nbPeriodes; ++t){
            f.at(i).at(t) = stoi(words.at(t));
        }

        for (int j=0; j<nbClient; ++j){
            getline(file,line);
            words = split(line,' ');

            for (int t=0; t<nbPeriodes; ++t){
                c.at(i).at(j).at(t) = stoi(words.at(t));
            }
        }
    }

    FLPData* i = new FLPData(c,f,n,p,nbClient,nbDepotsPotentiels,nbPeriodes);
    return i;
}

void write(FLPSolution* sol, string solName){
    
}