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

MaxFlow::MaxFlow(const Graph& graph, int source, int sink, int targetFlow, int phiInverse) : source(source), sink(sink), targetFlow(targetFlow), residual(graph), original(graph), phiInverse(phiInverse) {
    // stores node id that was used to get to a given vertex
    // assumes no multigraph
    this->parent = std::vector<int>(graph.nodeCount(), -1);
};

// helper algorithm to find
int MaxFlow::findFlow() {
    // before each round, wipe predicates
    std::fill(this->parent.begin(), this->parent.end(), -1);
    
    // stores: node, excess flow in queue
    std::queue<std::pair<int, int>> q;
    // technically isn't infinity but should be good enough
    q.push({this->source, std::numeric_limits<int>::max()});
    
    // run Breadth First Search to find flows
    while (!q.empty() && this->parent[this->sink] == -1) {
        int node = q.front().first;
        int flow = q.front().second;
        q.pop();
        
        for (Edge edge : residual.adjacencyList[node]) {
            int next = edge.to_vertex;
            // check if the neighbor has been visited yet and has capacity left (weight > 0)
            if (this->parent[next] == -1 && edge.weight > 0) {
                this->parent[next] = node;
                int newFlow = std::min(flow, edge.weight);
                if (next == sink) {
                    //std::cout << "Found flow with value " << newFlow << std::endl;
                    return newFlow;
                }
                q.push({next, newFlow});
            }
        }
    }

    // no flow found
    return 0;
}

int MaxFlow::computeMaxFlow() {
    this->setCapacities(this->phiInverse);
    int flow = 0;
    
    int next_flow = 0;
    
    while ((next_flow = this->findFlow()) > 0) {
        flow += next_flow;
        //std::cout << "Reverse path: ";
        int current = this->sink;
        while (current != this->source) {
            //std::cout << current << " - ";
            int prev = parent[current];
            assert(prev != -1);
            
            auto backward = std::find_if(this->residual.adjacencyList[prev].begin(), this->residual.adjacencyList[prev].end(), [current](Edge edge) {
                return edge.to_vertex == current;
            });
            assert(backward != this->residual.adjacencyList[prev].end());
            auto forward = std::find_if(this->residual.adjacencyList[current].begin(), this->residual.adjacencyList[current].end(), [prev](Edge edge) {
                return edge.to_vertex == prev;
            });
            assert(backward != this->residual.adjacencyList[current].end());
            
            backward->weight -= next_flow;
            forward->weight += next_flow;
            current = prev;
        }
        //std::cout << current << "\n\n";
    }
    
    return flow;
}


void MaxFlow::setCapacities(int innerEdgeCapacities) {
    for (int index = 0; index < this->residual.nodeCount(); index++) {
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
            // TODO: getting non existant incorrect edges??
            bool found = false;
            for (Edge& edge : this->residual.adjacencyList[current]) {
                int to_vertex = edge.to_vertex;
                auto original_edge = std::find_if(this->original.adjacencyList[current].begin(), this->original.adjacencyList[current].end(), [to_vertex](const Edge& edge) {
                    return edge.to_vertex == to_vertex;
                });
                assert(original_edge != this->original.adjacencyList[current].end());
                if (edge.weight > original_edge->weight) {
                    edge.weight = original_edge->weight;
                    
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
            }
            if (!found) {
                break;
            }
        }
        
        if (sourceConnect != -1 && sinkConnect != -1) {
            //std::cout << "matching " << sinkConnect << " and " << sourceConnect <<"\n";
            match.push_back({sinkConnect, sourceConnect});
        }
    } while (sinkConnect != -1 && sourceConnect != -1); // need to study this condition more
    //std::cout <<"okay so\n";
    return match;
}
