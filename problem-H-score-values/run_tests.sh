#!/bin/bash

# Compile the solution
echo "Compiling solution.cpp..."
g++ -std=c++17 -O2 -Wall -o solution solution.cpp

if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

echo "Compilation successful!"
echo ""

# Function to run tests
run_test() {
    local test_num=$1
    local input_file="test_inputs/test${test_num}.txt"
    local expected_file="test_inputs/expected${test_num}.txt"
    
    if [ ! -f "$input_file" ]; then
        return
    fi
    
    echo "Running Test $test_num:"
    echo "=================="
    
    # Run the solution
    ./solution < "$input_file" > output.txt
    
    # Check if expected file exists
    if [ -f "$expected_file" ]; then
        # Compare output with expected (ignoring trailing whitespace)
        if diff -w -B output.txt "$expected_file" > /dev/null; then
            echo "✓ Test $test_num passed"
        else
            echo "✗ Test $test_num failed"
            echo "Expected:"
            cat "$expected_file"
            echo ""
            echo "Got:"
            cat output.txt
        fi
    else
        echo "Output:"
        cat output.txt
        echo "(No expected file to compare)"
    fi
    echo ""
}

# Run all tests
for i in 1 2 3 4 5; do
    run_test $i
done

# Clean up
rm -f output.txt solution