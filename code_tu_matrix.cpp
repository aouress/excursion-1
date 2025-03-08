// contains all function defintions for coding the [T | u] matrix
#include "parse.h" // include parsing library for parsing functions 
#include "code_tu_matrix.h"
#include <iostream>
#include <fstream> // fstream documentation | https://cplusplus.com/reference/fstream/
#include <string>  // string documentation  | https://cplusplus.com/reference/string/
#include <vector>  // vector documentation  | https://cplusplus.com/reference/vector/
#include <cstdlib> // cstdlib documentation | https://cplusplus.com/reference/cstdlib/
#include <sstream> // sstream documentation | https://cplusplus.com/reference/sstream/

using namespace std;

// Function that places the A and -A^T matrices into [T | u] as codes of Kirchhoff's Current and Voltage Laws 
void add_KCL_KVL(vector<vector<double>> &Tu, int max_node, int length) {
    fstream netlist;

    // open the netlist file and configure it to read
    netlist.open("netlist.txt", ios::in);

    if (netlist.is_open()) {
        //declare a string to hold each line 
        string line; 

        // over the the length parameter get a line from the netlist file (length == number of lines)        
        for(int i = 0; i < length; i++) {
            getline(netlist, line);

            // use get_sord to get the source and destination nodes 
            int source_node = get_sord(line, 's');
            int dest_node = get_sord(line, 'd');

            // assign source node values in A to be 1 and source node values in -A^T to -1
            Tu[source_node - 1][max_node + length + i] = 1;

            // The transpose requires an index transformation such that the row indices of A flip with the column indices of -A^T
            // The row indices of -A^T start as the max_node parameter
            Tu[max_node + i][source_node - 1] = -1; 
            
            // as long as the destination node is nonzero, set values in A to be -1, and values in -A^T to be 1
            if (dest_node != 0){
                    Tu[dest_node - 1][max_node + length + i] = -1;

                    // Same transpose transformation as described in the source_node case 
                    Tu[max_node + i][dest_node - 1] = 1;
            }
            
        }
    }
    //close netlist file 
    netlist.close(); 
}

// adds identity matrix and M matrix to the [T | u] matrix 
// for resistive circuits with only voltage sources and resistors, the M matrix will always be an identity matrix 
// the M matrix is 1/3 parts for coding Ohm's law
void addIs(vector<vector<double>>& Tu, int max_node, int length) {
    int startRow = max_node; // The index of the row is after max_node and length
    int startCol = max_node; // Places identity matrix to the right by the size of max_node

    //Adds an identity and M matrices to Tu matrix
    for (int i = 0; i < length; i++) {
        Tu[startRow + i][startCol + i] = 1; //Diagonally adds 1s
        Tu[length + startRow + i][startCol + i] = 1;
    }
}

// adds mu_s column vector to the [ T | u ] matrix, this function is 1/3 parts for coding Ohm's law
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

// adds the N matrix to the [T | u] matrix, this function is 1/3 parts for coding Ohm's law 
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
    int i = 0; //to iterate through the matrix rows

    //Add voltages to muS, iterating through the voltage vector
    for (double v : resistances) {
        Tu[startRow+i][startCol+i] = v;
        i++;
    }
}