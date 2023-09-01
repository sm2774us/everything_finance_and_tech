#!/bin/bash

PRODUCE_GARBAGE=${1:-false}
ITERATIONS=${2:-1000000}

CMD="java -verbose:gc -Xms32m -Xmx64m -cp target/classes com.coralblocks.coralme.example.NoGCTest $PRODUCE_GARBAGE $ITERATIONS"

echo $CMD

$CMD


