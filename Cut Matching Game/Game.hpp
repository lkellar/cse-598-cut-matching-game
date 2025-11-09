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
    Game(int nodes);
    // end the round by adding the matching player's submission to the matrix
    void bumpRound(Matching& matching);
    //Bisection generateBisection(std::vector<Matching>& matchings);
    Subset generateCut();
    
private:
    // matrix that consists of M_1 * M_2 * M_3 .. up til the current round
    //MatrixXd matchings;
    std::vector<Matching> matchings;
    int nodes;
    std::vector<double> computeProjection();
    std::vector<double> generateRandomVector() const;
    double computeMedian(std::vector<double>& data) const;
};


#endif /* Game_hpp */
