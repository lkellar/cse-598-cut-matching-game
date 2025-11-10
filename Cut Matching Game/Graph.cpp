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
#include <queue>
#include <limits>

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

int Graph::nodeCount() const {
    return static_cast<int>(this->adjacencyList.size());
}


void Graph::addUndirectedEdge(int u, int v, int weight) {
    assert(this->nodeCount() > u);
    assert(this->nodeCount() > v);
    this->adjacencyList[u].push_back(Edge(v, weight));
    this->adjacencyList[v].push_back(Edge(u, weight));
}

int Graph::createNode(std::vector<Edge> edges) {
    int nextNodeId = this->nodeCount();
    this->adjacencyList.push_back(edges);
    return nextNodeId;
}

Graph::Graph(std::vector<std::vector<Edge>>&& adjacencyList) {
    this->adjacencyList = adjacencyList;
}

void Graph::subdivideGraph() {
    for (int u = 0; u < this->nodeCount(); u++) {
        for (Edge edge : this->adjacencyList[u]) {
            int v = edge.to_vertex;
            int weight = edge.weight;
            // when we add the new node, we'll add both edges at once (so we label the new node once)
            // so we'll decide to do this when u < v
            if (v > u) {
                continue;
            }
            int splitNodeId = this->createNode();
            
            this->addUndirectedEdge(u, splitNodeId, weight);
            
            // if u,v is not a subloop
            if (u != v) {
                this->addUndirectedEdge(v, splitNodeId, weight);
            }
        }
    }
}

void Graph::display() const {
    for (int index = 0; index < this->nodeCount(); index++) {
        std::cout << index << ": ";
        for (Edge neighbor : this->adjacencyList[index]) {
            std::cout << "(" << neighbor.to_vertex << "," << neighbor.weight << ") ";
        }
        std::cout << "\n";
    }
}

void Graph::displayDOT() const {
    std::cout << "graph {\n";
    for (int index = 0; index < this->nodeCount(); index++) {
        for (Edge neighbor : this->adjacencyList[index]) {
            // since we're working with undirected graphs, only output each edge once
            if (index > neighbor.to_vertex) {
                continue;
            }
            std::cout << index << " -- " << neighbor.to_vertex << " [label=" << neighbor.weight << "]\n";
        }
    }
    std::cout << "}\n";
}

Graph Graph::getInducedGraph(const Subset& subset) const {
    std::vector<std::vector<Edge>> inducedAdjacencyList;
    
    inducedAdjacencyList.resize(subset.size());
    
    // create a vector of old index -> new index. nodes not present in induced subgraph get -1
    std::vector<int> newEdgeMapping(this->nodeCount(), -1);
    
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


void Graph::addSourceSink(const Subset& sourceNodes) {
    // presumably the source is already sorted, so it shouldn't take much time
    Subset sortedCut = sourceNodes;
    std::sort(sortedCut.begin(), sortedCut.end());
    
    Subset::iterator it = sortedCut.begin();
    
    int initialNodeCount = this->nodeCount();
    
    int superSource = this->createNode();
    int superSink = this->createNode();
    
    // TODO: probably need to change this, like to 1/phi
    int CAPACITY = 1;
    
    for (int node = 0; node < initialNodeCount; node++) {
        while (*it < node && it != sortedCut.end()) {
            it++;
        }
        if (*it == node) {
            this->addUndirectedEdge(node, superSource, CAPACITY);
        } else {
            this->addUndirectedEdge(node, superSink, CAPACITY);
        }
    }
}
