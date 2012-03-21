#include "genlib.h"
#include <iostream>
#include "vector.h"
#include "set.h"
#include "simpio.h"

bool Solvable(int start, Vector<int> & squares);
bool RecSolve(int start, Vector<int> &squares, Set<int> &visitedSquares, bool &solved);

int main () {
    Vector<int> squares;
    int start;

    while(true) {
        cout << "Enter integers for puzzle (enter 0 when done): ";
        int num = GetInteger();
        squares.add(num);
        if (num == 0) break;
    }
    while(true) {
        cout << "Enter starting index: ";
        start = GetInteger();
        if (start < squares.size()) break;
    }
    
    if (Solvable(start, squares))
        cout << "True! The puzzle is solvable." << endl;
    else
        cout << "False! The puzzle cannot be solved." << endl;

    return 0;
}

bool Solvable(int start, Vector<int> & squares) {
    Set<int> visitedSquares;
    bool solved = false;
    return RecSolve(start, squares, visitedSquares, solved);
}

bool RecSolve(int start, Vector<int> &squares, Set<int> &visitedSquares, bool &solved) {
    if (visitedSquares.contains(start))
        return solved;
    if (start == squares.size()-1)
        solved = true;

    visitedSquares.add(start);

    if (start + squares[start] < squares.size())
        RecSolve(start + squares[start], squares, visitedSquares, solved);
    if (start - squares[start] >= 0)
        RecSolve(start - squares[start], squares, visitedSquares, solved);

    return solved;
}