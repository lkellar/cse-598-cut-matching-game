//
//  Game.cpp
//  Cut Matching Game
//
//  Created by Lucas Kellar on 11/4/25.
//

#include "Game.hpp"
#include "EdmondsKarpMaxFlow.hpp"
#include "PushRelabelMaxFlow.hpp"
#include <random>
#include <algorithm>
#include <cassert>
#include <cmath>

#include <iostream>

std::random_device dev;
std::mt19937 gen(dev());
std::uniform_real_distribution<double> dis(0, 1);

Game::Game(const Graph& graph, int firstActiveNode, int pastActiveNode, int phiInverse, int randomVectorCount) : graph(graph), phiInverse(phiInverse), activeNodeCount(pastActiveNode - firstActiveNode), firstActiveNode(firstActiveNode), pastActiveNode(pastActiveNode),  randomVectorCount(randomVectorCount) {
    if (randomVectorCount != -1) {
        std::cout << "Using at maximum " << randomVectorCount << " random vectors\n";
        randomVectorCache.reserve(randomVectorCount);
    }
}

std::vector<double> Game::generateRandomVector() {
    std::vector<double> random_vector;
    random_vector.resize(this->activeNodeCount);
    
    double sum = 0;
    for(int i = 0; i < this->activeNodeCount; ++i) {
        double next = dis(gen);
        random_vector[i] = next;
        sum += next * next;
    }
    
    double length = sqrt(sum);
    for(int i = 0; i < this->activeNodeCount; ++i) {
        random_vector[i] /= length;
    }
    
    return random_vector;
}


std::vector<double> Game::computeProjection() {
    std::vector<double> random_vector = this->generateRandomVector();
    
    for (auto& matching : this->matchings) {
        applyMatchingToVector(random_vector, matching);
    }
    
    return random_vector;
}


void Game::applyMatchingToVector(std::vector<double>& posVector, const Matching& match) {
    for (auto& pair : match) {
        // assert that all matchings are between split nodes
        assert(firstActiveNode <= pair.first && pair.first < pastActiveNode);
        assert(firstActiveNode <= pair.second && pair.second < pastActiveNode);
        int shiftedFirst = pair.first - firstActiveNode;
        int shiftedSecond = pair.second - firstActiveNode;
        // set both nodes in the matching to the average
        double avg = (posVector[shiftedFirst] + posVector[shiftedSecond]) / 2;
        posVector[shiftedFirst] = avg;
        posVector[shiftedSecond] = avg;
    }
}


void Game::applyMatchingToCachedVectors(const Matching& match) {
    for (auto& vec : this->randomVectorCache) {
        applyMatchingToVector(vec, match);
    }
}

std::pair<Subset, Subset> Game::generateCut() {
    std::vector<double> posVector;
    if (this->randomVectorCount != -1) {
        posVector = this->randomVectorCache[this->currentRound % this->randomVectorCount];
    } else {
        posVector = this->computeProjection();
    }
    
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
    
    std::vector<std::pair<double, int>>::iterator median = pairedPosVector.begin() + this->activeNodeCount / 2;
    
    // rearranges the array in O(n) time to get everything below the median in the first half of the array and everything equal to or above in the second half
    std::nth_element(pairedPosVector.begin(), median, pairedPosVector.end(), compare);
    
    Subset cut, notCut;
    cut.reserve(this->activeNodeCount / 2);
    // in case its odd
    notCut.reserve((this->activeNodeCount / 2) + 1);
    
    for (auto it = pairedPosVector.begin(); it != pairedPosVector.end(); it++) {
        if (it < median) {
            cut.push_back(it->second + this->firstActiveNode);
        } else {
            notCut.push_back(it->second + this->firstActiveNode);
        }
    }
    
    return {cut, notCut};
}

Matching Game::generateMatching(const Cut& cut, Graph graph) {
    std::pair<int, int> sourceSink = graph.addSourceSink(cut);
    
    int targetFlow = this->activeNodeCount / 2;
    
    // target max flow should be n/2 where n is number of split nodes (so basically m/2)
    EdmondsKarpMaxFlow flow(graph, sourceSink.first, sourceSink.second, targetFlow, phiInverse);
    //PushRelabelMaxFlow pushFlow(graph, sourceSink.first, sourceSink.second, targetFlow, phiInverse);

    int maxFlow = flow.computeMaxFlow();
    //int pushMaxFlow = pushFlow.computeMaxFlow();
    
    // explicitly flush
    std::cout << "Edmonds Karp Max Flow: " << maxFlow << " | Target was " << targetFlow << std::endl;
    
    if (maxFlow < targetFlow) {
        std::cout << "Found 1/" << phiInverse << " cut in graph. Quitting\n";
        std::cout << "Took " << this->matchings.size() + 1 << " rounds to find the cut\n";
        exit(0);
    }
    // We can use a quirk of the graph setup / edmonds karp to find the matching within the edmonds karp flow process, without having to seperately decompse it
    Matching match;
    match.reserve(flow.matching.size());
    match.assign(flow.matching.begin(), flow.matching.end());
    //Matching match = flow.decomposeFlow();
    
    return match;
}

void Game::bumpRound(Matching matching) {
    this->matchings.push_back(matching);
    this->currentRound++;
    if (this->randomVectorCount != -1) {
        applyMatchingToCachedVectors(matching);
    }
}

void Game::run() {
    //int originalNodeCount = static_cast<double>(firstSplitNode);
    int originalNodeCount = this->graph.nodeCount();
    int rounds = std::ceil(pow(std::log2(originalNodeCount), 2));
    if (rounds < 10) {
        std::cout << "Estimated Rounds: " << rounds << ". Using a minimum of 10 rounds\n";
        rounds = 10;
    } else {
        std::cout << "Estimated Rounds: " << rounds << "\n";
    }
    // generate random vectors ahead of time. could be done on demand as well
    if (this->randomVectorCount != -1) {
        this->randomVectorCache.reserve(this->randomVectorCount);
        for (int index = 0; index < this->randomVectorCount; index++) {
            this->randomVectorCache.push_back(this->computeProjection());
        }
    }
    for (int i = 0; i < rounds; i++) {
        Cut cut = this->generateCut();
        Matching match = this->generateMatching(cut, graph);
        this->bumpRound(match);
    }
    std::cout << "Couldn't find min cut. Graph should be a 1/" << phiInverse << " expander\n";
}
