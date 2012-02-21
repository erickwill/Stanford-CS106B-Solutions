/*
* Finds all of the perfect numbers from 1 - 10000.
*/

#include <iostream>
#include "genlib.h"
#include "simpio.h"

const int MAX_NUM = 10000; // maximum number to test

bool IsPerfect(int num);

int main() {
    for (int i = 0; i < MAX_NUM; i++) {
        if (IsPerfect(i)) {
            cout << i << endl;
        }
    }
    return 0;
}

/*
* Returns true if the sum of all of the numbers that divide 
* into it equally (excluding itself) add up to the number
* itself.
*/
bool IsPerfect(int num) {
    int sum = 0;
    for (int i = 0; i < num; i++) {
        if (i > 0 && num % i == 0) {
            sum += i;
        }
    }
    return (num > 0 && sum == num);
}