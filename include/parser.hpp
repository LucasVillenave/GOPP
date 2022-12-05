#ifndef PARSER
#define PARSER

#include <string>
#include <vector>

class FLPSolution;
class FLPData;


//load instance from instances/
FLPData* load(std::string path);

//write Solution to solutions/
void write(FLPSolution* sol, std::string solName);

#endif
