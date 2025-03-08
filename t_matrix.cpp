// include header file with the parsing function declarations
#include "parse.h"
#include <iostream>
#include <fstream> // fstream documentation | https://cplusplus.com/reference/fstream/
#include <string>  // string documentation  | https://cplusplus.com/reference/string/
#include <vector>  // vector documentation  | https://cplusplus.com/reference/vector/
#include <cstdlib> // cstdlib documentation | https://cplusplus.com/reference/cstdlib/
#include <sstream> // sstream documentation | https://cplusplus.com/reference/sstream/stringstream/

using namespace std;

void insertMMatrix(vector<vector<float>>& Tu, int max_node, int length) {
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

void addMu(vector<vector<float>>& Tu, int max_node, int length) {
    
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
    int startCol = max_node+length+length; //Places identity matrix to the right by the size of max_node and two lengths
    int i=0; //to iterate through the matrix rows

    //Add voltages to muS, iterating through the voltage vector
    for (double v : voltages) {
        Tu[startRow+i][startCol] = v;
        i++;
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

    //make zeros matrix 
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            Tu[i][j] = 0; 
        }
    }

    // output rows 
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j ++) {
            cout << Tu[i][j] << " "; 
        }
        cout << endl;
    }

    //Inserting the m-matrix
    insertMMatrix(Tu, max_node, length);
    addMu(Tu, max_node, length);

    //Showing the matrix after inserting m-matrix
    cout << "\nT matrix with M, N and mu-matrix,:" << endl;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            cout << Tu[i][j] << " "; 
        }
        cout << endl;
    }

}

int main() {
    read_file();
}


