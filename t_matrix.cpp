// good video on headers: https://www.youtube.com/watch?v=9RJTQmK0YPI
// also a useful article on headers: https://learn.microsoft.com/en-us/cpp/cpp/header-files-cpp?view=msvc-170

#include "parse.h" // include header file with the parsing function declarations
#include "matrix_solve.h" // include header file with matrix solving function declarations
#include <iostream>
#include <fstream> // fstream documentation | https://cplusplus.com/reference/fstream/
#include <string>  // string documentation  | https://cplusplus.com/reference/string/
#include <vector>  // vector documentation  | https://cplusplus.com/reference/vector/
#include <cstdlib> // cstdlib documentation | https://cplusplus.com/reference/cstdlib/
#include <sstream> // sstream documentation | https://cplusplus.com/reference/sstream/

using namespace std;

void addA_AT_Matrix(vector<vector<double>> &Tu, int max_node, int length) {
    // Places A into Tu
    fstream netlist;

    netlist.open("netlist.txt", ios::in);
    if (netlist.is_open()) {
        string line; 
        
            for(int i = 0; i < length; i++) {
                getline(netlist, line);

                Tu[get_sord(line, 's')-1][i + max_node + length] = 1;
                
                if (get_sord(line, 'd') != 0){
                        Tu[get_sord(line, 'd')-1][i + max_node + length] = -1;
                }
               
            }
       // Tu[max_node][length] =  get_sord (line, 's');
    }
}

// adds identity matrix and M matrix to Tu matrix
void addM_IMatrix(vector<vector<double>>& Tu, int max_node, int length) {
    int startRow = max_node+length; //The index of the row is after max_node and length
    int startCol = max_node; //Places identity matrix to the right by the size of max_node

    for (int i = 0; i<length; i++) {
        Tu[startRow+i][startCol+i] = 1; //Diagonally adds 1s
    }

    startRow = max_node; //The index of the row is after max_node
    startCol = max_node; //Places identity matrix to the right by the size of max_node

    //Adds an identity matrix in the center
    for (int i = 0; i<length; i++) {
        Tu[startRow+i][startCol+i] = 1; //Diagonally adds 1s
    }
}

void addMu(vector<vector<double>>& Tu, int max_node, int length) {

    //Reading the text file and storing voltages
    string stream; //String to hold the data in the text file
    ifstream MyReadFile("netlist.txt"); //reading the text file

    vector<double> voltages; //vector to store voltages in the string

    //Reading the file line by line
    while (getline(MyReadFile, stream)) {
        istringstream iss(stream);
        string component;
        int node1, node2;
        double value;

        // Extract values from line
        if (iss >>component >>node1 >>node2 >>value) {
            //If the component starts with 'V', then save the value
            if (component[0] == 'V') {
                voltages.push_back(value);
            }
            //If not, place a '0' as a placeholder
            else {
                voltages.push_back(0);
            }
        }
    }

    //Close the file
    MyReadFile.close();

    //Determining where the muS matrix is in the tu matrix
    int startRow = max_node+length;  //The index of the row is after max_node and length
    int startCol = max_node+length+length; //Places mu matrix to the right by the size of max_node and two lengths
    int i=0; //to iterate through the matrix rows

    //Add voltages to muS, iterating through the voltage vector
    for (double v : voltages) {
        Tu[startRow+i][startCol] = v;
        i++;
    }
}

void addN(vector<vector<double>>& Tu, int max_node, int length) {

    //Reading the text file and storing values
    string stream; //String to hold the data in the text file
    ifstream MyReadFile("netlist.txt"); //reading the text file

    vector<double> resistances; //vector to store values in the string

    //Reading the file line by line
    while (getline(MyReadFile, stream)) {
        istringstream iss(stream);
        string component;
        int node1, node2;
        double value;

        // Extract resistances from line
        if (iss >>component >>node1 >>node2 >>value) {
            //If the component starts with 'R', then save the value
            if (component[0] == 'R') {
                resistances.push_back((value*-1.0));
            }
            //If not, place a '0' as a placeholder
            else {
                resistances.push_back(0);
            }
        }
    }

    //Close the file
    MyReadFile.close();

    //Determining where the muS matrix is in the tu matrix
    int startRow = max_node+length;  //The index of the row is after max_node and length
    int startCol = max_node+length; //Places N matrix to the right by the size of max_node and length
    int i=0; //to iterate through the matrix rows

    //Add voltages to muS, iterating through the voltage vector
    for (double v : resistances) {
        Tu[startRow+i][startCol+i] = v;
        i++;
    }
}

void print_matrix (vector<vector<double>> &Tu, int row, int col) {
    // prints matrix given max row and col values  
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j ++) {
            cout << Tu[i][j] << " "; 
        }
        cout << endl;
    }  
    cout << endl; 
}

void write_file(vector<double> &s, int row) {
    // writes output.txt file given the solution vector and row count 
    fstream out_file;
    string file_name = "output.txt"; 

    out_file.open(file_name, ios::out);
    if (out_file.is_open()) {
        for (int i = 0; i < row; i++) {
            out_file << s[i] << " "; 
        }
    }
    else {
        cout << "Error opening output file." << endl; 
    }

}

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


    // random 8x9 matrix for testing the row reduction algorithm 
    /*Tu = {
        {2, 3, -1, 4, 5, 0, -2, 1, 7},
        {1, -2, 3, -4, 5, -6, 7, -8, 9},
        {0, 5, -2, 3, 1, -4, 6, 2, -3},
        {4, -1, 2, -5, 3, 6, -7, 8, 0},
        {3, 2, -4, 1, -6, 5, -7, 9, -8},
        {-2, 4, -1, 3, -5, 2, 6, -8, 7},
        {5, -3, 2, -1, 4, -7, 8, -6, 9},
        {1, 6, -5, 3, -2, 7, -4, 0, 8}
    };*/

    addA_AT_Matrix(Tu, max_node, length); 
    addM_IMatrix(Tu, max_node, length); 
    addMu(Tu, max_node, length); 
    addN(Tu, max_node, length); 
    

    // output rows 
    print_matrix(Tu, row, col);  

    //compute reduced echelon form of the matrix
    ref_matrix(Tu, max_node, length, row, col); 
    print_matrix(Tu, row, col); 

    // check if the matrix has a unique solution 
    if (!valid_matrix(Tu, row, col)) {
        cout << "The netlist provided describes an invalid circuit configuration" << endl; 
        return 0;
    }

    // compute the reduced row echelon form of the matrix by back substitution of the row echelon matrix 
    back_sub(Tu, row, col); 

    // output rows 
    print_matrix(Tu, row, col); 

    vector<double> solution_vector(row); 

    for (int i = 0; i < row; i ++) {
        solution_vector[i] = Tu[i][col-1]; 
        cout << solution_vector[i] << " ";
    }
    cout << endl;
    write_file(solution_vector, row); 
    return 1;
}

int main() {
    read_file();
}