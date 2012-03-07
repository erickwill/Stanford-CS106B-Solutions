#include "mazerun.h"
#include "maze.h"
#include "genlib.h"
#include "graphics.h"
#include "extgraph.h"
#include "grid.h"
#include <iostream>
#include "random.h"

const int TOTAL_ROWS = 17;
const int TOTAL_COLUMNS = 30;

void InitMaze(Maze &maze);
pointT GetNeighborCell(pointT &pt);

void MazeRun() {
    Randomize();
    InitGraphics();
    Maze maze(TOTAL_ROWS, TOTAL_COLUMNS, true);
    maze.draw();
    InitMaze(maze);    
}

void InitMaze(Maze &maze) {
    cout << endl << "Building Maze..." << endl;
    pointT pt;
    pt.row = RandomInteger(0, TOTAL_ROWS-1);
    pt.col = RandomInteger(0, TOTAL_COLUMNS-1);

    Grid<bool> excluded(TOTAL_ROWS, TOTAL_COLUMNS);
    excluded(pt.row, pt.col) = false; // included
    int includedCount = 0;

    while(includedCount < (TOTAL_ROWS * TOTAL_COLUMNS)-1) {
        pointT neighbor = GetNeighborCell(pt);
        if (excluded(neighbor.row, neighbor.col)) {
            excluded(neighbor.row, neighbor.col) = false;
            maze.setWall(pt, neighbor, false);
            includedCount++;
        }
        pt = neighbor;
        UpdateDisplay();
    }
    cout << "Maze Complete." << endl << endl;
}

pointT GetNeighborCell(pointT &pt) {
    pointT neighbor = pt;
    int rand = RandomInteger(1,4);
    switch(rand) {
        case 1: 
            neighbor.col++;
            break;
        case 2: 
            neighbor.col--;
            break;
        case 3: 
            neighbor.row--;
            break;
        default: 
            neighbor.row++;
    }
    if (neighbor.row < 0 || neighbor.col < 0 || 
        neighbor.row > TOTAL_ROWS-1 || neighbor.col > TOTAL_COLUMNS-1)
        GetNeighborCell(pt);
    else
        return neighbor;
}