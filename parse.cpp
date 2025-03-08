// contains all function definitions for parsing the netlist given a single line as an input
#include "parse.h"
#include <iostream>
#include <fstream> // fstream documentation | https://cplusplus.com/reference/fstream/
#include <string>  // string documentation  | https://cplusplus.com/reference/string/
#include <vector>  // vector documentation  | https://cplusplus.com/reference/vector/
#include <cstdlib> // cstdlib documentation | https://cplusplus.com/reference/cstdlib/

using namespace std;

int get_sord (string line, char code) {
    // given a line and a code return the desired integer source or destination node
    // code 's': source 
    // code 'd': destination

    // size_t is the find method's return data type 
    size_t start = 0, end; 
    string sub; 

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

    // return the string as an integer 
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