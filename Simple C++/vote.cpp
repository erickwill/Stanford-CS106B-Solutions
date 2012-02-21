/*
* The user enters the total number of votes, the vote spread,
* and the percentage of error. The program prints the percentage
* of an invalid election based on multiple trial runs.
*/

#include <iostream>
#include "genlib.h"
#include "simpio.h"
#include "random.h"

const int NUM_TRIALS = 500;

double CalculateError(int voters, double spread, double error);

int main() {
    Randomize();

    int voters;
    while(true) {
        cout << "Enter number of voters: ";
        voters = GetInteger();
        if (voters > 0) break;
        cout << "Please enter a positive number." << endl;
    }
    
    double spread;
    while(true) {
        cout << "Enter percentage spread between candidates: ";
        spread = GetReal();
        if (spread >= 0 && spread <= 1) break;
        cout << "Spread must be between 0 and 1.0" << endl;
    }

    double error;
    while(true) {
        cout << "Enter voting error percentage: ";
        error = GetReal();
        if (error >= 0 && error <= 1) break;
        cout << "Error percentage must be between 0 and 1.0 " << endl;
    }

    cout << endl << "Chance of an invalid election result after " 
            << NUM_TRIALS << " trials = " 
            << CalculateError(voters, spread, error) << "%" << endl;
    return 0;
}

double CalculateError(int v, double s, double e) {
    double total = 0; // number of invalid elections
    int spread = v * s;
    for (int j = 0; j < NUM_TRIALS; j++) {
        int countA = 0; // candidate A
        int countB = 0; // candidate B
        for (int i = 0; i < v; i++) {
            bool vote = RandomChance(1.0 - e); // cast vote
            if (i < (v/2) + spread) { // apply votes according to spread
                if (vote) countA++;
                else countB++;
            } else {
                if (vote) countB++;
                else countA++;
            }
        }
        if (countB > countA)
            total++;
    }
    return double(total/NUM_TRIALS * 100);
}