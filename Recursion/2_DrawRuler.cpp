#include "genlib.h"
#include "graphics.h"
#include <iostream>
#include "extgraph.h"

void DrawRuler(double x, double y, double w, double h);
void RecDrawRuler(double x, double y, double w, double h);

int main ()
{
    InitGraphics();
    double x,y = 0;
    double width = GetWindowWidth();
    double height = GetWindowHeight()/3;
    DrawRuler(x,y,width,height);

    return 0;
}

void DrawRuler(double x, double y, double w, double h)
{
    MovePen(x,y); 
    DrawLine(w, 0); // draw horizontal line along base
    RecDrawRuler(x,y,w,h); // recursively draw vertical lines
}

void RecDrawRuler(double x, double y, double w, double h)
{
    if (h < .05) return;
    double middle = (x/2)+(w/2);
    MovePen(middle, y); // move pen to center of X-axis
    DrawLine(0,h);
    RecDrawRuler(x, y, middle, h/2); // draw left
    RecDrawRuler(middle, y, w, h/2); // draw right
}