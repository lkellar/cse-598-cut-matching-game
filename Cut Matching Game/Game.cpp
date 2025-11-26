//
//  Game.cpp
//  Cut Matching Game
//
//  Created by Lucas Kellar on 11/4/25.
//

#include "Game.hpp"
#include "MaxFlow.hpp"
#include <random>
#include <algorithm>
#include <cassert>

#include <iostream>

std::random_device dev;
std::mt19937 gen(dev());
std::uniform_real_distribution<double> dis(0, 1);

Game::Game(const Graph& graph, int firstSplitNode, int pastSplitNode, int phiInverse) : graph(graph), firstSplitNode(firstSplitNode), pastSplitNode(pastSplitNode), splitNodeCount(pastSplitNode - firstSplitNode), phiInverse(phiInverse) { }

std::vector<double> Game::generateRandomVector() const {
    std::vector<double> random_vector;
    random_vector.resize(this->splitNodeCount);
    
    double sum = 0;
    for(int i = 0; i < splitNodeCount; ++i) {
        double next = dis(gen);
        random_vector[i] = next;
        sum += next * next;
    }
    
    double length = sqrt(sum);
    for(int i = 0; i < this->splitNodeCount; ++i) {
        random_vector[i] /= length;
    }
    
    return random_vector;
}


std::vector<double> Game::computeProjection() {
    std::vector<double> random_vector = this->generateRandomVector();
    
    for (auto& matching : this->matchings) {
        for (auto& pair : matching) {
            // assert that all matchings are between split nodes
            assert(firstSplitNode <= pair.first && pair.first < pastSplitNode);
            assert(firstSplitNode <= pair.second && pair.second < pastSplitNode);
            int shiftedFirst = pair.first - firstSplitNode;
            int shiftedSecond = pair.second - firstSplitNode;
            // set both nodes in the matching to the average
            double avg = (random_vector[shiftedFirst] + random_vector[shiftedSecond]) / 2;
            random_vector[shiftedFirst] = avg;
            random_vector[shiftedSecond] = avg;
        }
    }
    
    return random_vector;
}

std::pair<Subset, Subset> Game::generateCut() {
    std::vector<double> posVector = this->computeProjection();
    
    // TODO: handle odd number of split nodes later
    assert(this->splitNodeCount % 2 == 0);
    
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
    
    std::vector<std::pair<double, int>>::iterator median = pairedPosVector.begin() + this->splitNodeCount / 2;
    
    // rearranges the array in O(n) time to get everything below the median in the first half of the array and everything equal to or above in the second half
    std::nth_element(pairedPosVector.begin(), median, pairedPosVector.end(), compare);
    
    Subset cut, notCut;
    cut.reserve(this->splitNodeCount / 2);
    notCut.reserve(this->splitNodeCount / 2);
    
    for (auto it = pairedPosVector.begin(); it != pairedPosVector.end(); it++) {
        if (it < median) {
            cut.push_back(it->second + this->firstSplitNode);
        } else {
            notCut.push_back(it->second + this->firstSplitNode);
        }
    }
    
    return {cut, notCut};
}

Matching Game::generateMatching(const Cut& cut, Graph graph) {
    std::pair<int, int> sourceSink = graph.addSourceSink(cut);
    
    //std::cout << "Cut Size: " << cut.first.size() << std::endl;
    assert(this->splitNodeCount % 2 == 0);
    int targetFlow = this->splitNodeCount / 2;
    
    // target max flow should be n/2 where n is number of split nodes (so basically m/2)
    MaxFlow flow(graph, sourceSink.first, sourceSink.second, targetFlow, phiInverse);
    
    int maxFlow = flow.computeMaxFlow();
    
    std::cout << "Found Max Flow of " << maxFlow << " | Target was " << targetFlow << "\n";
    if (maxFlow * phiInverse < targetFlow) {
        std::cout << "Found 1/" << phiInverse << " cut in graph. Quitting\n";
        // TODO: actually report the cut
        exit(0);
    }
    Matching match = flow.decomposeFlow();
    
    return match;
}

void Game::bumpRound(Matching matching) {
    this->matchings.push_back(matching);
}

void Game::run() {
    for (int i = 0; i < 15; i++) {
        Cut cut = this->generateCut();
        Matching match = this->generateMatching(cut, graph);
        this->bumpRound(match);
    }
}
