//
//  MaxFlow.cpp
//  Cut Matching Game
//
//  Created by Lucas Kellar on 11/9/25.
//

#include "MaxFlow.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>

MaxFlow::MaxFlow(const Graph& graph, int source, int sink, int targetFlow, int phiInverse) : residual(graph), original(graph), source(source), sink(sink), targetFlow(targetFlow), phiInverse(phiInverse), nodeCount(graph.nodeCount()) { };


void MaxFlow::setCapacities(int innerEdgeCapacities) {
    for (int index = 0; index < this->nodeCount; index++) {
        for (auto& neighbor : this->residual.adjacencyList[index]) {
            if (
                index == this->source ||
                neighbor.to_vertex == this->source ||
                index == this->sink ||
                neighbor.to_vertex == this->sink
            ) {
                neighbor.weight = 1;
            } else {
                neighbor.weight = innerEdgeCapacities;
            }
        }
    }
}

Matching MaxFlow::decomposeFlow() {
    Matching match;
    
    int sourceConnect = -1;
    int sinkConnect = -1;
    
    do {
        int current = this->sink;
        // these two represent the split node in the cut (connected to the source) and the split node NOT in the cut (connected to the sink)
        sourceConnect = -1;
        sinkConnect = -1;
        while (current != this->source) {
            bool found = false;
            int edgeIdx = 0;
            for (Edge& edge : this->residual.adjacencyList[current]) {
                int to_vertex = edge.to_vertex;
                // We're checking the same edge, not the reverse edge so the edge index should be the same
                // saves us a lookup
                assert(this->original.adjacencyList[current][edgeIdx].to_vertex == to_vertex);
                int original_edge_weight = this->original.adjacencyList[current][edgeIdx].weight;
                if (edge.weight > original_edge_weight) {
                    edge.weight = original_edge_weight;
                    
                    if (to_vertex == this->source) {
                        sourceConnect = current;
                    }
                    if (current == this->sink) {
                        sinkConnect = to_vertex;
                    }
                    current = to_vertex;
                    found = true;
                    break;
                }
                edgeIdx++;
            }
            if (!found) {
                break;
            }
        }
        
        if (sourceConnect != -1 && sinkConnect != -1) {
            //std::cout << "matching " << sinkConnect << " and " << sourceConnect <<"\n";
            match.push_back({sinkConnect, sourceConnect});
        }
    } while (sinkConnect != -1); // need to study this condition more
    //std::cout <<"okay so\n";
    return match;
}


Edge& MaxFlow::findResidualEdgeTo(int from, int to) {
    for (Edge& edge : this->residual.adjacencyList[from]) {
        if (edge.to_vertex == to) {
            return edge;
        }
    }
    assert(false);
}
