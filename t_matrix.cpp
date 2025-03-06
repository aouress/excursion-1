// good video on headers: https://www.youtube.com/watch?v=9RJTQmK0YPI
// also a useful article on headers: https://learn.microsoft.com/en-us/cpp/cpp/header-files-cpp?view=msvc-170
#include "parse.h" // include header file with the parsing function declarations
#include <iostream>
#include <fstream> // fstream documentation | https://cplusplus.com/reference/fstream/
#include <string>  // string documentation  | https://cplusplus.com/reference/string/
#include <vector>  // vector documentation  | https://cplusplus.com/reference/vector/
#include <cstdlib> // cstdlib documentation | https://cplusplus.com/reference/cstdlib/

using namespace std;

void insertMMatrix(vector<vector<float>>& T, int max_node, int length) {
    int startRow = max_node+length;  //The index of the row is after max_node and length
    int startCol = max_node;         //Places identity matrix to the right by the size of max_node

    for (int i = 0; i < length; i++) {
        T[startRow + i][startCol + i] = 1;  //Diagonally adds 1s
    }
}

int solve_matrix(vector<vector<float>> &T, int max_node, int length); 

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
void read_file() {
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
    vector<vector<float>> Tu(row, vector<float>(col, 0)); 

    insertMMatrix(Tu, max_node, length); 

    // output rows 
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j ++) {
            cout << Tu[i][j] << " "; 
        }
        cout << endl;
    }
}

int main() {
    read_file();
}

int solve_matrix(vector<vector<float>> &T, int max_node, int length) {
    //iterate through each row. Find a nonzero pivot in the column. increment a pivot row counter.  
}