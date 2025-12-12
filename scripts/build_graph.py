class Graph:
    neighbors = []
    edgeCount = 0
    
    def __init__(self, nodes):
        self.neighbors = [[] for i in range(nodes)]
        
    def addEdge(self, u, v):
        # all edges are undirected and have capacity 1
        self.neighbors[u].append(str(v))
        self.neighbors[v].append(str(u))
        self.edgeCount += 1
    
    def writeTo(self, file):
        output = f'{len(self.neighbors)} {self.edgeCount}\n'
        for neighborList in self.neighbors:
            if len(neighborList) > 0:
                output += '1 '
                output += ' 1 '.join(neighborList)
            output += '\n'
        with open(file, 'w') as f:
            f.write(output)