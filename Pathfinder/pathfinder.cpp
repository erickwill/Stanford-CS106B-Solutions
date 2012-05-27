/* pathfinder.cpp
 * ---------------
 * A starter file with some starting constants and handy utility routines.
 */

#include <fstream>
#include "genlib.h"
#include "extgraph.h"
#include <iostream>
#include <cmath>
#include "pqheap.h"
#include "simpio.h"
#include "graph.h"
#include "strutils.h"
#include "vector.h"

/* Constants
 * --------
 * A few program-wide constants concerning the graphical display.
 */
const double CircleRadius =.05;         // the radius of a node
const int LabelFontSize = 9;          // for node name labels

/* Function: DrawFilledCircleWithLabel
 * Usage:  DrawFilledCircleWithLabel(center, "Green", "You are here");
 * -------------------------------------------------------------------
 * Uses facilities from extgraph to draw a circle filled with
 * color specified. The circle is centered at the given coord has the
 * specified radius.  A label is drawn to the right of the circle.
 * You can leave off the last argument if no label is desired.
 */
void DrawFilledCircleWithLabel(coordT center, string color, string label = "")
{
    MovePen(center.x + CircleRadius, center.y);
    SetPenColor(color);
    StartFilledRegion(1.0);
    DrawArc(CircleRadius, 0, 360);
    EndFilledRegion();
    if (!label.empty()) {
        MovePen(center.x + CircleRadius, center.y);
        SetFont("Helvetica");
        SetPointSize(LabelFontSize);
        DrawTextString(label);
    }
}

/* Function: DrawLineBetween
 * Usage:  DrawLineBetween(coord1, coord2, "Black");
 * -------------------------------------------------
 * Uses facilities from extgraph to draw a line of the
 * specified color between the two given coordinates.
 */
void DrawLineBetween(coordT start, coordT end, string color)
{
    SetPenColor(color);
    MovePen(start.x, start.y);
    DrawLine(end.x - start.x, end.y - start.y);
}


/* Function: GetMouseClick
 * Usage:  loc = GetMouseClick();
 * ------------------------------
 * Waits for the user to click somewhere on the graphics window
 * and returns the coordinate of where the click took place.
 */
coordT GetMouseClick()
{
    coordT where;
    WaitForMouseDown();
    WaitForMouseUp();
    where.x = GetMouseX();
    where.y = GetMouseY();
    return where;
}

/* Function: WithinDistance
 * Usage:  if (WithinDistance(click, pt))...
 * -----------------------------------------
 * Returns true if the Cartesian distance between the two coordinates
 * is <= the specified distance, false otherwise. If not specified,
 * the distance is assumed to be size of the node diameter on screen.
 */
bool WithinDistance(coordT pt1, coordT pt2, double maxDistance = CircleRadius*2)
{
    double dx = pt2.x - pt1.x;
    double dy = pt2.y - pt1.y;
    double distance = sqrt(dx*dx + dy*dy);
    return (distance <= maxDistance);
}

string drawBackground(ifstream &in)
{
    string image;
    in >> image;
    DrawNamedPicture(image);
    return image;
}

string nextToken(ifstream &in)
{
    string line;
    in >> line;
    return line;
}

void readArcs(Graph &graph, ifstream &in)
{
    while(!in.fail())
    {
        string start = nextToken(in);
        string end = nextToken(in);
        if (end == "") return;
        double cost = StringToReal(nextToken(in));
        graph.addFlight(start,end,cost);
    }
}

void readFile(Graph &graph, ifstream &in)
{
    while (!in.fail())
    {
        string line = nextToken(in);
        if (line == "NODES")
        {
            line = nextToken(in);
        }
        else if (line == "ARCS")
        {
            readArcs(graph,in);
            return;
        }
        string city = line;
        double xCoord = StringToReal(nextToken(in));
        double yCoord = StringToReal(nextToken(in));
        graph.addCity(city, xCoord, yCoord);
    }    
}

string loadGraphFile(Graph &graph)
{
    ifstream in;
    while(true)
    {
        cout << "Enter name of graph data file: ";
        string filename = GetLine();
        in.open(filename.c_str());
        if(in.good()) break;
        cout << "Invalid file. Try again." << endl;
        in.clear();
    }
    InitGraphics();
    string image = drawBackground(in);
    readFile(graph,in);
    in.close();
    return image;
}

int arcCmp(arcT *a, arcT *b)
{
    string aStart = a->start->name;
    string aEnd = a->finish->name;
    string bStart = b->start->name;
    string bEnd = b->finish->name;
    if((aStart == bStart && aEnd == bEnd) ||
        (aStart == bEnd && aEnd == bStart))
        return 0;
    else
        return 1;
}

void drawCities(Graph &graph)
{
    Set<nodeT *>::Iterator itrNode = graph.itrNodes();
    while(itrNode.hasNext())
    {
        nodeT *node = itrNode.next();
        DrawFilledCircleWithLabel(node->xy, "black", node->name);
    }
}

void drawArcs(Graph &graph)
{
    Set<arcT *> drawnArcs(arcCmp);
    Set<arcT *>::Iterator itrArc = graph.itrArcs();
    while(itrArc.hasNext())
    {
        arcT *arc = itrArc.next();
        if (!drawnArcs.contains(arc))
        {
            DrawLineBetween(arc->start->xy,arc->finish->xy, "black");
        }
        drawnArcs.add(arc);
    }
}

void drawArcs(Set<arcT *> &arcs)
{
    double cost = 0;
    Set<arcT *>::Iterator itrArc = arcs.iterator();
    while(itrArc.hasNext())
    {
        arcT *arc = itrArc.next();
        DrawLineBetween(arc->start->xy,arc->finish->xy, "red");
        cost += arc->cost;
    }
    cout << "Total Cost: " << cost << endl;
}


double TotalPathDistance(Vector<arcT *> path) 
{
    double distance = 0;
    for (int i = 0; i < path.size(); i++)
    {
        distance += path[i]->cost;
    }
    return distance;
}

int pathCmp(Vector<arcT *> a, Vector<arcT *> b)
{
    double aCost = TotalPathDistance(a);
    double bCost = TotalPathDistance(b);
    if (aCost > bCost) return -1;
    else if (bCost > aCost) return 1;
    return 0;
}

Vector<arcT *> FindShortestPath(nodeT *start, nodeT *finish) 
{
    Vector<arcT *> path;
    PQueue<Vector<arcT *> > queue(pathCmp);
    Map<double> fixed;
    int count = 0;
    while (start != finish) 
    {
        if (!fixed.containsKey(start->name)) 
        {
            fixed.add(start->name, TotalPathDistance(path));
            Set<arcT *>::Iterator itr = start->arcs.iterator();
            while(itr.hasNext()) 
            {
                arcT *arc = itr.next();
                if (!fixed.containsKey(arc->finish->name)) 
                {
                    Vector<arcT *> newPath = path;
                    newPath.add(arc);
                    queue.enqueue(newPath);
                }
            }
        }
        if (queue.isEmpty()) return Vector<arcT *>();
        path = queue.dequeueMax();
        count++;
        start = path[path.size() - 1]->finish;
    }
    cout << "Dequeued paths = " << count << endl;
    return path;
}

nodeT * GetCities(Graph &graph)
{
    while(true)
    {
        coordT cityA = GetMouseClick();
        Set<nodeT *>::Iterator itrCityA = graph.itrNodes();
        while(itrCityA.hasNext())
        {
            nodeT *node = itrCityA.next();
            if (WithinDistance(cityA,node->xy,CircleRadius))
            {
                return node;
            }
        }
    }
}

void minPath(Graph &graph)
{
    cout << "Choose starting city: ";
    nodeT *start = GetCities(graph);
    cout << start->name << endl;

    nodeT *end;
    while(true)
    {
        cout << "Choose destination city: ";
        end = GetCities(graph);
        if (end->name != start->name)
        {
            cout << end->name << endl;
            break;
        }
        cout << endl << "Please choose a different city." << endl;
    }
    
    double cost = 0;
    Vector<arcT *> path = FindShortestPath(start,end);

    for (int i = 0; i < path.size(); i++)
    {
        cost += path[i]->cost;
        DrawLineBetween(path[i]->start->xy, path[i]->finish->xy,"red");
        DrawFilledCircleWithLabel(path[i]->start->xy, "red", path[i]->start->name);
        cout << path[i]->start->name << " -> " << path[i]->finish->name << endl;
    }
    DrawFilledCircleWithLabel(path[path.size()-1]->finish->xy, 
        "red", path[path.size()-1]->finish->name);
    cout << "Total Cost = " << cost << endl;
    cout << "Total paths = " << path.size() << endl;
}

int menu()
{
    while(true)
    {
        cout << endl;
        cout << "Your options are: " << endl;
        cout << "    (1) Choose a new graph file" << endl <<
            "    (2) Find shortest path using Dijkstra's algorithm" << endl <<
            "    (3) Compute the minimal spanning tree using Kruskal's algorithm" << endl <<
            "    (4) Quit" << endl << "Enter Selection: ";
        int choice = GetInteger();
        if (choice >= 1 && choice <= 4) return choice;
        cout << "Invalid choice. Try again." << endl;

    }
}

int arcCostCmp(arcT *a, arcT *b)
{
    double aCost = a->cost;
    double bCost = b->cost;
    
    if(aCost > bCost) return -1;
    if(bCost > aCost) return 1;
    return 0;
}

/*
* Kruskal implementation code taken from
* http://www.codingfriends.com/index.php/2010/07/07/graphs-part-2/
*
*/
Set<arcT *> kruskal(Graph &graph)
{
    cout << endl << "Calculating minimum spanning tree..." << endl;
    PQueue<arcT *> path(arcCostCmp);
    Vector< Set<string> > nodes;
    Set<arcT *> minTree;
 
    // place into a pqueue and also build up a vector of sets of the different node names
    Set<arcT *>::Iterator itrArcSet = graph.itrArcs();
    while(itrArcSet.hasNext())
    {
        arcT *arc = itrArcSet.next();
        path.enqueue(arc);
        bool inAlready = false;
        for (int i = 0; i < nodes.size(); i++)
        {
            if (nodes[i].contains(arc->start->name))
            {
                inAlready = true;
                break;
            }
        }
        if (!inAlready)
        {
            Set<string> newNode;
            newNode.add(arc->start->name);
            nodes.add(newNode);
        }
    }

    while (!path.isEmpty())
    {
        arcT * arc = path.dequeueMax();
        // start node and end nodes set id
        int startN, endN;
        startN = endN = -1;
        for (int i =0; i < nodes.size(); i++)
        {
            if (nodes[i].contains(arc->start->name))
                startN = i;
            if (nodes[i].contains(arc->finish->name))
                endN = i;
        }
        // if in different sets then
        if (startN != endN)
        {
            nodes[startN].unionWith(nodes[endN]);
            nodes.removeAt(endN);
            minTree.add(arc);
        } 
    }
    return minTree;
}

int main()
{
    InitGraphics();
    SetWindowTitle("CS106 Pathfinder");
    cout << "This masterful piece of work is a graph extravaganza!" << endl
        << "The main attractions include a lovely visual presentation of the graph" << endl
        << "along with an implementation of Dijkstra's shortest path algorithm and" << endl
        << "the computation of a minimal spanning tree.  Enjoy!" << endl << endl;
    
    Graph graph;
    string image = loadGraphFile(graph);
    drawCities(graph);
    drawArcs(graph);
    Set<arcT *> minTree;

    while(true)
    {
        int selection = menu();
        switch (selection)
        {
            case 1:
                cout << endl;
                graph.clear();
                image = loadGraphFile(graph);
                drawCities(graph);
                drawArcs(graph);
                break;
            case 2:
                cout << endl;
                InitGraphics();
                DrawNamedPicture(image);
                drawCities(graph);
                drawArcs(graph);
                minPath(graph);
                cout << "Press Enter to continue...";
                GetLine();
                break;
            case 3:
                InitGraphics();
                DrawNamedPicture(image);
                drawCities(graph);
                minTree = kruskal(graph);
                drawArcs(minTree);
                cout << "Press Enter to continue...";
                GetLine();
                break;
            case 4:
                exit(0);
        }
    }
    return (0);
}