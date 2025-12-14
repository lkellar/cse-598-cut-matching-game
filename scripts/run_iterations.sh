#!/bin/zsh

ITERATIONS=$1
PHI_INVERSE=$2
INPUT=$3

TIMEFMT=$'================\nCPU\t%P\nuser\t%*U\nsystem\t%*S\ntotal\t%*E'

for i in {1..$ITERATIONS}; do
    echo "Iteration $i"
    if [ "$#" -ge 4 ]; then
    # send random vector argument
        #time ./cmg $PHI_INVERSE $INPUT $4
        ./cmg $PHI_INVERSE $INPUT $4
    else
        #time 
        ./cmg $PHI_INVERSE $INPUT
    fi
    echo "\n"
done