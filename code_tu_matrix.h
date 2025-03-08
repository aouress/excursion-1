// holds all function signatures for coding the [T | u] matrix 
#ifndef CODE_T_MATRIX
#define CODE_T_MATRIX

#include <vector> 
using namespace std;

// Function that places the A and -A^T matrices into [T | u] as codes of Kirchhoff's Current and Voltage Laws 
void add_KCL_KVL(vector<vector<double>> &Tu, int max_node, int length); 

// adds identity matrix and M matrix to the [T | u] matrix 
// for resistive circuits with only voltage sources and resistors, the M matrix will always be an identity matrix 
// the M matrix is 1/3 parts for coding Ohm's law
void addIs(vector<vector<double>>& Tu, int max_node, int length); 

// adds mu_s column vector to the [ T | u ] matrix, this function is 1/3 parts for coding Ohm's law
void addMu(vector<vector<double>>& Tu, int max_node, int length); 

// adds the N matrix to the [T | u] matrix, this function is 1/3 parts for coding Ohm's law 
void addN(vector<vector<double>>& Tu, int max_node, int length); 

#endif