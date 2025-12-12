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

// accepts input in CHACO format
// https://chriswalshaw.co.uk/jostle/jostle-exe.pdf
Graph::Graph( std::stringstream& buffer) {
    std::string line;
    
    int index = 0;
    std::getline(buffer, line);
    // first line is NODES EDGES so stoi will grab the nodes
    int nodes = stoi(line);
    this->adjacencyList.resize(nodes);

    while (std::getline(buffer, line)) {
        std::istringstream linebuf(line);
        std::vector<Edge> neighbors;
        std::string segment;
        
        while (linebuf >> segment) {
            int weight = std::stoi(segment);
            
            if (!(linebuf >> segment)) {
                std::cerr << "Found incomplete adjacency list on line " << (index+1) << "; found an weight with no neighbor\n";
                break;
            }
            int to = std::stoi(segment);
            // SKIP SELF LOOPS
            if (index == to) {
                continue;
            }
            
            neighbors.push_back(Edge(to, weight));
        }
        
        this->adjacencyList[index] = neighbors;
        index++;
    }
    
    if (index != nodes) {
        std::cerr << "Input buffer has " << index << " lines but expected data for " << nodes << " nodes.\n";
    }
}

int Graph::nodeCount() const {
    return static_cast<int>(this->adjacencyList.size());
}


void Graph::addUndirectedEdge(int u, int v, int weight) {
    // SKIP SELF LOOPS
    if (u == v) {
        std::cerr << "Skipping self-loop for " << u << "\n";
        return;
    }
    assert(this->nodeCount() > u);
    assert(this->nodeCount() > v);
    this->adjacencyList[u].push_back(Edge(v, weight));
    this->adjacencyList[v].push_back(Edge(u, weight));
}

void Graph::deleteUndirectedEdge(int u, int v) {
    //std::cout << "Deleting (" << u << ", " << v << ")\n";
    assert(this->nodeCount() > u);
    assert(this->nodeCount() > v);
    auto it = std::find_if(this->adjacencyList[u].begin(), this->adjacencyList[u].end(), [v](const Edge& edge) {
        return edge.to_vertex == v;
    });
    
    assert(it != this->adjacencyList[u].end());
    this->adjacencyList[u].erase(it);
    
    if (u == v) {
        return;
    }
    
    it = std::find_if(this->adjacencyList[v].begin(), this->adjacencyList[v].end(), [u](const Edge& edge) {
        return edge.to_vertex == u;
    });
    
    assert(it != this->adjacencyList[v].end());
    this->adjacencyList[v].erase(it);
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
    int initialNodeCount = this->nodeCount();
    for (int u = 0; u < initialNodeCount; u++) {
        // need to copy the vector because we're going to modify it
        std::vector<Edge> neighbors = this->adjacencyList[u];
        for (Edge edge : neighbors) {
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
            
            this->deleteUndirectedEdge(u, v);
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
            // ignore weights for now since we're mostly looking at unit capacity
            std::cout << index << " -- " << neighbor.to_vertex <<"\n"; /*<< " [label=" << neighbor.weight << "]\n";*/
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


std::pair<int, int> Graph::addSourceSink(const Cut& cut) {
    // presumably the cut is already sorted, so it shouldn't take much time
    Subset sortedCut = cut.first;
    std::sort(sortedCut.begin(), sortedCut.end());
    
    Subset sortedNotCut = cut.second;
    std::sort(sortedNotCut.begin(), sortedNotCut.end());
    
    Subset::iterator cutIt = sortedCut.begin();
    Subset::iterator notCutIt = sortedNotCut.begin();
    
    int initialNodeCount = this->nodeCount();
    
    int superSource = this->createNode();
    //std::cout << "super source: " << superSource << "\n";
    int superSink = this->createNode();
    //std::cout << "super sink: " << superSink << "\n";
    
    
    // TODO: probably need to change this, like to 1/phi
    int CAPACITY = 1;
    
    for (int node = 0; node < initialNodeCount; node++) {
        while (*cutIt < node && cutIt != sortedCut.end()) {
            cutIt++;
        }
        while (*notCutIt < node && notCutIt != sortedNotCut.end()) {
            notCutIt++;
        }
        if (*cutIt == node) {
            //std::cout << "adding source edge!\n";
            this->addUndirectedEdge(node, superSource, CAPACITY);
        } else if (*notCutIt == node) {
            //std::cout << "adding sink edge!\n";
            this->addUndirectedEdge(node, superSink, CAPACITY);
        }
    }
    
    return {superSource, superSink};
}
