#include <iostream>
#include <fstream> // fstream documentation | https://cplusplus.com/reference/fstream/
#include <string>  // string documentation  | https://cplusplus.com/reference/string/
#include <vector>  // vector documentation  | https://cplusplus.com/reference/vector/
#include <cstdlib> // cstdlib documentation | https://cplusplus.com/reference/cstdlib/


using namespace std;

int get_sord (string line, char code) {
    // given a line and a code return the desired float number
    // code 's': source 
    // code 'd': destination

    fstream netlist;
    // size_t is the find method's return data type 
    size_t start = 0, end; 
    string sub; 

    netlist.open("netlist.txt", ios::in); 

    if (code == 's') {
        
        // first space is between component type and source node so skip 
        end = line.find(' ', start);     
        // iterate once through start and end values so that they are between the source node 
        start = end + 1;
        end = line.find(' ', start);
        sub = line.substr(start, end);   
        
    }
    else if (code == 'd') {
        end = line.find(' ', start); 

        // increment twice to get start and end between the desintation node
        for (int i = 0; i < 2; i++) {
            start = end + 1; 
            end = line.find(' ', start); 
        }
        sub = line.substr(start, end); 
    }
    else {
        cout << "Invalid code for finding source or destination node" << endl; 
    }
    netlist.close();
    return stoi(sub);
}

char get_type (string line) {
    // return 'V' if voltage source and 'R' if resistor

    // the type will always be in the first position of the line, so just return that char
    return line[0]; 
}

float get_value(string line) {
    //returns the value of the circuit element as a float

    fstream netlist;
    size_t start = 0, end; 
    string sub; 

    netlist.open("netlist.txt", ios::in); 
    end = line.find(' ', start); 

    // increment three times to get start and end between the element's value 
    for (int i = 0; i < 3; i++) {
        start = end + 1; 
        end = line.find(' ', start); 
    }
    sub = line.substr(start, end); 
    
    netlist.close();
    
    // use cstdlib to convert from c-string to float 
    return atof(sub.c_str());
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
}

int main() {
    read_file();
}