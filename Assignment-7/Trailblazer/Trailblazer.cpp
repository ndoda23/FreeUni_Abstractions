/******************************************************************************
 * File: Trailblazer.cpp
 *
 * Implementation of the graph algorithms that comprise the Trailblazer
 * assignment.
 */

#include "Trailblazer.h"
#include "TrailblazerGraphics.h"
#include "TrailblazerTypes.h"
#include "TrailblazerPQueue.h"
#include <random.h>
using namespace std;

// makes each point gray 
void makeEachGray(Grid < double>&, Map <Loc, Color>&);

void findAllNegihbors(Loc, Grid<double>&, Vector<Loc>&);

Vector<Loc> returningPath(Map<Loc, Loc>&, Loc,Loc);

Map<Loc,Loc> clustering(int, int);

TrailblazerPQueue<Edge> insertEdges(int, int);

bool isValidPoint(int first, int second);

void addRightEdge(TrailblazerPQueue<Edge>& pqueue, Loc, int, int);

void addEdge(TrailblazerPQueue<Edge>&, Loc,int,int);

/* Function: shortestPath
 * 
 * Finds the shortest path between the locations given by start and end in the
 * specified world.	 The cost of moving from one edge to the next is specified
 * by the given cost function.	The resulting path is then returned as a
 * Vector<Loc> containing the locations to visit in the order in which they
 * would be visited.	If no path is found, this function should report an
 * error.
 *
 * In Part Two of this assignment, you will need to add an additional parameter
 * to this function that represents the heuristic to use while performing the
 * search.  Make sure to update both this implementation prototype and the
 * function prototype in Trailblazer.h.
 */
Vector<Loc> 
shortestPath(Loc start,
             Loc end,
             Grid<double>& world,
             double costFn(Loc from, Loc to, Grid<double>& world),
    double heuristic(Loc start, Loc end, Grid<double>& world)) {

    TrailblazerPQueue<Loc> priorityQueue;

    // this is map for colors for each location
    Map <Loc, Color> colors;
    makeEachGray(world,colors);
    colors[start] = YELLOW;

    // this is map for distance for each location
    Map<Loc, double> distance;
    distance[start] = 0;

    priorityQueue.enqueue(start, heuristic(start,end,world));
    colorCell(world, start, YELLOW);

    // to find out which is parent
    Map<Loc, Loc> parent;

    while (!priorityQueue.isEmpty()) {
        Loc lc = priorityQueue.dequeueMin();
        colors[lc] = GREEN;
        colorCell(world, lc, GREEN);

        if (lc == end) {
            break;
        }

        Vector<Loc> neighbors;
        findAllNegihbors(lc, world, neighbors);
        for (Loc neighborLoc : neighbors) {
            if (colors[neighborLoc] == GRAY) {
                colors[neighborLoc] = YELLOW;
                distance[neighborLoc] = costFn(lc, neighborLoc, world) + distance[lc];
                parent[neighborLoc] = lc;
                priorityQueue.enqueue(neighborLoc, costFn(lc, neighborLoc, world) + distance[lc]+heuristic(neighborLoc,end,world));
                colorCell(world, neighborLoc, YELLOW);

            }
            else if (colors[neighborLoc] == YELLOW && distance[neighborLoc] > costFn(lc, neighborLoc, world) + distance[lc]) {
                distance[neighborLoc] = costFn(lc, neighborLoc, world) + distance[lc];
                parent[neighborLoc] = lc;
                priorityQueue.decreaseKey(neighborLoc, costFn(lc, neighborLoc, world) + distance[lc] + heuristic(neighborLoc, end, world));
            }
        }
    }

    return returningPath(parent,start, end);
}

Set<Edge> createMaze(int numRows, int numCols) {
    Set<Edge> finResult;

    Map<Loc, Loc> clusters = clustering(numRows,numCols); 

    TrailblazerPQueue<Edge> priorityQueue = insertEdges(numRows,numCols);

    while (finResult.size() < numRows*numCols-1) {
        Edge edge = priorityQueue.dequeueMin();
        Loc firstEndP = edge.start;
        Loc secondEndP = edge.end;
        // first cluster
        while (clusters[firstEndP] != firstEndP) {
            firstEndP = clusters[firstEndP];
        }
        // second cluster
        while (clusters[secondEndP] != secondEndP) {
            secondEndP = clusters[secondEndP];
        }
        // end points are not in the same cluster.
        if (firstEndP != secondEndP) {
            clusters[firstEndP] = clusters[secondEndP];
            finResult.add(edge);
        }
    }
    return finResult;
}

// makes each point gray.
void makeEachGray(Grid <double>& world, Map <Loc, Color>& colors) {
    for (int row = 0; row < world.numRows(); row++) {
        for (int col = 0; col < world.numCols(); col++) {
            Loc location;
            location.row = row;
            location.col = col;
            // each color must be gray.
            colors[location] = GRAY;
        }
    }
}

// this method finds all neighbor locations for desirable location
void findAllNegihbors(Loc currLoc, Grid<double>& world, Vector<Loc>& neighbors) {
    for (int rowDif = -1; rowDif <= 1; rowDif++) {
        for (int colDif = -1; colDif <= 1; colDif++) {
            // if it is current point.
            if (rowDif == 0 && colDif == 0) continue;
            if (world.inBounds(rowDif + currLoc.row, colDif + currLoc.col)) {
                Loc loc;
                // row index
                loc.row = rowDif + currLoc.row;
                // col index
                loc.col = colDif + currLoc.col;
                neighbors.add(loc);
            }
        }
    }
}

// reconstructing path
Vector<Loc> returningPath(Map<Loc, Loc>& parent, Loc start, Loc end) {
    Vector<Loc> vec;

    if  (start != end && !parent.containsKey(end)) {
        return vec;
    }

    Loc location = end;
    while (location != start) {
        vec.insert(0, location);
        location = parent[location];
    }
    // start location
    vec.insert(0, start);
    return vec;
}

// it places each node into its cluster.
Map<Loc,Loc> clustering(int numRows, int numCols){

    Map<Loc, Loc> res;

    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            Loc location;
            location = makeLoc(i, j);
            res[location] = location;
        }
    }
    return res;
}

// inserting edges into a priority queue
TrailblazerPQueue<Edge> insertEdges(int numRows, int numCols) {
    TrailblazerPQueue<Edge> pqueue;

    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            Loc location;
            location = makeLoc(i, j);

            // if it's in bounds.
            if (isValidPoint(j, numCols)) {
                addRightEdge(pqueue, location,i,j);
            }

            // if it's in bounds.
            if (isValidPoint(i, numRows)) {
                addEdge(pqueue, location, i, j);
            }
        }
    }

    return pqueue;
}

// checks if it is in bounds
bool isValidPoint(int first, int second) {
    if (first + 1 < second) return true;
    return false;
}

// addes valid edges.
void addEdge(TrailblazerPQueue<Edge>& pqueue, Loc location , int i , int j) {
    Loc curr = makeLoc(i+1,j);
    Edge edge = makeEdge(location,curr);
    pqueue.enqueue(edge, randomReal(0, 1));
}

// addes valid edges.
void addRightEdge(TrailblazerPQueue<Edge>& pqueue, Loc location, int i, int j) {
    Loc curr = makeLoc(i, j+1);
    Edge edge = makeEdge(location, curr);
    pqueue.enqueue(edge, randomReal(0, 1));
}