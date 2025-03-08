// holds all function signatures for the parsing functions 
#ifndef PARSE_H
#define PARSE_H
#include <string>

using namespace std;

int get_sord(string line, char code); 

char get_type(string line); 

float get_value(string line);  

//finds the source node and compares to previous max 
int find_max_source_node(string line, int max_node);

#endif