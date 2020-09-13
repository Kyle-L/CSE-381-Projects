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


    // All done.
    return 0;
}

// End of source code
