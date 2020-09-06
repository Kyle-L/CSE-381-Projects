/*
 * A simple shell program to execute commands in either a 
 * serialized or parallelized manner.
 * 
 * File:   liererkt_hw4.cpp
 * Author: Kyle Lierer
 *
 * Copyright (C) 2020 liererkt@miamioh.edu
 * 
 */

#include "liererkt_hw4.h"

#include <boost/asio.hpp>
#include <boost/format.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

/**
 * Starts the shell.
 */
int main() {    
    process(std::cin, std::cout, "> ", false);
    return 0;
}

void process(std::istream& is, std::ostream& os, 
        const std::string& prompt, bool parallel) {
    // Temporary variables used the store the entire line, command, 
    // and url if one is present.
    std::string line, cmd, url;
    
    // A vector to keep track of child processes if run in parallel.
    ChildVec vec;
    
    // Outputs a prompt (if one is present) and retrieves the supplied line.
    while (os << prompt, std::getline(is, line)) {
        // Gets the command and url (if one is present).
        std::istringstream ss(line);
        ss >> cmd >> url;
        
        // If the command is exit, stop processing.
        if (cmd == "exit") {
            return;
            
        // If the command is SERIAL, process all commands at the text file 
        // from the URL in a serialized manner.
        } else if (cmd == "SERIAL") {
            processFromURL(url, os, false);
            
        // If the command is PARALLEL, process all commands at the text file 
        // from the URL in a parallelized manner.
        } else if (cmd == "PARALLEL") {
            processFromURL(url, os, true);
            
        // Otherwise, execute the line as a command in either a parallelized or 
        // serialized manner depending on the parallel parameter.
        } else {
            ChildPair pair = execute(line);
            // If it is not running in a parallelized manner, execute the
            // command and then wait for a exit code.
            // NOTE: If the line is either a comment or empty, the child will  
            //       not execute and thus will not have a valid PID (PID < 0).
            //       So, an exit code will not be displayed since it didn't 
            //       execute.
            if (!parallel && pair.second >= 0)
                std::cout << "Exit code: " << pair.first.wait() << std::endl;
            
            // If it is not running in a parallelized manner, execute the
            // command and add it to a vector to wait for exit codes later.
            else if (pair.second >= 0)
                vec.push_back(pair.first);
        }
    }
    // Wait for the exit code of all parallelized commands.
    while (!vec.empty()) {
       std::cout << "Exit code: " << vec.front().wait() << std::endl; 
       vec.erase(vec.begin());
    }
}

ChildPair execute(std::string command, std::ostream& os) { 
    std::istringstream ss(command);

    // Format the command to be executed on the child process by formatting it
    // as a vector of strings.
    StrVec vec;
    for (std::string temp; ss >> std::quoted(temp);) {
        vec.push_back(temp);
    }
    
    // Creates a child process where the command where will be executed on.
    ChildProcess child;
    ChildPair pair;
    pair.first = child;
    
    
    // Exit if there is no command or it is a comment.
    if (vec.empty() || vec.at(0) == "#") {
        pair.second = -1;
        return pair;
    }
    
    // Print the current command being executed.
    os << "Running:";
    for (auto x : vec) {
        os << ' ' << x;
    }
    os << std::endl;
    
    // Execute the command on a forked child process.
    pair.second = child.forkNexec(vec);
    return pair;
}

std::tuple<std::string, std::string, std::string>
breakDownURL(const std::string& url) {
    // The values to be returned.
    std::string hostName, port = "80", path = "/";
    
    // Extract the substrings from the given url into the above
    // variables.

    // First find index positions of sentinel substrings to ease
    // substring operations.
    const size_t hostStart = url.find("//") + 2;
    const size_t pathStart = url.find('/', hostStart);
    const size_t portPos   = url.find(':', hostStart);
    
    // Compute were the hostname ends. If we have a colon, it ends
    // with a ":". Otherwise the hostname ends with the last
    const size_t hostEnd   = (portPos == std::string::npos ? pathStart :
                              portPos);
    
    // Now extract the hostName and path and port substrings
    hostName = url.substr(hostStart, hostEnd - hostStart);
    path     = url.substr(pathStart);
    if (portPos != std::string::npos) {
        port = url.substr(portPos + 1, pathStart - portPos - 1);
    }
    
    // Return 3-values encapsulated into 1-tuple.
    return {hostName, port, path};
}

void processFromURL(std::string& url, std::ostream& os, bool parallel) {
    // Download URL components. That is, given a URL 
    std::string hostname, port, path;
    std::tie(hostname, port, path) = breakDownURL(url);

    // Start the download of the file (that the user wants to be
    // processed) at the specified URL.  We use a BOOST tcp::iostream.
    boost::asio::ip::tcp::iostream data(hostname, port);
    data << "GET "   << path     << " HTTP/1.1\r\n"
         << "Host: " << hostname << "\r\n"
         << "Connection: Close\r\n\r\n";

    // Skip the response header.
    for (std::string hdr; std::getline(data, hdr) && 
        !hdr.empty() && hdr != "\r";) {}
    
    process(data, os, "", parallel);
}

