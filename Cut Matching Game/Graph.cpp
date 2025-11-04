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

Graph::Graph(int nodes, std::stringstream& buffer) {
    this->adjacencyList.resize(nodes);
    std::string line;
    
    int index = 0;
    while (std::getline(buffer, line)) {
        std::istringstream linebuf(line);
        std::vector<int> neighbors;
        std::string segment;
        while (std::getline(linebuf, segment, ',')) {
            neighbors.push_back(std::stoi(segment));
        }
        
        this->adjacencyList[index] = neighbors;
        index++;
    }
    
    if (index != nodes) {
        std::cerr << "Input buffer has " << index << " lines but expected data for " << nodes << " nodes.";
    }
}

Graph::Graph(std::vector<std::vector<int>>&& adjacencyList) {
    this->adjacencyList = adjacencyList;
}

Graph Graph::createSubdivisionGraph() const {
    // create new adjacency list for subdivided graph
    // TODO: could do more to reserve space in this list (since we know the sizes ahead of time)
    std::vector<std::vector<int>> subdividedList;
    int nextNodeId = static_cast<int>(this->adjacencyList.size());
    
    // could easily do this more accurately with the amount of existing nodes and new nodes we'll have
    subdividedList.resize(this->adjacencyList.size());
    
    for (int u = 0; u < this->adjacencyList.size(); u++) {
        for (int v : this->adjacencyList[u]) {
            // when we add the new node, we'll add both edges at once (so we label the new node once)
            // so we'll decide to do this when u < v
            if (v > u) {
                continue;
            }
            int nextNode = nextNodeId;

            subdividedList[u].push_back(nextNode);
            
            assert(subdividedList.size() == nextNodeId);
            subdividedList.push_back({u});
            
            // if u,v is not a subloop
            if (u != v) {
                subdividedList[v].push_back(nextNode);
                subdividedList[nextNode].push_back(v);
            }
            
            nextNodeId++;
        }
    }
    
    return Graph(std::move(subdividedList));
}

void Graph::display() const {
    for (size_t index = 0; index < this->adjacencyList.size(); index++) {
        std::cout << index << ": ";
        for (int neighbor : this->adjacencyList[index]) {
            std::cout << neighbor << " ";
        }
        std::cout << "\n";
    }
}

void Graph::displayDOT() const {
    std::cout << "graph {\n";
    for (size_t index = 0; index < this->adjacencyList.size(); index++) {
        for (int neighbor : this->adjacencyList[index]) {
            // since we're working with undirected graphs, only output each edge once
            if (index > neighbor) {
                continue;
            }
            std::cout << index << " -- " << neighbor << "\n";
        }
    }
    std::cout << "}\n";
}
