#!/bin/bash

# ICPC World Finals 2025 - Problem G: Lava Moat
# Test runner script

set -e

# Compile the solution
echo "Compiling solution..."
g++ -std=c++17 -O2 -Wall -Wextra solution.cpp -o solution

# Run tests
echo "Running tests..."

# Test 1: Sample input from problem statement
echo -n "Test 1 (Sample input): "
if timeout 4s ./solution < test_inputs/test1.txt > temp_output.txt 2>/dev/null; then
    if diff -w temp_output.txt test_inputs/expected1.txt >/dev/null 2>&1; then
        echo "✓ PASSED"
    else
        echo "✗ FAILED (output mismatch)"
        echo "Expected:"
        cat test_inputs/expected1.txt
        echo "Got:"
        cat temp_output.txt
    fi
else
    echo "✗ FAILED (timeout or runtime error)"
fi

# Test 2: Simple rectangular terrain
echo -n "Test 2 (Simple test): "
if timeout 4s ./solution < test_inputs/test2.txt > temp_output.txt 2>/dev/null; then
    if diff -w temp_output.txt test_inputs/expected2.txt >/dev/null 2>&1; then
        echo "✓ PASSED"
    else
        echo "✗ FAILED (output mismatch)"
        echo "Expected:"
        cat test_inputs/expected2.txt
        echo "Got:"
        cat temp_output.txt
    fi
else
    echo "✗ FAILED (timeout or runtime error)"
fi

# Test 3: Large test
echo -n "Test 3 (Large test): "
if timeout 4s ./solution < test_inputs/test3.txt > temp_output.txt 2>/dev/null; then
    if diff -w temp_output.txt test_inputs/expected3.txt >/dev/null 2>&1; then
        echo "✓ PASSED"
    else
        echo "✗ FAILED (output mismatch)"
        echo "Expected:"
        cat test_inputs/expected3.txt
        echo "Got:"
        cat temp_output.txt
    fi
else
    echo "✗ FAILED (timeout or runtime error)"
fi

# Test 4: Stress test
echo -n "Test 4 (Stress test): "
if timeout 4s ./solution < test_inputs/test4.txt > temp_output.txt 2>/dev/null; then
    if diff -w temp_output.txt test_inputs/expected4.txt >/dev/null 2>&1; then
        echo "✓ PASSED"
    else
        echo "✗ FAILED (output mismatch)"
        echo "Expected:"
        cat test_inputs/expected4.txt
        echo "Got:"
        cat temp_output.txt
    fi
else
    echo "✗ FAILED (timeout or runtime error)"
fi

# Timing test for performance verification
echo ""
echo "Performance test (timing on stress test):"
time ./solution < test_inputs/test4.txt >/dev/null 2>&1

# Clean up
rm -f temp_output.txt solution

echo ""
echo "All tests completed."