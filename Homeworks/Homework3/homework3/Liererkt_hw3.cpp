/*
 * A program to print the full hierarchy of processes for a given PID 
 * (process ID, specified as the 2nd command-line argument). The data about 
 * processes is read from a given text file (as 1st command-line argument).
 * 
 * File:   liererkt_hw3.h
 * Author: Kyle Lierer
 *
 * Copyright (C) 2020 liererkt@miamioh.edu
 * 
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Liererkt_hw3.h"
using namespace std;

// Constructor
Solution::Solution() {}

// Destructor
Solution::~Solution() {}

// Loads a PID tree into memory.
void Solution::LoadTree(std::ifstream& in) {
    std::string line, dummy, cmd;
    int pid, ppid;
    
    // Empties the old process trees out of memory.
    Solution::parentMap.empty();
    Solution::commandMap.empty();
    
    // Ignores the header line.
    std::getline(in, line);
    
    // Iterates through all lines to parse the process tree from.
    while (std::getline(in, line)) {
        // Turns the line into a stringstream for easier parsing.
        std::istringstream  is(line);
        
        // Skips the UID and extracts the PID and PPID to build the maps from.
        is >> dummy >> pid >> ppid;
        
        // Skips C, STIME, TTY, and TIME as they are not needed for mapping.
        is >> dummy >> dummy >> dummy >> dummy;
        
        // Removes the whitespace till the start of CMD.
        std::ws(is);
        
        // Gets the rest of the line which is just the CMD needed for mapping.
        std::getline(is, cmd);
        
        // Maps the PPID and CMD to the PID so that the tree can easily
        // be printed later.
        Solution::parentMap[pid] = ppid;
        Solution::commandMap[pid] = cmd;
    }
}

// Recursively prints the full hierarchy of processes (in top-down order) 
// for a given PID of the in memory process tree to an ostream.
void Solution::PrintTree(std::ostream& out, const int& pid) {
    // Gets the PPID to recursively go to the root of the tree.
    int ppid = Solution::parentMap[pid];
    
    // If the PPID is 0, stop recursively going up the tree; otherwise, 
    // keep recursively going up the tree.
    if (ppid != 0) {
        PrintTree(out, Solution::parentMap[pid]);
    }
    
    // Output the PID, PPID, and CMD.
    out << pid << "\t" << ppid;
    out << "\t" << Solution::commandMap[pid] << std::endl;  
}

int main(int argc, char** argv) {
    // Creates an instance of Solution.
    Solution sol;
    
    // Loads the argument file to be supplied to the solution.
    ifstream file(argv[1]);
    
    // Loads the tree so that it can be printed later.
    sol.LoadTree(file);
    
    // Prints a header text so that user knows how to interpret the output.
    std::cout << "Process tree for PID: " << argv[2] << std::endl;
    std::cout << "PID\tPPID\tCMD" << std::endl;
    
    // Prints the process hierarchy top-down.
    sol.PrintTree(std::cout, std::stoi(argv[2]));
    
    return 0;
}

