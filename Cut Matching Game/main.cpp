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
    if (argc != 3 && argc != 4) {
        std::cerr << "Expected 2 or 3  arguments: 1/phi, file, and #random_vectors (OPTIONAL)\n";
        return EXIT_FAILURE;
    }
    
    int phiInverse = atoi(argv[1]);
    
    // use -1 as an value for infinite if not present
    int randomVectorCount = -1;
    if (argc == 4) {
        randomVectorCount = atoi(argv[3]);
    }
    
    std::ifstream file(argv[2]);
    std::stringstream fileBuffer;
    
    if (file.is_open()) {
        fileBuffer << file.rdbuf();
    } else {
        std::cerr << "Error opening file (" << argv[2] <<"): " << strerror(errno) << "\n" ;
        return EXIT_FAILURE;
    }
    file.close();
    
    Graph graph(fileBuffer);
    int originalNodeCount = graph.nodeCount();
    //graph.displayDOT();
    graph.subdivideGraph();
    
    // initialize game, with index[nodes] being where the first split node starts and index[graph.nodeCount()] being right after the last split node
    Game game(graph, originalNodeCount, graph.nodeCount(), phiInverse, randomVectorCount);
    game.run();
    return EXIT_SUCCESS;
}
