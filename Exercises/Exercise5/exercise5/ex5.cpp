/**
 *  Reminder regarding input file format --
 *
 *  A list of files are supplied, with 1-line per file. Each line
 *  consists of 4 tab-separated columns in the following format:
 *
 *   permissions    userID    groupID   path
 *
 * Copyright (C) 2020 liererkt@miamiOH.edu
 * 
 * NOTE: The path is always only 1 word (no spaces).
 * 
 */

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <cstdlib>

using StrVec = std::vector<std::string>;
using Map = std::unordered_map<std::string, StrVec>;

/**
 * Implement this program to print path of files that are readable by a
 * given user.
 *
 * \param[in] argc The number of command-line arguments. For this program
 * you may assume you will always have 2 command-line arguments.
 *
 * \param[in] argv The actual command-line arguments supplied to this
 * program.
 */
int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "Incorrect number of command-line arguments specified.\n";
        return 1;  // Tell OS this was an unsuccessful run of program.
    }
    
    // Opens the data file to search from.
    std::ifstream in(argv[1]);
    
    // Iterates through all lines in the data file looking for readable paths
    // of the user.
    std::string line, permissions, user, command;
    while (std::getline(in, line)) {
        // Extract the permissions, user, and command for a line.
        std::stringstream ss(line);
        ss >> permissions >> user >> command >> command;
        
        // If the user is the same as the 2nd argument and the executable is
        // readable, print the command.
        if (user == argv[2] && permissions[0] == 'r') {
            std::cout << command << std::endl;
        }
    }
    
    // All done.
    return 0;
}

// End of source code
