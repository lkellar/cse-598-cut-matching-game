//
//  Graph.hpp
//  Cut Matching Game
//
//  Created by Lucas Kellar on 10/23/25.
//

#ifndef Graph_h
#define Graph_h

#include <vector>
#include <sstream>

// Assumes edges are unit capacity
class Graph {
public:
    Graph(int nodes, std::stringstream& buffer);
    Graph(std::vector<std::vector<int>>&& adjacencyList);
    // Creates a graph where each edge (u,v) is split into two edges joined by a new node w, resulting in (u,w) and (w,v)
    Graph createSubdivisionGraph() const;
    void display() const;
    // output in graphviz DOT format
    void displayDOT() const;
private:
    std::vector<std::vector<int>> adjacencyList;
};

#endif /* Graph_h */
