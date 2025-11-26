//
//  Game.hpp
//  Cut Matching Game
//
//  Created by Lucas Kellar on 11/4/25.
//

#ifndef Game_hpp
#define Game_hpp

#include "Graph.hpp"

class Game {
public:
    // pass indexes of the first and last split node (exclusive), because we only generate cuts of split nodes
    Game(const Graph& graph, int firstSplitNode, int pastSplitNode, int phiInverse);
    // end the round by adding the matching player's submission to the matrix
    void bumpRound(Matching matching);
    // returns both sides of a cut of split nodes
    Cut generateCut();
    // need to copy the graph so we can add temporary source/sinks
    Matching generateMatching(const Cut& cut, Graph graph);
    void run();
private:
    const Graph& graph;
    std::vector<Matching> matchings;
    // represents 1/phi, but as an int (since most phi is 1 / int) instead of a double
    int phiInverse;
    int splitNodeCount;
    int firstSplitNode;
    int pastSplitNode;
    std::vector<double> computeProjection();
    std::vector<double> generateRandomVector() const;
    double computeMedian(std::vector<double>& data) const;
};


#endif /* Game_hpp */
