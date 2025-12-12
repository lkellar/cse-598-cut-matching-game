# Outputs in Chaco format, unweighted
# https://chriswalshaw.co.uk/jostle/jostle-exe.pdf

from argparse import ArgumentParser
import pathlib

parser = ArgumentParser(prog='GraphGen')
parser.add_argument('output', type=pathlib.Path)
parser.add_argument('-n', '--nodes', type=int, default=100)
# sometimes ignored
parser.add_argument('-m', '--edges', type=int, default=1000)
parser.add_argument('-i', '--input', type=pathlib.Path)
parser.add_argument('-s', '--skip-first', action='store_true')

args = parser.parse_args()

class Graph:
    neighbors = []
    edgeCount = 0
    nodes = 0
    
    def __init__(self, nodes = args.nodes):
        self.neighbors = [[] for i in range(nodes)]
        self.nodes = nodes
        
    def addEdge(self, u, v):
        # no duplicates
        try:
            assert(u < len(self.neighbors) and v < len(self.neighbors))
        except AssertionError as e:
            print(f"Failed on ({u}, {v})")
            raise e
        if v in self.neighbors[u] or u in self.neighbors[v]:
            return
        # all edges are undirected and have capacity 1
        self.neighbors[u].append(v)
        self.neighbors[v].append(u)
        self.edgeCount += 1
    
    def writeTo(self, file=args.output):
        output = f'{len(self.neighbors)} {self.edgeCount}\n'
        for neighborList in self.neighbors:
            if len(neighborList) > 0:
                #output += '1 '
                #output += ' 1 '.join(neighborList)
                # nodes should be 1-indexed, so shift them all up
                output += ' '.join([str(u + 1) for u in neighborList])
            output += '\n'
        with open(file, 'w') as f:
            f.write(output)