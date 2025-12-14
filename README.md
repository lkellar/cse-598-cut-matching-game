# Cut Matching Game Implementation

An implementation of the [cut matching game](https://people.eecs.berkeley.edu/~vazirani/pubs/partitioning.pdf). Put simply, given an $\phi$ and a graph, the goal is to either find a $\phi$-sparse cut in the graph or certify it as a $\phi$-expander (no $\phi$-sparse cut exists). 

Additionally, this implementation aims to test if generating a new random vector for each round is necessary. You can set a maximum number of random vectors to be generated with a command line option (after that, previous generated vectors will be reused).

Written in pure C++. Currently uses an implementation of the [Edmonds-Karp](https://en.wikipedia.org/wiki/Edmonds–Karp_algorithm) for max flow. There's also a draft of [Push-Relabel](https://en.wikipedia.org/wiki/Push–relabel_maximum_flow_algorithm) (with the current-arc variation) that partially works, but is too slow to be used at the present.

## Build

You should just be able to clone the repo and run `scripts/build.sh` (run in the root directory). This should produce an executable called `cmg`. If you're looking to debug or configure it more, try opening / using the attached XCode project.

## Usage

The program accepts the following arguments:

`cmg phiInverse inputGraph #randomVectors (OPTIONAL)`

- `phiInverse`: Should represent the $1/\phi$ that we're trying to find a cut /expander for.
- `inputGraph`: Path to a graph in the [Chaco Format](https://chriswalshaw.co.uk/jostle/jostle-exe.pdf). Specificially this is a file where the first line is `#NODES #EDGES` and then each following line is an adjacency graph (so the second line is a space seperated list of the neighbors of node 1). Nodes are 1-indexed. At this time, only unit capacity graphs are supported (so weights shouldn't be included)
- `#randomVectors`: OPTIONAL. If set, no more than `#randomVectors` random vectors will be generated. If the number of rounds exceeds `#randomVectors`, previously generated random vectors will be used in the same order.

The program will run up to $(\log(n))^2$ iterations (or $10$ if $(\log(n))^2 < 10$) and will stop if a cut is found.

Additionally, a number of scritps exist in the `scripts/` folder like `run_iteration.sh` and `extract_rounds.py` that may be useful to running large batches of tests. There also exist many scripts to construct graphs for testing.