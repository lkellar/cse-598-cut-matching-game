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
    // Modifies the graph where each edge (u,v) is split into two edges joined by a new node w, resulting in (u,w) and (w,v)
    void subdivideGraph();
    // generate new graph only containing nodes from the subset
    Graph getInducedGraph(const Subset& subset) const;
    // adds super source/sink nodes to the graph, where super source is connected to provided subset, and super sink is provided to the nodes not in the subset
    void addSourceSink(const Subset& sourceNodes);
    void display() const;
    // output in graphviz DOT format
    void displayDOT() const;
private:
    std::vector<std::vector<Edge>> adjacencyList;
    // assumes u and v are already nodes in the graph (the adjacencyList.size > u and > v)
    void addUndirectedEdge(int u, int v, int weight);
    // adds another node with provided edges (default none) to the adjacency list and returns its id
    int createNode(std::vector<Edge> edges = {});
    int nodeCount() const;
    friend class MaxFlow;
};

#endif /* Graph_h */
