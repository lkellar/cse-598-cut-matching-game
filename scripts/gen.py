# Generate Example Graphs in Adjacency List format
# Edges are generated randomly with unit capacity
# Outputs in Chaco format
# https://chriswalshaw.co.uk/jostle/jostle-exe.pdf

from argparse import ArgumentParser
import pathlib
import random
import math

parser = ArgumentParser(prog='GraphGen')
parser.add_argument('output', type=pathlib.Path)
parser.add_argument('-n', '--nodes', type=int, default=100)
parser.add_argument('-m', '--edges', type=int, default = 1000)

#https://stackoverflow.com/a/55245866/8525240
def indexToPair(i):
    k = math.floor((1+math.sqrt(1+8*i))/2)
    return k,i-k*(k-1)//2

def generate():
    args = parser.parse_args()
    
    nodes = range(0, args.nodes)
    maxEdges = (args.nodes * (args.nodes - 1)) // 2
    if args.edges > maxEdges:
        raise RuntimeError(f'Graph with {args.nodes} nodes can\'t contain {args.edges} edges')
    edges = []
    
    for index in random.sample(range(maxEdges), args.edges):
        edges.append(indexToPair(index))
        
    adjacencyList = {k: [] for k in nodes}
    
    for edges in edges:
        adjacencyList[edges[0]].append(str(edges[1]))
        adjacencyList[edges[1]].append(str(edges[0]))
    
    output = ''
    output += f'{args.nodes} {args.edges}\n'
    for key in sorted(adjacencyList.keys()):
        if len(adjacencyList[key]) == 0:
            output += '\n'
            continue
        # output all edges as capacity 1 for now
        output += '1 '
        output += ' 1 '.join(adjacencyList[key])
        output += '\n'

    with open(args.output, 'w') as f:
        f.write(output)
    
    
if __name__ == "__main__":
    generate()