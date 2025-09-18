#!/bin/bash

# Compile the solution
echo "Compiling solution..."
g++ -std=c++17 -O2 -Wall -o buggy_rover solution.cpp

if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

echo "Compilation successful!"
echo ""

# Define test cases and expected outputs
declare -a tests=("test1" "test2" "test3")
declare -a expected=("1" "0" "4")

# Run sample tests with expected outputs
echo "Running sample tests..."
echo "========================"

for i in "${!tests[@]}"; do
    test_file="test_inputs/${tests[$i]}.txt"
    expected_output="${expected[$i]}"
    
    if [ ! -f "$test_file" ]; then
        echo "Test file $test_file not found!"
        continue
    fi
    
    echo "Test ${tests[$i]}:"
    output=$(./buggy_rover < $test_file)
    echo "  Expected: $expected_output"
    echo "  Got: $output"
    
    if [ "$output" == "$expected_output" ]; then
        echo "  ✓ PASS"
    else
        echo "  ✗ FAIL"
    fi
    echo ""
done

# Run performance tests
echo "Running performance tests..."
echo "============================"

# Large test
if [ -f "test_inputs/test_large.txt" ]; then
    echo "Large test (200x200 grid, ~800 moves):"
    time ./buggy_rover < test_inputs/test_large.txt
    echo ""
fi

# Extreme test
if [ -f "test_inputs/test_extreme.txt" ]; then
    echo "Extreme test (200x200 grid, 10000 moves):"
    time ./buggy_rover < test_inputs/test_extreme.txt
    echo ""
fi

echo "All tests completed!"