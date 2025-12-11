//
//  PushRelabelMaxFlow.cpp
//  Cut Matching Game
//
//  Created by Lucas Kellar on 12/10/25.
//
// Most implemented from https://cp-algorithms.com/graph/push-relabel.html
// and https://cp-algorithms.com/graph/push-relabel-faster.html

#include "PushRelabelMaxFlow.hpp"
#include <cassert>

PushRelabelMaxFlow::PushRelabelMaxFlow(const Graph& graph, int source, int sink, int targetFlow, int phiInverse) : MaxFlow(graph, source, sink, targetFlow, phiInverse) { }

void PushRelabelMaxFlow::push(int u, int v) {
    Edge& edge = this->findResidualEdgeTo(u, v);
    Edge& backwards_edge = this->findResidualEdgeTo(v, u);
    int delta = std::min(excess[u], edge.weight);
    edge.weight -= delta;
    backwards_edge.weight += delta;
    excess[u] -= delta;
    excess[v] += delta;
    // if v newly has excess (we just gave it all the excess it has) and there's positive excess, add it to the queue
    if (delta > 0 && excess[v] == delta) {
        excess_vertices.push(v);
    }
}

void PushRelabelMaxFlow::relabel(int u) {
    int matchingHeight = this->nodeCount;
    for (auto& edge : this->residual.adjacencyList[u]) {
        if (edge.weight > 0) {
            matchingHeight = std::min(matchingHeight, this->height[edge.to_vertex]);
        }
    }
    if (matchingHeight <= nodeCount) {
        // set new height to 1 above the next vertex we can push to
        this->height[u] = matchingHeight += 1;
    }
}


void PushRelabelMaxFlow::update_max_height_vertices(std::vector<int>& max_height_vertices) {
    max_height_vertices.clear();
    for (int u = 0; u < nodeCount; u++) {
        if (u == source || u == sink || this->excess[u] <= 0) {
            continue;
        }
        int uHeight = height[u];
        // nodes above N are dead. ignore
        if (uHeight > nodeCount) {
            continue;
        }
        if (!max_height_vertices.empty() && height[max_height_vertices[0]] < uHeight) {
            max_height_vertices.clear();
        }
        if (max_height_vertices.empty() || height[max_height_vertices[0]] == uHeight) {
            max_height_vertices.push_back(u);
        }
    }
}

// current-arc add on
void PushRelabelMaxFlow::discharge(int u) {
    while (excess[u] > 0 && height[u] < nodeCount) {
        if (seen[u] < this->residual.adjacencyList[u].size()) {
            int edgeIdx = seen[u];
            Edge& edge = this->residual.adjacencyList[u][edgeIdx];
            if (edge.weight > 0 && height[u] > height[edge.to_vertex]) {
                push(u, edge.to_vertex);
            } else {
                seen[u]++;
            }
        } else {
            relabel(u);
            seen[u] = 0;
        }
    }
}

// bouncing back and forth between the current-arc and the highest vertices methods
int PushRelabelMaxFlow::computeMaxFlow() {
    this->setCapacities(this->phiInverse);
    this->height = std::vector<int>(this->nodeCount, 0);
    this->excess = std::vector<int>(this->nodeCount, 0);
    // TODO: remove if needed
    this->seen = std::vector<int>(this->nodeCount, 0);
    height[source] = nodeCount;
    excess[source] = std::numeric_limits<int>::max();
    
    /*std::vector<int> max_height_vertices;
    max_height_vertices.reserve(nodeCount);*/
    
    for (const Edge& edge : this->residual.adjacencyList[source]) {
        push(source, edge.to_vertex);
    }
    
    int iterations = 0;
    while (!this->excess_vertices.empty()) {
        int u = this->excess_vertices.front();
        this->excess_vertices.pop();
        if (u != source && u != sink) {
            discharge(u);
        }
        iterations++;
    }
    
    /*update_max_height_vertices(max_height_vertices);
    while (!max_height_vertices.empty()) {
        for (int u : max_height_vertices) {
            bool pushed = false;
            for (const Edge& edge : this->residual.adjacencyList[u]) {
                if (edge.weight > 0 && height[u] == height[edge.to_vertex] + 1) {
                    push(u, edge.to_vertex);
                    if (this->excess[u] <= 0) {
                        break;
                    }
                }
            }
            if (!pushed) {
                relabel(u);
                break;
            }
        }
    }*/
    
    int flow = 0;
    size_t sinkEdges = this->residual.adjacencyList[sink].size();
    for (int edgeIdx = 0; edgeIdx < sinkEdges; edgeIdx++) {
        const Edge& original_edge = this->original.adjacencyList[sink][edgeIdx];
        const Edge& residual_edge = this->residual.adjacencyList[sink][edgeIdx];
        assert(residual_edge.to_vertex == original_edge.to_vertex);
        flow += (residual_edge.weight - original_edge.weight);
    }
    return flow;
    
    //return excess[sink];
}
