#!/bin/bash

# Compile the solution
echo "Compiling solution.cpp..."
g++ -std=c++17 -O2 -Wall -o flubber_flow solution.cpp
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# Run tests
echo "Running tests..."
echo ""

# Test 1
echo "Test 1:"
./flubber_flow < test_inputs/test1.txt > test_output1.txt
if diff -q test_output1.txt test_inputs/expected1.txt > /dev/null; then
    echo "  ✓ PASS"
else
    echo "  ✗ FAIL"
    echo "  Expected: $(cat test_inputs/expected1.txt)"
    echo "  Got: $(cat test_output1.txt)"
fi

# Test 2
echo "Test 2:"
./flubber_flow < test_inputs/test2.txt > test_output2.txt
if diff -q test_output2.txt test_inputs/expected2.txt > /dev/null; then
    echo "  ✓ PASS"
else
    echo "  ✗ FAIL"
    echo "  Expected: $(cat test_inputs/expected2.txt)"
    echo "  Got: $(cat test_output2.txt)"
fi

# Clean up
rm -f test_output1.txt test_output2.txt flubber_flow

echo ""
echo "Tests complete."