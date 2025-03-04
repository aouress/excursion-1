#include <iostream>
#include <fstream>
#include <string> 
#include <vector>
// fstream documentation | https://cplusplus.com/reference/fstream/

using namespace std;

//finds the source node and compares to previous max 
int find_max_source_node(string line, int max_node) {
    
    // size_t is the find method's return data type 
    //http://cplusplus.com/reference/string/string/find/
    size_t start = 0, end;

    // first space is between component type and source node so skip 
    end = line.find(' ', start); 
    //second space is between source node and destination node, giving length of source node for indexing
    start = end + 1;
    end = line.find(' ', start);
    string sub = line.substr(start, end);

    //new max node found if equality holds 
    if (stoi(sub) > max_node) {
        max_node = stoi(sub); 
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
    vector<vector<int>> Tu(row, vector<int>(col, 0)); 

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