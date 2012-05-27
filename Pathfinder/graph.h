#ifndef _graph_h
#define _graph_h

#include "genlib.h"
#include "set.h"
#include "map.h"

struct nodeT; /* Forward references to these two types so */
struct arcT; /* that the C++ compiler can recognize them. */

struct coordT
{
    double x;
    double y;
};


struct graphT 
{
    Set<nodeT *> nodes;
    Set<arcT *> arcs;
    Map<nodeT *> nodeMap;
};
        
struct nodeT 
{
    string name;
    coordT xy;
    Set<arcT *> arcs;
};

struct arcT 
{
    nodeT *start;
    nodeT *finish;
    double cost;
};

class Graph
{
    public:
        Graph();
        ~Graph();
        void clear();
        void addCity(string name, double x, double y);
        void addFlight(string start, string end, double cost);
        Set<nodeT *>::Iterator itrNodes();
        Set<arcT *>::Iterator itrArcs();
        Map<nodeT *>::Iterator itrMapNodes();
        Set<arcT *> getArcSet();
        Set<nodeT *> getNodeSet();
        Map<nodeT *> getNodeMap();
        void print();

    private:
        void addArc(nodeT *start, nodeT *end, double cost);
        graphT *g;
};
#endif