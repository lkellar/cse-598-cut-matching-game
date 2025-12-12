#!/bin/zsh

ITERATIONS=$1
PHI_INVERSE=$2
INPUT=$3

TIMEFMT=$'================\nCPU\t%P\nuser\t%*U\nsystem\t%*S\ntotal\t%*E'

for i in {1..$ITERATIONS}; do
    echo "Iteration $i"
    time ./cmg $PHI_INVERSE $INPUT
    echo "\n"
done