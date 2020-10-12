/*
 * File:   exercise8_part2.cpp
 * Author: Kyle Lierer
 *
 * Copyright (C) 2020 liererkt@miamiOH.edu
 */

#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <functional>

// Forward declarations to make the compiler happy
int countFactors(const long);

/**
 * 
 * @param numList The list of numbers whose factor counts are to be computed.
 * @param result The list of factor counts corresponding to each value in 
 * numList.
 * @param startIdx The start index in the numList (inclusive).
 * @param endIdx The end index in the numList (exclusive).
 */
void thrMain(const std::vector<long>& numList, std::vector<int>& result, 
             const int startIdx, const int endIdx) {
    for (int i = startIdx; (i < endIdx); i++) {
        result[i] = countFactors(numList[i]);
    }
}

/** Method to count the factors for a given list of values.
 *
 * \param[in] numList The list of numbers whose factor counts are to
 * be computed.
 *
 * \param[in] thrCount The number of threads to be used. The starter
 * code ignores this parameter.
 *
 * \return The list of factor counts corresponding to each value in
 * numList.
 */
std::vector<int> 
getFactorCount(const std::vector<long>& numList, const int thrCount) {
    const int listSize = numList.size();
    const int count = (listSize / thrCount) + 1;
    
    // First allocate the return vector
    std::vector<int> factCounts(numList.size());
    
    // Threads
    std::vector<std::thread> threads;
    
    // Compute the factors for each number
    for (int start = 0, thr = 0; (thr < thrCount); thr++, start += count) {
        int end = std::min(listSize, start + count);
        threads.push_back(std::thread(thrMain, std::cref(numList), 
                std::ref(factCounts), start, end));
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    // Return the result back
    return factCounts;
}

//-------------------------------------------------------------
//  DO  NOT  MODIFY  CODE  BELOW  THIS  LINE
//-------------------------------------------------------------

/** Helper method to count the number of factors for a given number.
    This method is rather simple.  It iterates from 1..num and counts
    number of factors encountered.
    
    \param[in] num The number for which the factors need to be counted.

    \return The numebr of factors (including 1 and num) for the given
    number. For example, if num is 6, this method returns 4 (because
    factors of 6 are 1, 2, 3, 6).
*/
int countFactors(const long num) {
    int factorCount = 0;
    for (int long fact = 1; (fact <= num); fact++) {
        if (num % fact == 0) {
            factorCount++;  // Found a factor
        }
    }
    return factorCount;
}

/**
   Convenience method to load integers from a given file.

   \param[in] filePath The path to the file from where the numbers are
   to be loaded.

   \return The list of numbers loaded from the given data file.
*/
std::vector<long> loadData(const std::string& filePath) {
    // Load numbers from a given text file.
    std::ifstream dataFile(filePath);
    std::istream_iterator<long> readIter(dataFile), eof;
    std::vector<long> numberList(readIter, eof);
    return numberList;
}

/** A simple error message to reduce clutter in the source code. */
const std::string INSUFFICIENT_ARGS =
    "Error: Insufficient Arguments.\nThe program requires the following " \
    "arguments:\n    1. The file with numbers to be processed.\n" \
    "    2. The number of threads to be used for processing.\n" \
    "EXAMPLE: ./exercise7_part4 numbers.txt 3\n";


/**
 * The main method that fires off various threads and waits for them
 * to finish.
 *
 * @param args Optional command-line arguments. The first argument is
 * the number of threads. The second argument is the time delay.
 */
int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << INSUFFICIENT_ARGS;
        return 1;
    }
    // Load the data from the file.
    const std::vector<long> numberList = loadData(argv[1]);
    // Get factor counts for each number.
    const int numThreads = std::stoi(argv[2]);
    const std::vector<int> factCount = getFactorCount(numberList, numThreads);
    
    // Print the results
    for (size_t i = 0; (i < numberList.size()); i++) {
        std::cout << numberList[i] << ": " << factCount[i] << "\n";
    }
    return 0;
}
