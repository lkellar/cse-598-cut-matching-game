from build_graph import Graph

print("Ignoring arguments - writing to current directory")

def buildHalfExpander():
    graph = Graph(12)
    
    for outer in range(0, 6):
        for inner in range(outer + 1, 6):
            graph.addEdge(outer, inner)
            
    for outer in range(6, 12):
        for inner in range(outer + 1, 12):
            graph.addEdge(outer, inner)
    
    graph.addEdge(5,6)
    graph.addEdge(4,7)
    graph.addEdge(3,6)
            
    graph.writeTo('1-2-expander.graph')

def buildThirdExpander():
    graph = Graph(18)
    
    for outer in range(0, 9):
        for inner in range(outer + 1, 9):
            graph.addEdge(outer, inner)
            
    for outer in range(9, 18):
        for inner in range(outer + 1, 18):
            graph.addEdge(outer, inner)
    
    graph.addEdge(8,9)
    graph.addEdge(7,10)
    graph.addEdge(6,11)
            
    graph.writeTo('1-3-expander.graph')
    

def buildQuarterExpander():
    graph = Graph(16)
    
    for outer in range(0, 8):
        for inner in range(outer + 1, 8):
            graph.addEdge(outer, inner)
            
    for outer in range(8, 16):
        for inner in range(outer + 1, 16):
            graph.addEdge(outer, inner)
    
    graph.addEdge(7,8)
    graph.addEdge(6,10)
            
    graph.writeTo('1-4-expander.graph')

buildHalfExpander()
buildThirdExpander()
buildQuarterExpander()