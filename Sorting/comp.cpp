#include "comp.h"
#include "set.h"
#include <string>

int comp(string a, string b) {
    int aLen = a.length();
    int bLen = b.length();
    if (a.length() < b.length()) 
        return -1;
    else if (a.length() > b.length()) 
        return 1;
    else if (a > b)
        return 1;
    else if (a < b)
        return -1;
    else
        return 0;
}

int comp(Set<int> a, Set<int> b) {
    Set<int>::Iterator itrA = a.iterator();
    Set<int>::Iterator itrB = b.iterator();
    int sumA = 0;
    int sumB = 0;

    while(itrA.hasNext() || itrB.hasNext()) {
        if (itrA.hasNext())
            sumA += itrA.next();
        if (itrB.hasNext())
            sumB += itrB.next();
    }

    if (sumA > sumB) 
        return 1;
    else if (sumA < sumB)
        return -1;
    else
        return 0;
}