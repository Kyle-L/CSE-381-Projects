/*
 * A basic processing of files/data downloaded from a given web-site, given 
 * a list of 1-or-more files as command-line arguments.
 * 
 * File:   liererkt_hw6.cpp
 * Author: Kyle Lierer
 *
 * Copyright (C) 2020 liererkt@miamiOH.edu
 */

#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <thread>
#include <unordered_map>
#include <future>

// Using namespace to streamline working with Boost socket.
using namespace boost::asio;
using namespace boost::system;

using Dictionary = std::unordered_map<std::string, bool>;

/**
 * Takes a text file of words (each on their own line) and turns it into a 
 * dictionary.
 * @param filePath
 * @return 
 */
Dictionary loadDictionary(const std::string& filePath) { 
    // The dictionary that words from the file will be added.
    Dictionary dictionary;
    
    // Loads the file.
    std::ifstream in(filePath);
    
    // Iterates through each word and adds to the dictionary.
    std::string line;
    while (in >> line) {      
        dictionary[line] = true;
    }
    
    return dictionary;
}

/**
 * Helper method to break down a URL into hostname, port and path. For
 * example, given the url: "https://localhost:8080/~raodm/one.txt"
 * this method returns <"localhost", "8080", "/~raodm/one.txt">
 *
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
breakDownURL(const std::string& url) {
    // The values to be returned.
    std::string hostName, port = "80", path = "/";

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

/**
 * Given an input stream of plain text, counts the number of words and words in
 * a dictionary.
 * @param is An input stream of plain text.
 * @param dict A dictionary of words.
 * @return A tuple that contains the number of words and words in dict.
 */
std::tuple<int, int> 
getWordCounts(std::istream& is, const Dictionary& dict) {
    // The values to be returned.
    int words = 0, dictWords = 0;

    // The following loops skips over the HTTP response header.
    // Note: This makes the assumption that the HTTP response was 200 OK.
    for (std::string hdr; std::getline(is, hdr) && 
        !hdr.empty() && hdr != "\r";) {}
    
    std::string line, word;
    while (is >> line) {
        // Removes punctuation to make matching easier.
        std::replace_if(line.begin(), line.end(), ::ispunct, ' ');
        
        // Turns uppercase to lower case to make matching easier.
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        
        // Iterates through all the words in a line to see if each is in
        // the dictionary.
        std::istringstream ss(line);
        while (ss >> word) {
            words++;
            auto it = dict.find(word);
            if (it != dict.end()) {
                dictWords++;
            }
        }
    }
    return {words, dictWords};
}

/**
 * Takes a remote file of text and a dictionary and analyzes it to determine
 * how many words and words from the dictionary are present in it.
 * @param file The remote file of text.
 * @param dict The dictionary of words.
 * @param os Where the result will be outputted.
 */
std::string analyze(const std::string& file, const std::string& baseUrl, 
        const Dictionary& dict) {
    const std::string url = baseUrl + file;
        
    // Next extract download URL components.
    std::string hostname, port, path;
    std::tie(hostname, port, path) = breakDownURL(url);

    // Start the download of the file (that the user wants to be
    // processed) at the specified URL.
    boost::asio::ip::tcp::iostream data(hostname, port);
    data << "GET "   << path     << " HTTP/1.1\r\n"
         << "Host: " << hostname << "\r\n"
         << "Connection: Close\r\n\r\n";
    
    // Have the helper method process the file's data and print
    int words, dictWords;
    std::tie(words, dictWords) = getWordCounts(data, dict);
    
    std::string result = file + ": "
        + "words=" + std::to_string(words) + ", "
        + "English words=" + std::to_string(dictWords);
    
    return result;
}

int main(int argc, char *argv[]) {
    const std::string baseUrl = "http://os1.csi.miamioh.edu/~raodm/"
                                "cse381/hw4/SlowGet.cgi?file=";
    
    // Loads the dictionary of english words.
    const Dictionary dict = loadDictionary("english.txt");
        
    // Creates a new thread for every argument file,
    std::vector<std::future<std::string>> futList;
    for (int i = 1; i < argc; i++) {
        // Creates the new thread.
        auto fut = std::async(std::launch::async, 
                              analyze, argv[i], baseUrl, dict);
        
        // Moves the future value to a list to get later.
        futList.push_back(std::move(fut));
    }
    
    // Outputs all future values and closes the threads.
    for (auto& fut : futList) {
        std::cout << fut.get() << std::endl;
    }
    
    return 0;
}

