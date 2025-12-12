# Generate Example Graphs in Adjacency List format
# Edges are generated randomly with unit capacity

import random
import math
from build_graph import Graph, parser

#https://stackoverflow.com/a/55245866/8525240
def indexToPair(i):
    k = math.floor((1+math.sqrt(1+8*i))/2)
    return k,i-k*(k-1)//2

def generate():
    graph = Graph()
    args = parser.parse_args()
    
    nodes = range(0, graph.nodes)
    maxEdges = (graph.nodes * (graph.nodes - 1)) // 2
    if args.edges > maxEdges:
        raise RuntimeError(f'Graph with {graph.nodes} nodes can\'t contain {args.edges} edges')
    edges = []
    
    for index in random.sample(range(maxEdges), args.edges):
        edges.append(indexToPair(index))
        
    adjacencyList = {k: [] for k in nodes}
    
    for edges in edges:
        graph.addEdge(edges[0], edges[1])
    
    graph.writeTo(args.output)
    
    
if __name__ == "__main__":
    generate()