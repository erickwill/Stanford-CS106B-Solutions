/* 
 * File: maze.cpp
 * --------------
 * A maze is mostly a grid of walls. plus a little drawing code.
 */

#include "maze.h"
#include "extgraph.h"

Maze::Maze(int numRows, int numCols, bool hasWalls) : cells(numRows, numCols) 
{
    for (int r = 0; r < numRows; r++)
        for (int c = 0; c < numCols; c++)
            for (int d = 0; d < NumDirs; d++)
                cells(r, c).walls[d] = hasWalls;
    configured = false;
}

int Maze::numRows() 
{ 
    return cells.numRows(); 
}

int Maze::numCols() 
{ 
    return cells.numCols(); 
}

bool Maze::pointInBounds(pointT p)
{
    return (p.row >= 0 && p.row < numRows() && p.col >=0 && p.col < numCols());
}


void Maze::draw()
{
    InitGraphics();    // this erases entire graphics window
    if (!configured) configureGraphics();
    for (int r = 0; r < cells.numRows(); r++) {
        for (int c = 0; c < cells.numCols(); c++) {
            pointT p = {r, c};
            drawWallsForCell(p);
        }
    }
    UpdateDisplay();
}

bool Maze::isWall(pointT p1, pointT p2)
{
    if (!pointInBounds(p1) || !pointInBounds(p2))
        Error("Point is not in bounds for maze");
    return cells(p1.row, p1.col).walls[neighborDir(p1, p2)];
}

void Maze::setWall(pointT p1, pointT p2, bool state)
{
    if (!pointInBounds(p1) || !pointInBounds(p2))
        Error("Point is not in bounds for maze");
    cells(p1.row, p1.col).walls[neighborDir(p1, p2)] = state;
    cells(p2.row, p2.col).walls[neighborDir(p2, p1)] = state;
    if (!configured) configureGraphics();
    drawWallsForCell(p1);
    UpdateDisplay();
}
void Maze::drawMark(pointT p, string color)
{
    if (!pointInBounds(p))
        Error("Point is not in bounds for maze");
    if (!configured) configureGraphics();
    double margin = cellSize*.3;
    double length = cellSize - 2*margin;
    SetPenColor(color);
    MovePen(originX + p.col*cellSize + margin, originY + p.row*cellSize + margin);
    DrawLine(length, length);
    MovePen(originX + p.col*cellSize + margin, originY + p.row*cellSize + cellSize - margin);
    DrawLine(length, -length);
    UpdateDisplay();
}


Maze::dirT Maze::neighborDir(pointT p1, pointT p2)
{
    if ((abs(p1.row-p2.row) + abs(p1.col-p2.col)) != 1)
        Error("Points are not neighbors");
    if (p1.row != p2.row)
        return (p1.row < p2.row ? North : South);
    else
        return (p1.col < p2.col ? East : West);
}

void Maze::drawWallsForCell(pointT p)
{
    MovePen(originX + p.col*cellSize, originY + p.row*cellSize);
    SetPenColor(cells(p.row, p.col).walls[South] ? "Black" : "White");
    DrawLine(cellSize, 0);
    SetPenColor(cells(p.row, p.col).walls[East] ? "Black" : "White");
    DrawLine(0, cellSize);
    SetPenColor(cells(p.row, p.col).walls[North] ? "Black" : "White");
    DrawLine(-cellSize, 0);
    SetPenColor(cells(p.row, p.col).walls[West] ? "Black" : "White");
    DrawLine(0, -cellSize);
}

void Maze::configureGraphics()
{
    cellSize = min(GetWindowWidth()/numCols(), GetWindowHeight()/numRows());
    originX = (GetWindowWidth() - numCols()*cellSize)/2;
    originY = (GetWindowHeight() - numRows()*cellSize)/2;
    configured = true;    
}


