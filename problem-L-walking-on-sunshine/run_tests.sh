#!/bin/bash

# Compile the solution
echo "Compiling solution..."
g++ -O2 -std=c++17 -o solution solution.cpp

if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

echo "Compilation successful!"
echo ""
echo "Running test cases:"
echo "==================="

# Expected outputs
expected=(3.0000000000 7.0000000000 0.0000000000 0.0000000000 0.0000000000 0.0000000000)

# Run each test
for i in {1..6}; do
    echo -n "Test $i: "
    output=$(./solution < test_inputs/test$i.txt)
    echo -n "$output"
    
    # Compare with expected
    if [ "$output" = "${expected[$i-1]}" ]; then
        echo " ✓"
    else
        echo " ✗ (expected: ${expected[$i-1]})"
    fi
done

echo "==================="
echo "All tests complete!"