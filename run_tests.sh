#!/bin/bash

ITERATIONS=1000

PROGRAM="./myprogram"

CONFIGS=(
    "-c 2 -p 2 -i 20 -s 1"
    "-c 4 -p 4 -i 40 -s 5 -d"
)

for ((i = 1; i <= ITERATIONS; i++)); do
    echo "Iteration $i/$ITERATIONS"
    for CONFIG in "${CONFIGS[@]}"; do
        echo "Running: $PROGRAM $CONFIG"
        $PROGRAM $CONFIG
        if [ $? -ne 0 ]; then
            echo "Test failed on iteration $i with config: $CONFIG"
            exit 1
        fi
    done
done

echo "All tests passed successfully!"