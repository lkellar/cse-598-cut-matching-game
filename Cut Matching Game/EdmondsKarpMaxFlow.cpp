//
//  EdmondsKarpMaxFlow.cpp
//  Cut Matching Game
//
//  Created by Lucas Kellar on 12/10/25.
//

#include "EdmondsKarpMaxFlow.hpp"
#include <cassert>
#include <unordered_map>

EdmondsKarpMaxFlow::EdmondsKarpMaxFlow(const Graph& graph, int source, int sink, int targetFlow, int phiInverse) : MaxFlow(graph, source, sink, targetFlow, phiInverse) {
    // stores node id that was used to get to a given vertex
    // assumes no multigraph
    this->parent = std::vector<int>(graph.nodeCount(), -1);
};

// helper algorithm to find
int EdmondsKarpMaxFlow::findFlow() {
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

int EdmondsKarpMaxFlow::computeMaxFlow() {
    this->setCapacities(this->phiInverse);
    int flow = 0;
    
    int next_flow = 0;
    matching.clear();
    
    while ((next_flow = this->findFlow()) > 0) {
        int sourceConnect = -1;
        int sinkConnect = -1;
        
        flow += next_flow;
        // update the residual graph based on the found flow
        int current = this->sink;
        while (current != this->source) {
            int prev = parent[current];
            assert(prev != -1);
            
            Edge& forward = this->findResidualEdgeTo(prev, current);
            Edge& backward = this->findResidualEdgeTo(current, prev);
            
            if (prev == this->source) {
                sourceConnect = current;
            }
            if (current == this->sink) {
                sinkConnect = prev;
            }
            
            forward.weight -= next_flow;
            backward.weight += next_flow;
            current = prev;
        }
        
        assert(sourceConnect != -1 && sinkConnect != -1);
        matching[sinkConnect] = sourceConnect;
    }
    
    return flow;
}
