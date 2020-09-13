// Copyright (C) 2019 raodm@miamioh.edu

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

/**
 *  Reminder regarding input file format --
 *
 *  A list of files are supplied, with 1-line per file. Each line
 *  consists of 4 tab-separated columns in the following format:
 *
 *   permissions    userID    groupID   path
 *
 * NOTE: The path is always only 1 word (no spaces).
 */

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

    // We assume 1st argument is input data file. So open it for reading.
    std::ifstream is(argv[1]);
    // We assume the 2nd argument is the user id to be processed.
    const std::string user = argv[2];

    // The 4 strings to read the 4 columns of values.
    std::string permissions, uid, group, path;

    // Process line-by-line and track group with highest count
    while (is >> permissions >> uid >> group >> path) {
        if ((user == uid) && (permissions[0] == 'r')) {
            std::cout << path << std::endl;
        }
    }

    // All done.
    return 0;
}

// End of source code
