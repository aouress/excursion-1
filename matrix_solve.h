// contains function signatures used to solve and verify the [T | u] matrix
#ifndef MATRIX_SOLVE
#define MATRIX_SOLVE

#include<vector>
using namespace std;

// ----------------- ELEMENTARY ROW OPERATIONS ------------------------------------

void scale_replace_row (vector<vector<double>> &Tu, int row_1, int row_2, int current_col, int max_col);

void scale_row (vector<vector<double>> &Tu, int row, int col, int max_col); 

void interchange_row(vector<vector<double>> &Tu, int row_1, int row_2); 

// ----------------- MATRIX SOLUTION METHODS --------------------------------------

void ref_matrix(vector<vector<double>> &Tu, int max_node, int length, int row, int col); 

void back_sub(vector<vector<double>> &Tu, int row, int col); 

// ----------------- MATRIX CHECKS AND THRESHOLD ----------------------------------

void matrix_threshold (vector<vector<double>> &Tu, int row, int col);

bool valid_matrix(vector<vector<double>> &Tu, int row, int col); 

int matrix_rank(vector<vector<double>> &Tu, int row, int col);

#endif