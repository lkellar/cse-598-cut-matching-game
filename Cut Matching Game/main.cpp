//
//  main.cpp
//  Cut Matching Game
//
//  Created by Lucas Kellar on 10/22/25.
//

#include <iostream>
#include "Graph.hpp"
#include "Game.hpp"
#include "MaxFlow.hpp"
#include <fstream>
#include <sstream>


#include <random>

int main(int argc, const char * argv[]) {
    if (argc != 3) {
        std::cerr << "Expected 2 arguments: 1/phi, and file\n";
        return EXIT_FAILURE;
    }
    
    int phiInverse = atoi(argv[1]);
    
    std::ifstream file(argv[2]);
    std::stringstream fileBuffer;
    
    if (file.is_open()) {
        fileBuffer << file.rdbuf();
    } else {
        std::cerr << "Error opening file (" << argv[2] <<"): " << strerror(errno) << "\n" ;
        return EXIT_FAILURE;
    }
    file.close();
    
    std::cout << "Original Graph\n";
    Graph graph(fileBuffer);
    int originalNodeCount = graph.nodeCount();
    graph.displayDOT();
    graph.subdivideGraph();
    
    // initialize game, with index[nodes] being where the first split node starts and index[graph.nodeCount()] being right after the last split node
    Game game(graph, originalNodeCount, graph.nodeCount(), phiInverse);
    
    //Cut cut = game.generateCut();
   /* Cut cut = {{10,11,12,13,16,14,15,17,18,19}, {20,21,22,23,24,25,26,27,28,29}};
    Matching match = game.generateMatching(cut, graph);
    game.bumpRound(match);*/
    game.run();
    
    
    /*std::cout << "Subdivision\n";
    Graph subdivision = graph.createSubdivisionGraph();
    subdivision.displayDOT();*/
    
    /*Game game(nodes);
    Subset cut = game.generateCut();
    
    std::cout << "Cut\n";
    
    Graph induced = graph.getInducedGraph(cut);
    induced.display();*/
    
    
    return EXIT_SUCCESS;
}
