#include "genlib.h"
#include <iostream>
#include "simpio.h"

const int LARGE_STEP = 2;
const int SMALL_STEP = 1;

int CountWays(int numStairs);

int main () {
    int stairs;
    while(true) {
        cout << "Enter 0 to exit." << endl << "Enter number of stairs: ";
        stairs = GetInteger();
        if (stairs == 0) exit(0);
        int numWays = CountWays(stairs);
        cout << "The number of different ways to climb is " 
            << numWays << endl << endl;
    }
    return 0;
}

int CountWays(int numStairs) {
    if (numStairs < 4) return numStairs;
    return CountWays(numStairs-SMALL_STEP) + CountWays(numStairs-LARGE_STEP);
}