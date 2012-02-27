/*
 * Project: Assign1graphics
 * Created by CS106 C++ Assignment Wizard 0.1
 *
 * Asks the user to click three points on the window.
 * The program then draws a triangle from those points and 
 * starts an animation. The user exits the program from a subsequent
 * mouse click.
 *
 */

#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "random.h"

const double CIRCLE_SIZE = .05; // size of small circle for animation

struct Point {
    double X;
    double Y;
};

Point firstVertex, secondVertex, thirdVertex;

Point RandPoint();
void CreateTriangle();
void SetFirstCircle();
void DrawLoop();

int main ()
{
    InitGraphics(); 
    Randomize();
    printf("Click 3 points to make a triangle.\n");
    CreateTriangle();
    SetFirstCircle();
    DrawLoop();
    return 0;
}

/* Draws a triangle from three mouse clicked points. */
void CreateTriangle() {
    int count = 0;
    while(count < 3) {
        WaitForMouseDown();
        WaitForMouseUp();
        double X = GetMouseX();
        double Y = GetMouseY();
        /* log points */
        if (count == 0) {
            MovePen(X, Y);
            firstVertex.X = X;
            firstVertex.Y = Y;
        } else if (count == 1) {
            secondVertex.X = X;
            secondVertex.Y = Y;
            DrawLine(secondVertex.X - firstVertex.X, 
                secondVertex.Y - firstVertex.Y);
        } else {
            thirdVertex.X = X;
            thirdVertex.Y = Y;
            DrawLine(thirdVertex.X - secondVertex.X, 
                thirdVertex.Y - secondVertex.Y);
            // close triangle
            DrawLine(firstVertex.X - thirdVertex.X, 
                firstVertex.Y - thirdVertex.Y);
        }
        count++;
    }
}

/* Draws initial circle at random triangle point. */
void SetFirstCircle() {
    Point currentPoint = RandPoint();
    MovePen(currentPoint.X, currentPoint.Y);
    StartFilledRegion(1);
    DrawArc(CIRCLE_SIZE, 0, 360);
    EndFilledRegion();
}

/* Loop for drawing circles to fill the triangle. */
void DrawLoop() {
    printf("Click mouse to exit program.\n");
    while(true) {
        if (MouseButtonIsDown()) ExitGraphics(); // exit program with mouse click
        Point next = RandPoint();
        // calculate circle coordinates
        double nextX = ((next.X - GetCurrentX()) / 2) + GetCurrentX();
        double nextY = ((next.Y - GetCurrentY()) / 2) + GetCurrentY();
        MovePen(nextX, nextY);
        StartFilledRegion(1);
        DrawArc(CIRCLE_SIZE, 0, 360);
        EndFilledRegion();
        UpdateDisplay();
    }
}

/* Returns a random vertex from a triangle. */
Point RandPoint() {
    int rand = RandomInteger(1,3);
    Point pt;
    if (rand == 1) {
        pt = firstVertex;
    } else if (rand == 2) {
        pt = secondVertex;
    } else
        pt = thirdVertex;
    return pt;
}