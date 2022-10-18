#ifndef PARSER
#define PARSER

#include <string>
#include <vector>

class Solution;
class Instance;


//load instance from instances/
Instance* load(std::string path);

//write Solution to solutions/
void clean(Solution* sol);

#endif
