/*
 * Project: ADT
 */

#include "genlib.h"
#include "simpio.h"
#include "randomwriter.h"
#include "mazerun.h"
#include <iostream>

int main ()
{
    while(true) {
        cout << "0. Exit" << endl;
        cout << "1. Random Writer" << endl;
        cout << "2. Maze" << endl;
        cout << endl << "Please choose a program: ";
        int choice = GetInteger();
        if (choice == 0) exit(0);
        else if (choice == 1)
            RandWriter();
        else if (choice == 2)
            MazeRun();
    }
    return 0;
}