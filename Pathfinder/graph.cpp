#include "graph.h"
#include <iostream>

Graph::Graph()
{
    g = new graphT;
}

Graph::~Graph()
{
    clear();
    delete g;
}

Set<nodeT *> Graph::getNodeSet()
{
    return g->nodes;
}
Map<nodeT *> Graph::getNodeMap()
{
    return g->nodeMap;
}

Set<arcT *> Graph::getArcSet()
{
    return g->arcs;
}

void Graph::clear()
{
    Set<nodeT *>::Iterator itrNodes = g->nodes.iterator();
    while(itrNodes.hasNext())
    {
        nodeT *node = itrNodes.next();
        delete node;
    }
    
    Set<arcT *>::Iterator itrArcs = g->arcs.iterator();
    while(itrArcs.hasNext())
    {
        arcT *arc = itrArcs.next();
        delete arc;
    }
    g->arcs.clear();
    g->nodes.clear();
    g->nodeMap.clear();
}

void Graph::addCity(string name, double x, double y)
{
    nodeT *node = new nodeT;
    node->name = name;
    node->xy.x = x;
    node->xy.y = y;
    g->nodes.add(node);
    g->nodeMap[name] = node;
}

void Graph::addFlight(string start, string end, double cost)
{
    nodeT *n1 = g->nodeMap[start];
    nodeT *n2 = g->nodeMap[end];
    addArc(n1, n2, cost);
    addArc(n2, n1, cost);
}

void Graph::addArc(nodeT *start, nodeT *end, double cost)
{
    arcT *arc = new arcT;
    arc->start = start;
    arc->finish = end;
    arc->cost = cost;
    g->arcs.add(arc);
    start->arcs.add(arc);
}

Set<nodeT *>::Iterator Graph::itrNodes()
{
    return g->nodes.iterator();
}

Set<arcT *>::Iterator Graph::itrArcs()
{
    return g->arcs.iterator();
}

Map<nodeT *>::Iterator Graph::itrMapNodes()
{
    return g->nodeMap.iterator();
}

void Graph::print()
{
    cout << endl;
    Set<nodeT *>::Iterator itrNodes = g->nodes.iterator();
    while(itrNodes.hasNext())
    {
        nodeT *node = itrNodes.next();
        Set<arcT *>::Iterator itrArcs = node->arcs.iterator();
        cout << node->name << " x=" << node->xy.x << " y=" << node->xy.y << " -> ";
        while(itrArcs.hasNext())
        {
            arcT *arc = itrArcs.next();
            cout << arc->finish->name << ", ";
        }
        cout << endl;
    }
    cout << endl;
}
