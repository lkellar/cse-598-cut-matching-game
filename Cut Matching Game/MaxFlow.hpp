//
//  MaxFlow.hpp
//  Cut Matching Game
//
//  Created by Lucas Kellar on 11/9/25.
//

#ifndef MaxFlow_hpp
#define MaxFlow_hpp

#include "Graph.hpp"

class MaxFlow {
public:
    // assumes graph is undirected, so the residual graph is equal to the grah
    MaxFlow(Graph graph, int source, int sink);
    // implementation of Edmonds-Karp
    // https://en.wikipedia.org/wiki/Edmonds–Karp_algorithm
    // also referenced https://cp-algorithms.com/graph/edmonds_karp.html
    int computeMaxFlow();
private:
    // helper algorithm to run the breadth first search to find a flow
    int findFlow();
    // edge weights represent capacities
    Graph residual;
    int source;
    int sink;
    std::vector<int> parent;
};



#endif /* MaxFlow_hpp */
