import sys
from build_graph import Graph
N = int(sys.argv[1])

graph = Graph(N*2)
output = ''
for outer in range(N):
    for inner in range(N):
        if outer == inner:
            continue
        graph.addEdge(outer, inner)

graph.addEdge(N-1, N)
for outer in range(N, 2*N):
    for inner in range(N, 2*N):
        if outer == inner:
            continue
        graph.addEdge(outer, inner)

graph.writeTo(f'barbell_large_{2*N}.graph')