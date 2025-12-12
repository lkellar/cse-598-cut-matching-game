from build_graph import Graph

graph = Graph()

for node in range(1, graph.nodes):
    graph.addEdge(node - 1, node)

graph.writeTo()