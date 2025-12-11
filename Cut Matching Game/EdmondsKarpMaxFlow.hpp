//
//  EdmondsKarpMaxFlow.hpp
//  Cut Matching Game
//
//  Created by Lucas Kellar on 12/10/25.
//

#ifndef EdmondsKarpMaxFlow_hpp
#define EdmondsKarpMaxFlow_hpp

#include "MaxFlow.hpp"

class EdmondsKarpMaxFlow : public MaxFlow {
public:
    EdmondsKarpMaxFlow(const Graph& graph, int source, int sink, int targetFlow, int phiInverse);
    // implementation of Edmonds-Karp
    // https://en.wikipedia.org/wiki/Edmonds–Karp_algorithm
    // also referenced https://cp-algorithms.com/graph/edmonds_karp.html
    int computeMaxFlow();
private:
    // helper algorithm to run the breadth first search to find a flow
    int findFlow();
    // stores node id that was used to get to a given vertex
    // assumes no multigraph
    std::vector<int> parent;
};

#endif /* EdmondsKarpMaxFlow_hpp */
