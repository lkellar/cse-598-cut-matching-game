//
//  Game.cpp
//  Cut Matching Game
//
//  Created by Lucas Kellar on 11/4/25.
//

#include "Game.hpp"
#include <random>
#include <algorithm>
#include <cassert>

#include <iostream>

std::random_device dev;
std::mt19937 gen(dev());
std::uniform_real_distribution<double> dis(0, 1);

Game::Game(int nodes) : nodes(nodes) { }

std::vector<double> Game::generateRandomVector() const {
    std::vector<double> random_vector;
    random_vector.resize(this->nodes);
    
    double sum = 0;
    for(int i = 0; i < this->nodes; ++i) {
        double next = dis(gen);
        random_vector[i] = next;
        sum += next * next;
    }
    
    double length = sqrt(sum);
    for(int i = 0; i < this->nodes; ++i) {
        random_vector[i] /= length;
    }
    
    return random_vector;
}


std::vector<double> Game::computeProjection() {
    std::vector<double> random_vector = this->generateRandomVector();
    
    for (auto& matching : this->matchings) {
        for (auto& pair : matching) {
            // set both nodes in the matching to the average
            double avg = (random_vector[pair.first] + random_vector[pair.second]) / 2;
            random_vector[pair.first] = avg;
            random_vector[pair.second] = avg;
        }
    }
    
    return random_vector;
}

Subset Game::generateCut() {
    std::vector<double> posVector = this->computeProjection();
    
    // TODO: handle odd number of nodes later
    assert(this->nodes % 2 == 0);
    
    // so we can keep track of position and node when we find median
    std::vector<std::pair<double, int>> pairedPosVector;
    pairedPosVector.reserve(posVector.size());
    
    int index = 0;
    for (double pos : posVector) {
        pairedPosVector.push_back({pos, index});
        index++;
    }
    
    auto compare = [](std::pair<double, int> &left, std::pair<double, int> &right) {
        return left.first < right.first;
    };
    
    std::vector<std::pair<double, int>>::iterator median = pairedPosVector.begin() + this->nodes / 2;
    
    // rearranges the array in O(n) time to get everything below the median in the first half of the array and everything equal to or above in the second half
    std::nth_element(pairedPosVector.begin(), median, pairedPosVector.end(), compare);
    
    Subset subset;
    subset.reserve(this->nodes / 2);
    
    for (auto it = pairedPosVector.begin(); it < median; it++) {
        subset.push_back(it->second);
    }
    
    return subset;
}

