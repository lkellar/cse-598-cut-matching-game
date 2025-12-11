import sys
N = int(sys.argv[1])

edges = 0
output = ''
for outer in range(N):
    for inner in range(N):
        if outer == inner:
            continue
        edges += 1
        output += f"1 {inner}"
        if inner != N-1:
            output += " "
    
    if outer != N-1:
        output += "\n"

edges += 2
output += f'1 {N}\n1 {N-1} '

for outer in range(N, 2*N):
    for inner in range(N, 2*N):
        if outer == inner:
            continue
        edges += 1
        output += f"1 {inner}"
        if inner != N-1:
            output += " "
    output += "\n"
    
with open(f'barbell_large_{2*N}.graph', 'w') as f:
    f.write(f'{2 *N} {edges}\n')
    f.write(output)