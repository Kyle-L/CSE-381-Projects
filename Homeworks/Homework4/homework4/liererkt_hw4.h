/*
 * A simple shell program to execute commands in either a 
 * serialized or parallelized manner.
 * 
 * File:   liererkt_hw4.h
 * Author: Kyle Lierer
 *
 * Copyright (C) 2020 liererkt@miamioh.edu
 * 
 */

#ifndef LIERERKT_HW4_H
#define LIERERKT_HW4_H

#include <iostream>
#include<string>
#include <fstream>
#include <vector>
#include <utility>
#include "ChildProcess.h"

using ChildVec = std::vector<ChildProcess>;
using ChildPair = std::pair<ChildProcess, int>;

/**
 * Reads commands in from an input stream and executes them in a serialized or 
 * parallelized manner.
 * @param is An input stream where commands are being read from.
 * @param os An output stream where the results of the commands are displayed.
 * @param prompt An optional prompt that is displayed to the output stream.
 * @param parallel Whether the commands are executed in a parallelized manner.
 */
void process(std::istream& is = std::cin, std::ostream& os = std::cout, 
        const std::string& prompt = "> ", bool parallel = false);

/**
 * Processes shell commands from a given URL to a text file that contains the 
 * actual commands to be executed
 * @param url The URL to the text file with the commands.
 * @param os An output stream where the results of the commands are displayed.
 * @param parallel Whether the commands are executed in a parallelized manner.
 */
void processFromURL(std::string& url, std::ostream& os, bool parallel);

/**
 * Executes a command and outputs the result.
 * @param command The command as a string.
 * @param os An output stream where the results of the commands are displayed.
 * @return A ChildPair which contains the ChildProcess the command was executed
 *         on as well the PID of the ChildPair.
 */
ChildPair execute(std::string command, std::ostream& os = std::cout);

/**
 * Helper method to break down a URL into hostname, port and path. For
 * example, given the url: "https://localhost:8080/~raodm/one.txt"
 * this method returns <"localhost", "8080", "/~raodm/one.txt">
 * @param url A string containing a valid URL. The port number in URL
 * is always optional.  The default port number is assumed to be 80.
 *
 * @return This method returns a std::tuple with 3 strings. The 3
 * strings are in the order: hostname, port, and path.  Here we use
 * std::tuple because a method can return only 1 value.  The
 * std::tuple is a convenient class to encapsulate multiple return
 * values into a single return value.
 */
std::tuple<std::string, std::string, std::string> 
breakDownURL(const std::string& url);


#endif /* LIERERKT_HW4_H */

