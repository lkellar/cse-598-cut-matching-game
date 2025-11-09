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
#include <utility>
#include <unordered_set>

class SubdivisionGraph;

using Matching = std::vector<std::pair<int, int>>;
using Subset = std::vector<int>;

struct Edge {
    int to_vertex;
    int weight;
};

// Assumes edges are unit capacity
class Graph {
public:
    // accepts a buffer of an adjacency list, where each line is neighbor,weight,neighbor,weight and so on
    Graph(int nodes, std::stringstream& buffer);
    explicit Graph(std::vector<std::vector<Edge>>&& adjacencyList);
    // Creates a graph where each edge (u,v) is split into two edges joined by a new node w, resulting in (u,w) and (w,v)
    SubdivisionGraph createSubdivisionGraph() const;
    // generate new graph only containing nodes from the subset
    Graph getInducedGraph(const Subset& subset) const;
    void display() const;
    // output in graphviz DOT format
    void displayDOT() const;
private:
    std::vector<std::vector<Edge>> adjacencyList;
};

class SubdivisionGraph : public Graph {
public:
    SubdivisionGraph(std::vector<std::vector<Edge>>&& adjacencyList, int firstSplitNode);
private:
    // index in the adjacency list of the first split node
    // e.g. if fsn is n; all 0...n-1 are normal nodes and n... are split nodes
    int firstSplitNode;
};

#endif /* Graph_h */
