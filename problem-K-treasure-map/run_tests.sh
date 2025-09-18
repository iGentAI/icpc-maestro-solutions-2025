#!/bin/bash

# Compile the corrected solution
echo "Compiling treasure_map_fixed.cpp..."
g++ -std=c++17 -O2 -o treasure_map_fixed treasure_map_fixed.cpp
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

echo "Running tests..."
echo "==============="

# Test 1
echo -e "\nTest Case 1:"
echo "3 3 5 1 1
1 3 1
3 3 2
2 3 3
2 2 4
2 1 5" | ./treasure_map_fixed
echo "Expected: 3"

# Test 2
echo -e "\nTest Case 2:"
echo "3 5 4 3 4
2 4 1
2 2 2
1 1 4
3 1 5" | ./treasure_map_fixed
echo "Expected: 1"

# Test 3
echo -e "\nTest Case 3:"
echo "3 3 3 3 3
2 3 1
2 1 2
1 2 4" | ./treasure_map_fixed
echo "Expected: 0"

# Test 4
echo -e "\nTest Case 4:"
echo "3 3 4 3 2
2 1 2
2 3 3
1 3 4
1 1 5" | ./treasure_map_fixed
echo "Expected: impossible"

# Test 5
echo -e "\nTest Case 5:"
echo "3 3 3 2 2
3 2 0
2 2 1
2 3 0" | ./treasure_map_fixed
echo "Expected: impossible"

echo -e "\nAll tests completed!"