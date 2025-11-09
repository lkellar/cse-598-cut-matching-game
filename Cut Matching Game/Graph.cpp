//
//  Graph.cpp
//  Cut Matching Game
//
//  Created by Lucas Kellar on 10/30/25.
//

#include "Graph.hpp"
#include <sstream>
#include <string>
#include <iostream>
#include <cassert>
#include <algorithm>

Graph::Graph(int nodes, std::stringstream& buffer) {
    this->adjacencyList.resize(nodes);
    std::string line;
    
    int index = 0;
    while (std::getline(buffer, line)) {
        std::istringstream linebuf(line);
        std::vector<Edge> neighbors;
        std::string segment;
        
        while (std::getline(linebuf, segment, ',')) {
            int to = std::stoi(segment);
            
            if (!std::getline(linebuf, segment, ',')) {
                std::cerr << "Found incomplete adjacency list on line " << index << "; found an neighbor with no weight\n";
                break;
            }
            int weight = std::stoi(segment);
            
            neighbors.push_back(Edge(to, weight));
        }
        
        this->adjacencyList[index] = neighbors;
        index++;
    }
    
    if (index != nodes) {
        std::cerr << "Input buffer has " << index << " lines but expected data for " << nodes << " nodes.";
    }
}

Graph::Graph(std::vector<std::vector<Edge>>&& adjacencyList) {
    this->adjacencyList = adjacencyList;
}


SubdivisionGraph::SubdivisionGraph(std::vector<std::vector<Edge>>&& adjacencyList, int firstSplitNode) : Graph(std::move(adjacencyList)), firstSplitNode(firstSplitNode) { }


SubdivisionGraph Graph::createSubdivisionGraph() const {
    // create new adjacency list for subdivided graph
    // TODO: could do more to reserve space in this list (since we know the sizes ahead of time)
    std::vector<std::vector<Edge>> subdividedList;
    int originalNodeCount = static_cast<int>(this->adjacencyList.size());
    int nextNodeId = originalNodeCount;
    
    // could easily do this more accurately with the amount of existing nodes and new nodes we'll have
    subdividedList.resize(this->adjacencyList.size());
    
    for (int u = 0; u < this->adjacencyList.size(); u++) {
        for (Edge edge : this->adjacencyList[u]) {
            int v = edge.to_vertex;
            int weight = edge.weight;
            // when we add the new node, we'll add both edges at once (so we label the new node once)
            // so we'll decide to do this when u < v
            if (v > u) {
                continue;
            }
            int nextNode = nextNodeId;

            subdividedList[u].push_back(Edge(nextNode, weight));
            
            assert(subdividedList.size() == nextNodeId);
            subdividedList.push_back({Edge(u, weight)});
            
            // if u,v is not a subloop
            if (u != v) {
                subdividedList[v].push_back(Edge(nextNode, weight));
                subdividedList[nextNode].push_back(Edge(v, weight));
            }
            
            nextNodeId++;
        }
    }
    
    return SubdivisionGraph(std::move(subdividedList), originalNodeCount);
}

void Graph::display() const {
    for (size_t index = 0; index < this->adjacencyList.size(); index++) {
        std::cout << index << ": ";
        for (Edge neighbor : this->adjacencyList[index]) {
            std::cout << "(" << neighbor.to_vertex << "," << neighbor.weight << ") ";
        }
        std::cout << "\n";
    }
}

void Graph::displayDOT() const {
    std::cout << "graph {\n";
    for (size_t index = 0; index < this->adjacencyList.size(); index++) {
        for (Edge neighbor : this->adjacencyList[index]) {
            // since we're working with undirected graphs, only output each edge once
            if (index > neighbor.to_vertex) {
                continue;
            }
            std::cout << index << " -- " << neighbor.to_vertex << " [weight=" << neighbor.weight << "]\n";
        }
    }
    std::cout << "}\n";
}

Graph Graph::getInducedGraph(const Subset& subset) const {
    std::vector<std::vector<Edge>> inducedAdjacencyList;
    
    inducedAdjacencyList.resize(subset.size());
    
    // create a vector of old index -> new index. nodes not present in induced subgraph get -1
    std::vector<int> newEdgeMapping(this->adjacencyList.size(), -1);
    
    int nextIndex = 0;
    for (int node : subset) {
        newEdgeMapping[node] = nextIndex;
        nextIndex++;
    }
    
    for (int node : subset) {
        int newNodeLabel = newEdgeMapping[node];
        assert(newNodeLabel != -1);
        
        for (Edge neighbor : this->adjacencyList[node]) {
            int newNeighborLabel = newEdgeMapping[neighbor.to_vertex];
            if (newNeighborLabel != -1) {
                inducedAdjacencyList[newNodeLabel].push_back(Edge(newNeighborLabel, neighbor.weight));
            }
        }
    }
    
    return Graph(std::move(inducedAdjacencyList));
}

