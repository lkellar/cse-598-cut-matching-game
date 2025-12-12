from build_graph import Graph

graph = Graph()

if graph.nodes % 2 != 0:
    print("Nodes must be divisible by 2")
    exit(1)

midpoint = graph.nodes // 2
for outer in range(midpoint):
    for inner in range(outer + 1, midpoint):
        if outer == inner:
            continue
        graph.addEdge(outer, inner)

graph.addEdge(midpoint - 1, midpoint)
for outer in range(midpoint, graph.nodes):
    for inner in range(outer + 1, graph.nodes):
        if outer == inner:
            continue
        graph.addEdge(outer, inner)

graph.writeTo()