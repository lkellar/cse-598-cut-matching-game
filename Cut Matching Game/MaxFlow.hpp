//
//  MaxFlow.hpp
//  Cut Matching Game
//
//  Created by Lucas Kellar on 11/9/25.
//

#ifndef MaxFlow_hpp
#define MaxFlow_hpp

#include "Graph.hpp"

// CURRENT STATUS:
// - IGNORES WEIGHTS, e.g. all are capacity 1 (or, all inner edges will be set to capacity phiInverse)
// - ASSUMES UNDIRECTED GRAPHS
class MaxFlow {
public:
    // assumes graph is undirected, so the residual graph is equal to the graph
    MaxFlow(const Graph& graph, int source, int sink, int targetFlow, int phiInverse) ;
    
    virtual int computeMaxFlow() = 0;
    // assumes max flow has been run on residual graph
    Matching decomposeFlow();
protected:
    // edge weights represent capacities
    // we're hacking this a bit and treating undirected edges here as directed edges (e.g. weights are directional /represent residual capacity, connections are not)
    Graph residual;
    const Graph& original;
    int source;
    int sink;
    int targetFlow;
    int phiInverse;
    // Sets all capacities on inner edges (NOT connected to source or sink) to the argument.
    // Sets all capacities connected to source/sink to 1
    void setCapacities(int innerEdgeCapacities);
    // will fail if edge doesn't exist
    // this is incredibly slow and should be deleted
    Edge& findResidualEdgeTo(int from, int to);
    const int nodeCount;
};



#endif /* MaxFlow_hpp */
