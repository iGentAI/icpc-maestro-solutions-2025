#!/bin/bash

# Compile the solution
echo "Compiling solution..."
g++ -O2 -std=c++17 -Wall solution.cpp -o solution
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

echo "Compilation successful!"
echo ""

# Run tests if test inputs exist
if [ -d "test_inputs" ]; then
    for test_file in test_inputs/test*.txt; do
        if [ -f "$test_file" ]; then
            test_name=$(basename "$test_file")
            echo "Running $test_name:"
            ./solution < "$test_file"
            echo ""
        fi
    done
else
    echo "No test_inputs directory found."
fi