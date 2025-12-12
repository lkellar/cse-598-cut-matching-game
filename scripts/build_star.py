from build_graph import Graph

graph = Graph()

for node in range(1, graph.nodes):
    graph.addEdge(0, node)

graph.writeTo()