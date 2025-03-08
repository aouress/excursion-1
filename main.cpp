// good video on headers: https://www.youtube.com/watch?v=9RJTQmK0YPI
// also a useful article on headers: https://learn.microsoft.com/en-us/cpp/cpp/header-files-cpp?view=msvc-170

#include "parse.h" // include header file with the parsing function declarations
#include "matrix_solve.h" // include header file with matrix solving function declarations
#include "code_tu_matrix.h" // include header file with matrix coding functions 
#include <iostream>
#include <fstream> // fstream documentation | https://cplusplus.com/reference/fstream/
#include <string>  // string documentation  | https://cplusplus.com/reference/string/
#include <vector>  // vector documentation  | https://cplusplus.com/reference/vector/
#include <cstdlib> // cstdlib documentation | https://cplusplus.com/reference/cstdlib/
#include <sstream> // sstream documentation | https://cplusplus.com/reference/sstream/

using namespace std;

 
/*void print_matrix (vector<vector<double>> &Tu, int row, int col) {
    // prints matrix given max row and col values  
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j ++) {
            cout << Tu[i][j] << " "; 
        }
        cout << endl;
    }  
    cout << endl; 
}*/

void write_file(vector<double> &s, int row) {
    // writes output.txt file given the solution vector and row count 
    fstream out_file;
    string file_name = "output.txt"; 

    // open the new output file and configure it such that we can write to it 
    out_file.open(file_name, ios::out);
    if (out_file.is_open()) {
        // for each element in the solution vector, write the value with a space between in the output.txt file 
        for (int i = 0; i < row; i++) {
            out_file << s[i] << " "; 
        }
    }
    else {
        cout << "Error opening output file." << endl; 
    }
    out_file.close(); 
}

//given a netlist file, read and write the data into appropriate location in the matrix 
int main() {
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

    // code KCL, KVL, and Ohm's law into the [T | u] matrix 
    add_KCL_KVL(Tu, max_node, length); 
    addIs(Tu, max_node, length); 
    addMu(Tu, max_node, length); 
    addN(Tu, max_node, length); 
    
    
    //compute row echelon form of the matrix
    ref_matrix(Tu, max_node, length, row, col);  

    //check if the matrix has a unique solution 
    if (!valid_matrix(Tu, row, col)) {
        cout << "The netlist provided describes an invalid circuit configuration" << endl; 
        return 0;
    }

    // compute the reduced row echelon form of the matrix by back substitution of the row echelon matrix 
    back_sub(Tu, row, col); 

    // declare a vector to hold the u column in the [T | u] matrix 
    vector<double> solution_vector(row); 

    // assigning the solution vector to be the last column of the Tu matrix (u column) 
    for (int i = 0; i < row; i ++) {
        solution_vector[i] = Tu[i][col-1]; 
    }

    // create an output.txt file that contains the solution vector. 
    write_file(solution_vector, row); 
    return 1;
}
