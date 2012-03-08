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

Stack<pointT> path;    // possible solution path
Vector<pointT> usedPts; // tested solution points
Queue<Stack<pointT> > pathCollection; // possible solution paths

/*
* Main maze method. Initializes a grid of cells. Uses Aldous-Broder
* to create a perfect maze. Solves the maze using 
* breadth-first search and draws the solution.
*/
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

    // clear containers for next use
    path.clear();
    pathCollection.clear();
    usedPts.clear();
}

/* 
* Initializes grid and creates a perfect maze using 
* Aldous-Broder algorithm.
*/
void InitMaze(Maze &maze) {
    cout << endl << "Building Maze..." << endl;
    pointT pt; // initial random point
    pt.row = RandomInteger(0, TOTAL_ROWS-1);
    pt.col = RandomInteger(0, TOTAL_COLUMNS-1);

    Grid<bool> excluded(TOTAL_ROWS, TOTAL_COLUMNS); // keep track of included cells
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

/* Returns a random neighbor cell. */
pointT GetNeighborCell(pointT &pt, Maze &maze) {
    Vector<pointT> neighbors; // possible neighbor cells

    // check for valid neighbor and add to vector
    for (int i = 0; i < 4; i++) {
        pointT neighborPt = pt;
        switch(i) {
            case 0: 
                neighborPt.col++;
                if (neighborPt.col <= TOTAL_COLUMNS-1)
                neighbors.add(neighborPt);
                break;
            case 1: 
                neighborPt.col--;
                if (neighborPt.col >= 0)
                neighbors.add(neighborPt);
                break;
            case 2: 
                neighborPt.row--;
                if (neighborPt.row >= 0)
                neighbors.add(neighborPt);
                break;
            default: 
                neighborPt.row++;
                if (neighborPt.row <= TOTAL_ROWS-1)
                neighbors.add(neighborPt);
        }
    }
    // select random neighbor
    int size = neighbors.size();
    int rand = RandomInteger(0,size-1);
    return neighbors[rand];
}

/* Solves the maze using breadth-first search. */
void SolveMaze(Maze &maze) {
    pointT startPt, finishPt;
    startPt.row = 0;
    startPt.col = 0;
    finishPt.row = TOTAL_ROWS - 1;
    finishPt.col = TOTAL_COLUMNS - 1;

    path.push(startPt);
    usedPts.add(startPt);
    pathCollection.enqueue(path);

    // BFS
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

/* 
* Adds more paths to queue by adding valid neighbor
* cells to the existing path.
*/
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

/* Checks if point has been tested to avoid circular paths. */
bool ptAllowed(Vector<pointT> &usedPts, pointT &neighbor) {
    for (int i = 0; i < usedPts.size(); i++) {
        if (usedPts[i].row == neighbor.row && usedPts[i].col == neighbor.col)
            return false;
    }
    return true;
}

/* Draws the solution from start to finish. */
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
