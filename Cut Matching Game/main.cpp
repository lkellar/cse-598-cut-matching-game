//
//  main.cpp
//  Cut Matching Game
//
//  Created by Lucas Kellar on 10/22/25.
//

#include <iostream>
#include "Graph.hpp"
#include <fstream>
#include <sstream>

int main(int argc, const char * argv[]) {
    if (argc != 3) {
        std::cerr << "Expected 2 arguments: line count and file\n";
        return EXIT_FAILURE;
    }
    
    std::ifstream file(argv[2]);
    std::stringstream fileBuffer;
    
    int nodes = atoi(argv[1]);
    
    if (file.is_open()) {
        fileBuffer << file.rdbuf();
    } else {
        std::cerr << "Error opening file (" << argv[1] <<"): " << strerror(errno) << "\n" ;
        return EXIT_FAILURE;
    }
    file.close();
    
    Graph graph(nodes, fileBuffer);
    graph.displayDOT();
    std::cout << "Subdivision\n";
    Graph subdivision = graph.createSubdivisionGraph();
    subdivision.displayDOT();
    
    return EXIT_SUCCESS;
}
