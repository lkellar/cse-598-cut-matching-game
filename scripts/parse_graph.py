from build_graph import Graph, parser
import pathlib

args = parser.parse_args()

graph = Graph()
data = []
with open(args.input, 'r') as f:
    data = f.read().split('\n')

for idx, node in enumerate(data):
    if args.skip_first and idx == 0:
        continue
    splits = node.rstrip().split(' ')
    if len(splits) < 2:
        continue
    for index in range(1, len(splits), 2):
        to = int(splits[index])
        # NO SELF LOOPS
        if to != idx:
            graph.addEdge(idx, to)
            #print(idx, to)
    if idx % 500 == 0:
        print(idx)
    

graph.writeTo()