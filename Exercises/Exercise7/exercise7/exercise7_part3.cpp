/**
 * A simple C++ program to demonstrate race conditions.
 * 
 * Copyright (C) 2020 liererkt@miamiOH.edu
 * 
 */

#include <iostream>
#include <thread>
#include <vector>
#include <string>

// Prototype for thread method defined further below.
void threadMain(int &x);

/** The main method.
 *
 *  The main method just creates a fixed number of threads and prints
 *  a result to demonstrate a race condition.
 */
int main() {
    std::vector<int> nums(10);
    // The list of threads.
    std::vector<std::thread> threads;
    for (size_t i = 0; (i < nums.size()); i++) {
        threads.push_back(std::thread(threadMain, std::ref(nums[i])));
    }
    // Wait for the threads to finish
    for (auto& t : threads) {
        t.join();
    }
    // Print the value of x (to observe race condition)
    for (size_t i = 0; (i < nums.size()); i++) {
        std::cout << "x = " << nums[i] << std::endl;
    }
    return 0;
}


//---------------------------------------------------------------
//  DO   NOT   MODIFY   CODE   BELOW    THIS   LINE
//---------------------------------------------------------------

/** Thread method, that is invoked from multiple threads.
 *
 * This method just increments the parameter 50,000 times.
 *
 * \param [out] x The parameter to be incremented by this method.
 */
void threadMain(int& x) {
    for (int i = 0; (i < 50'000); i++) {
        x++;   // increment x
    }
}
