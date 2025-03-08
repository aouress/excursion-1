// contains function defintions used to solve and verify the [T | u] matrix
#include "matrix_solve.h"
#include <iostream>
#include <fstream> // fstream documentation | https://cplusplus.com/reference/fstream/
#include <string>  // string documentation  | https://cplusplus.com/reference/string/
#include <vector>  // vector documentation  | https://cplusplus.com/reference/vector/
#include <cstdlib> // cstdlib documentation | https://cplusplus.com/reference/cstdlib/

using namespace std;

// ------------------------------ ELEMENTARY ROW OPERATIONS ----------------------------------

void interchange_row (vector<vector<double>> &Tu , int row_1, int row_2) {
    // swap row 1 and row 2 of the 2D vector matrix 
    vector<double> temp = Tu[row_1]; 
    Tu[row_1] = Tu[row_2]; 
    Tu[row_2] = temp; 

}

void scale_replace_row (vector<vector<double>> &Tu, int row_1, int row_2, int current_col, int max_col) {
    // find a scalar multiple, c, such that the elements in col sum to zero 
    // replace row_2 such that c*row_1 + row_2
    double c = -Tu[row_2][current_col]/Tu[row_1][current_col]; 

    //apply element wise scalar multiplication to row_2
    for (int i = 0; i < max_col; i++) {
        Tu[row_2][i] = c*Tu[row_1][i] + Tu[row_2][i]; 
    }
}

void scale_row (vector<vector<double>> &Tu, int row, int col, int max_col) {
    // scales the row such that the scalar multiple is the reciprocal of the row index
    double c = Tu[row][col]; 

    for (int i = 0; i < max_col; i++) {
        Tu[row][i] = Tu[row][i]/c; 
    }
}

// --------------------------- END OF ELEMENTARY ROW OPERATIONS -----------------------------------

// --------------------------- MATRIX SOLUTION METHODS --------------------------------------------

void ref_matrix(vector<vector<double>> &Tu, int max_node, int length, int row, int col) {
    //function to compute the reduced echelon form of the [T | u] augmented matrix 
    
    // counter to store how many pivots we've placed 
    int piv_row = 0;

    // the counter incremented when first nonzero element is swapped and also serves as a check to see if we've already swapped rows in the column 
    int nonzero_count = 0; 

    //iterate through each column and search for a nonzero element in the rows, then clear the remaining elements in the column 
    // the last column of the Tu matrix corresponds to the solution vector, so we do not need to iterate over it
    for (int j = 0; j < col-1; j ++) {
        for (int i = 0; i < row; i++) {
            // row interchange
            // find non zero element
            // if i is larger than or equal to the piv_row it will either interchange the same row, 
            // or interchange the ith row with the piv_row row. 
            if (Tu[i][j] != 0 && i >= piv_row && nonzero_count == 0) {
                interchange_row(Tu, i, piv_row); 

                // column has at least one nonzero element
                nonzero_count += 1;
                // nonzero element in correct pivot position 
                piv_row += 1;
            }

            // clear remaining nonzero elements from the column so long as a row has been interchanged
            // since we've already found the pivot for this column, we can assume all other 
            // nonzero elements come after the initial position of the first nonzero element 
            if(Tu[i][j] != 0 && nonzero_count == 1 && i != piv_row - 1) {
                scale_replace_row(Tu, piv_row - 1, i, j, col); 
            }
        }
        nonzero_count = 0;
    }

    matrix_threshold(Tu, row, col);
}

void back_sub(vector<vector<double>> &Tu, int row, int col) {
    // assume the matrix is in ref form with all pivots along the main diagonal of the coefficient matrix

    int piv_row = row - 1; 
    int piv_col = col - 2;

    //start in bottom corner of coefficient matrix 
    for (int j = col - 2; j >=0; j--) {
        for (int i = piv_row - 1; i >= 0; i--) {
            // scale and add the pivot row to each row whose column element is nonzero 
            // this adjusts all upper triangular matrix elements to zero 
            if (Tu[i][j] != 0) {
                scale_replace_row(Tu, piv_row, i, j, col);
            }
                 
        }
        //scale row to get a 1 in the pivot position  
        scale_row(Tu, piv_row, j, col);
        // decrement pivot row and piv col to move diagonally up the matrix
        piv_row -= 1;
        piv_col -= 1;

    }
    matrix_threshold(Tu, row, col); 
}

// -------------------------- END OF MATRIX SOLUTION METHODS -----------------------------------

// -------------------------- MATRIX CHECKS AND THRESHOLDS -------------------------------------

bool valid_matrix (vector<vector<double>> &Tu, int row, int col) {
    
    // if the rank of the coefficient matrix T is equal to the rank of the augmented matrix [ T | u ] the circuit is well defined 
    // essentially, this means we have no rows of the form [0 ... 0 | b ], so the system has a unique solution.
    if (matrix_rank(Tu, row, col) == matrix_rank(Tu, row, col-1)) {
        return true;
    }
    return false;
}

void matrix_threshold (vector<vector<double>> &Tu, int row, int col) {
    // upon testing the function with an arbitrary matrix, some values that should be exactly zero (and are very close to being zero; e.g., 1e^-9) are not zero. 
    // this final portion of the function checks a threshold value and assigns very very small values to 0.0 
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j ++) {
            if (abs(Tu[i][j]) < 1e-9) {
                Tu[i][j] = 0.0;
            }
    }
    }
}

int matrix_rank(vector<vector<double>> &Tu, int row, int col) {
    // the rank of a matrix is the number of pivots in the matrix 
    // this function counts the number of pivots and returns it as the rank

    // for each row, iterate through the columns until you see a nonzero value 
    // when you see a nonzero value, increment the pivot counter
    int pivot = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            
            if (Tu[i][j] != 0 && i >= pivot) {
                // nonzero element in correct pivot position 
                pivot += 1;
            }
        }
    }
    return pivot;
}

// --------------------------- END OF MATRIX CHECKS & THRESHOLDS ----------------------------------