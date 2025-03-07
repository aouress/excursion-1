// good video on headers: https://www.youtube.com/watch?v=9RJTQmK0YPI
// also a useful article on headers: https://learn.microsoft.com/en-us/cpp/cpp/header-files-cpp?view=msvc-170
#include "parse.h" // include header file with the parsing function declarations
#include <iostream>
#include <fstream> // fstream documentation | https://cplusplus.com/reference/fstream/
#include <string>  // string documentation  | https://cplusplus.com/reference/string/
#include <vector>  // vector documentation  | https://cplusplus.com/reference/vector/
#include <cstdlib> // cstdlib documentation | https://cplusplus.com/reference/cstdlib/

using namespace std;

void insertMMatrix(vector<vector<double>>& T, int max_node, int length) {
    int startRow = max_node+length;  //The index of the row is after max_node and length
    int startCol = max_node;         //Places identity matrix to the right by the size of max_node

    for (int i = 0; i < length; i++) {
        T[startRow + i][startCol + i] = 1;  //Diagonally adds 1s
    }
}

void back_sub(vector<vector<double>> &Tu, int row, int col); 

void interchange_row(vector<vector<double>> &Tu, int row_1, int row_2); 

void ref_matrix(vector<vector<double>> &Tu, int max_node, int length, int row, int col); 

void rref_matrix(vector<vector<double>> &Tu, int row, int col);

void scale_replace_row (vector<vector<double>> &Tu, int row_1, int row_2, int current_col, int max_col);

void matrix_threshold (vector<vector<double>> &Tu, int row, int col);

bool valid_matrix(vector<vector<double>> &Tu, int row, int col); 

int matrix_rank(vector<vector<double>> &Tu, int row, int col);

//finds the source node and compares to previous max 
int find_max_source_node(string line, int max_node) {
    
    // finds source node and returns as int
    int s_node = get_sord(line, 's'); 

    //new max node found if equality holds 
    if (s_node > max_node) {
        max_node = s_node;  
    }
    return max_node; 
}

//given a netlist file, read and write the data into appropriate location in the matrix 
int read_file() {
    fstream netlist;
    int length = 0;
    int max_node = 0; 

    //----

    // determine matrix dimensions (could be separate function)
    netlist.open("netlist.txt", ios::in); //read file
    if (netlist.is_open()) {
        string line; 

        //determine number of components for matrix size by counting each line
        while (getline(netlist, line)) {
            //count a line 
            length += 1;

            // size of matrix also depends on maximum node value of all source nodes
            // Therefore, finding the maximum source node is equivalent to finding the number of rows of matrix A
            max_node = find_max_source_node(line, max_node); 
        }
        
        netlist.close();
    }
    else {
        cout << "Error while opening file. Check file directory and name" << endl; 
    }
    //calculate dimension of T matrix | row: # of rows; col: # of cols
    // Including the solution column, the dimension of the augmented matrix of [T][e] = [0], [T | 0],
    // row # of rows and row + 1 # of columns 
    int row = 2*length + max_node;    
    int col = row + 1; 

    //----- 
    
    // Declares a nested vector that has row number of row vectors to form the [T | u]
    // augmented matrix
    vector<vector<double>> Tu(row, vector<double>(col, 0)); 

    Tu = {
        {2, 3, -1, 4, 5, 0, -2, 1, 7},
        {1, -2, 3, -4, 5, -6, 7, -8, 9},
        {0, 5, -2, 3, 1, -4, 6, 2, -3},
        {4, -1, 2, -5, 3, 6, -7, 8, 0},
        {3, 2, -4, 1, -6, 5, -7, 9, -8},
        {-2, 4, -1, 3, -5, 2, 6, -8, 7},
        {5, -3, 2, -1, 4, -7, 8, -6, 9},
        {1, 6, -5, 3, -2, 7, -4, 0, 8}
    };

    //insertMMatrix(Tu, max_node, length); 

    // output rows 
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j ++) {
            cout << Tu[i][j] << " "; 
        }
        cout << endl;
    }  
    cout << endl; 

    //compute reduced echelon form of the matrix
    ref_matrix(Tu, max_node, length, row, col); 

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j ++) {
            cout << Tu[i][j] << " "; 
        }
        cout << endl;
    }  
    cout << endl; 

    // check if the matrix has a unique solution 
    if (!valid_matrix(Tu, row, col)) {
        cout << "The netlist provided describes an invalid circuit configuration" << endl; 
        return 0;
    }

    // compute the reduced row echelon form of the matrix
    rref_matrix(Tu, row, col); 

    // output rows 
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j ++) {
            cout << Tu[i][j] << " "; 
        }
        cout << endl;
    }
    cout << endl;

    vector<double> solution_vector(row); 

    for (int i = 0; i < row; i ++) {
        solution_vector[i] = Tu[i][col-1]; 
        cout << solution_vector[i] << " ";
    }
    cout << endl;
    return 1;
}

int main() {
    read_file();
}

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

void rref_matrix(vector<vector<double>> &Tu, int row, int col) {

    // check if the matrix has a unique solution 
    if (!valid_matrix(Tu, row, col)) {
        cout << "The netlist provided describes an invalid circuit configuration" << endl; 
    }
    // back sub the ref matrix to obtain the rref matrix
    back_sub(Tu, row, col); 
}