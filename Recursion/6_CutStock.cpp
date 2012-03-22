#include <iostream>
#include "genlib.h"
#include "vector.h"
#include "simpio.h"

int CutStock(Vector<int> & requests, int stockLength);
void RecCut(int index, Vector<int> &requests, int &pipeCount,
           int stockLength, int &pipeRemainder, Vector<int> &totalPipes);
void PermuteVector(Vector<int> prefix, Vector<int> rest, 
                   int stockLength, Vector<int> &totalPipes);
void PrintVector(Vector<int> v);
int MinVal(Vector<int> &v);

int main () {
    cout << "Enter the length of a stock pipe: ";
    int stockLength = GetInteger();

    Vector<int> requests;
    while(true) {
        cout << "Enter requested pipe lengths (enter 0 when finished): ";
        int length = GetInteger();
        if (length == 0) break;
        requests.add(length);
    }

    int minPipes = CutStock(requests, stockLength);
    cout << "The minimum amount of pipes required = " << minPipes << endl;
    return 0;
}

int CutStock(Vector<int> & requests, int stockLength) {
    Vector<int> empty;
    Vector<int> totalPipes;
    PermuteVector(empty, requests, stockLength, totalPipes);
    return MinVal(totalPipes);
}

/* Creates all permutations of a vector and passes them to the function RecCut */ 
void PermuteVector(Vector<int> prefix, Vector<int> rest, 
                   int stockLength, Vector<int> &totalPipes) {
    if (rest.isEmpty()) {
        int index = 0;
        int pipeRemainder = 0;
        int pipeCount = 0;
        // get minimum pipes required for this vector
        RecCut(index, prefix, pipeCount, 
            stockLength, pipeRemainder, totalPipes);
    }
    else {
        for (int i = 0; i < rest.size(); i++) {
            prefix.add(rest[i]);
            rest.removeAt(i);
            PermuteVector(prefix, rest, stockLength, totalPipes);
        }
    }
}

/* 
* Finds the minimum amount of Pipes used to fill the values of a vector of integers.
* Minimum values are then stored in another vector.
*/
void RecCut(int index, Vector<int> &requests, int &pipeCount,
           int stockLength, int &pipeRemainder, Vector<int> &totalPipes) {
    
    if (index >= requests.size()) {
         totalPipes.add(pipeCount); // store minimum pipes value
         return;
    }

    if (requests[index] > pipeRemainder) {
        pipeCount++;
        pipeRemainder = stockLength;
        pipeRemainder -= requests[index];
    }
    else
        pipeRemainder -= requests[index];

    RecCut(index+1, requests, pipeCount, stockLength, pipeRemainder, totalPipes);
}

/* Returns the lowest value in a vector of integers */
int MinVal(Vector<int> &v) {
    if (v.isEmpty()) return 0;
    int min = v[0];
    for (int i = 0; i < v.size(); i++) {
        if (v[i] < min)
            min = v[i];
    }
    return min;
}