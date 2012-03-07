#include "mazerun.h"
#include "maze.h"
#include "genlib.h"
#include "graphics.h"
#include "extgraph.h"
#include "grid.h"
#include <iostream>
#include "random.h"
#include "queue.h"
#include "stack.h"
#include "simpio.h"
#include "vector.h"

const int TOTAL_ROWS = 17;
const int TOTAL_COLUMNS = 30;

void InitMaze(Maze &maze);
pointT GetNeighborCell(pointT &pt, Maze &maze);
void SolveMaze(Maze &maze);
void GetMorePaths(Maze &maze);
bool ptAllowed(Vector<pointT> &usedPts, pointT &neighbor);
void DrawPath(Stack<pointT> &path, Maze &maze);

Stack<pointT> path;    
Vector<pointT> usedPts;
Queue<Stack<pointT> > pathCollection;        


void MazeRun() {
    Randomize();
    InitGraphics();
    Maze maze(TOTAL_ROWS, TOTAL_COLUMNS, true);
    maze.draw();
    InitMaze(maze);
    cout << "Press Enter to solve maze: ";
    GetLine();
    cout << endl;
    SolveMaze(maze);
    path.clear();
    pathCollection.clear();
    usedPts.clear();
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
        pointT neighbor = GetNeighborCell(pt, maze);
        if (excluded(neighbor.row, neighbor.col)) {
            excluded(neighbor.row, neighbor.col) = false;
            maze.setWall(pt, neighbor, false);
            includedCount++;
        }
        pt = neighbor;
        UpdateDisplay();
    }
}

pointT GetNeighborCell(pointT &pt, Maze &maze) {
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
    if (!maze.pointInBounds(neighbor))
        GetNeighborCell(pt, maze);
    else
        return neighbor;
}

void SolveMaze(Maze &maze) {
    pointT startPt, finishPt;
    startPt.row = 0;
    startPt.col = 0;
    finishPt.row = TOTAL_ROWS - 1;
    finishPt.col = TOTAL_COLUMNS - 1;

    path.push(startPt);
    usedPts.add(startPt);
    pathCollection.enqueue(path);

    while(!pathCollection.isEmpty()) {
        path = pathCollection.dequeue();
        pointT pt = path.peek();
        if (finishPt.row == pt.row && finishPt.col == pt.col) {
            DrawPath(path, maze);
            return;
        } else {
            GetMorePaths(maze);
        }
    }
}

void GetMorePaths(Maze &maze) {
    pointT pt = path.peek();
    for (int i = 0; i < 4; i++) {
        pointT neighbor = pt;
        switch(i) {
            case 0: 
                neighbor.col++;
                break;
            case 1: 
                neighbor.col--;
                break;
            case 2: 
                neighbor.row--;
                break;
            default: 
                neighbor.row++;
        }
        if (ptAllowed(usedPts, neighbor) && 
            maze.pointInBounds(neighbor) && !maze.isWall(pt, neighbor)) {
            Stack<pointT> nextPath = path;
            nextPath.push(neighbor);
            pathCollection.enqueue(nextPath);
            usedPts.add(neighbor);
        }
    }
}

bool ptAllowed(Vector<pointT> &usedPts, pointT &neighbor) {
    for (int i = 0; i < usedPts.size(); i++) {
        if (usedPts[i].row == neighbor.row && usedPts[i].col == neighbor.col)
            return false;
    }
    return true;
}

void DrawPath(Stack<pointT> &path, Maze &maze) {
    Vector<pointT> finalPath;
    while(!path.isEmpty()) {
        finalPath.add(path.pop());
    }
    for (int i = finalPath.size()-1; i >= 0 ; i--) {
        maze.drawMark(finalPath[i], "blue");
        UpdateDisplay();
    }
}
