//
//  PushRelabelMaxFlow.hpp
//  Cut Matching Game
//
//  Created by Lucas Kellar on 12/10/25.
//
// Mostly implemented https://cp-algorithms.com/graph/push-relabel-faster.html

#ifndef PushRelabelMaxFlow_hpp
#define PushRelabelMaxFlow_hpp

#include "MaxFlow.hpp"
#include <queue>

class PushRelabelMaxFlow : public MaxFlow {
public:
    PushRelabelMaxFlow(const Graph& graph, int source, int sink, int targetFlow, int phiInverse);
    // implementation of Push Relabel max flow
    // https://en.wikipedia.org/wiki/Push–relabel_maximum_flow_algorithm
    // also referenced https://cp-algorithms.com/graph/push-relabel.html
    int computeMaxFlow();
private:
    void update_max_height_vertices(std::vector<int>& max_height_vertices);
    void relabel(int u);
    void push(int u, int v);
    void discharge(int u);
    std::vector<int> height;
    std::vector<int> excess;
    std::queue<int> excess_vertices;
    // current-arc
    std::vector<int> seen;
};

#endif /* PushRelabelMaxFlow_hpp */
