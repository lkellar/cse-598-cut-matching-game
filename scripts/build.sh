#!/bin/sh

DIR="Cut Matching Game"
g++ -std=gnu++20 -O3 -Wall -Wextra "$DIR/main.cpp" "$DIR/EdmondsKarpMaxflow.cpp" "$DIR/PushRelabelMaxFlow.cpp" "$DIR/MaxFlow.cpp" "$DIR/Game.cpp" "$DIR/Graph.cpp" -o cmg