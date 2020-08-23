/*
 * A program to print login-IDs of the users belonging to a given set of gids 
 * (group IDs) specified as command-line arguments. The necessary data is read 
 * from 2 given text files.
 * 
 * Copyright (C) 2020 liererkt@miamioh.edu
 * 
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>

using namespace std;
using strMap = unordered_map<string, string>;

const char GROUPS_PATH[] = "groups";
const char PASSWD_PATH[] = "passwd";

/// Builds a map of users to simplify user lookup.
/// \param in The file stream of the users document.
/// \return A map where the key is a user's gid and the value is their loginId.
strMap buildPasswdMap(ifstream& in) {
    // Declare a map to be used later.
    strMap resultMap;
    
    // Declare temp variables to be used in the loop.
    string line, loginId, uid;
    
    // Loops while a line is available to build the map from.
    while (getline(in, line)) {
        // Replaces ':' with ' ' so that the stream extraction operator 
        // can be used.
        replace(begin(line), end(line), ':', ' ');
        
        // Extracts the loginId and gid to insert into the map.
        // Note: The passkey is skipped over.
        istringstream  is(line);
        is >> loginId >> uid >> uid;
        
        // Finally, the loginId is added to the map for lookup.
        resultMap[uid] = loginId;
    }
    return resultMap;
}

/// Builds a map of groups to simplify group lookup.
/// \param in The file stream of the groups document.
/// \param userMap The map of users.
/// \return A map where the key is a group's gid and the value is a string 
///         representation of the users in the group.
strMap buildGroupMap(ifstream& in, strMap userMap) {
    // Declare a map to be used later.
    strMap resultMap;
    
    // Declare temp variables to be used in the loop.
    string line, uid, groupName, dummy;
    
    // Loops while a line is available to build the map from.
    while (getline(in, line)) {
        string gid = "", uids = "";
        
        // Replaces ':' with ' ' so that the stream extraction operator 
        // can be used.
        replace(begin(line), end(line), ':', ' ');
        
        // Extracts the group name, gid, and a comma delimited list of all 
        // uids in the group.
        istringstream  is(line);
        is >> groupName >> dummy >> gid >> uids;
        
        // Starts building the string representation of the users in the 
        // group.
        string result = groupName + ": ";
        
        // Loops through all the uids and creates a string representation of
        // an individual user that will be added to the string 
        // representation of all the users. 
        stringstream ss(uids);
        while (getline(ss, uid, ',')) {
            // Note: The format is 'loginId (uid)' and is space delimited.
            result += userMap[uid] + "(" + uid + ") ";
        }
        
        // Removes the last character since it is delimiter.
        result.pop_back();
        
        // Finally, the string representation is added to the map for lookup.
        resultMap[gid] = result;
    }
    return resultMap;
}

/// The main function that serves as a starting point for the command-line 
/// arguments to be processed.
/// \param argc The number of command-line arguments. 
/// \param argv The actual command-line arguments.
/// \return Whether the function was successfully processed or not.
int main(int argc, char** argv) {
    try {
        // Reads the files in to build the passwdMap and Groups map from.
        ifstream groups(GROUPS_PATH);
        ifstream passwd(PASSWD_PATH);

        // Builds maps to simplify lookup process.
        strMap passwdMap = buildPasswdMap(passwd);
        strMap groupsMap = buildGroupMap(groups, passwdMap);

        // Process all of the arguments.
        for (int i = 1; i < argc; i++) {
            if (groupsMap.find(argv[i]) == groupsMap.end()) {
                cout << argv[i] << " = Group not found." << endl;
            } else {
                cout << argv[i] << " = " << groupsMap[argv[i]] << endl;
            }
        }
    } catch (int e) {
        return e;
    }
    
    return 0;
}

