#!/bin/bash

# Compile the solution
echo "Compiling solution..."
g++ -O2 -std=c++17 -Wall -o solution solution.cpp
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# Compile stress test generator if it exists
if [ -f "stress_test_generator.cpp" ]; then
    echo "Compiling stress test generator..."
    g++ -O2 -std=c++17 -o stress_gen stress_test_generator.cpp
fi

# Run tests
echo "Running tests..."
passed=0
failed=0

for test_file in test_inputs/test*.txt; do
    if [ -f "$test_file" ]; then
        base_name=$(basename "$test_file" .txt)
        expected_file="test_inputs/expected${base_name#test}.txt"
        
        if [ -f "$expected_file" ]; then
            echo -n "Testing $base_name... "
            ./solution < "$test_file" > output.txt
            
            if diff -w -q output.txt "$expected_file" > /dev/null 2>&1; then
                echo "PASSED"
                ((passed++))
            else
                echo "FAILED"
                echo "  Expected:"
                cat "$expected_file" | head -5
                echo "  Got:"
                cat output.txt | head -5
                ((failed++))
            fi
        else
            echo "Warning: No expected output file for $test_file"
        fi
    fi
done

# Clean up
rm -f output.txt

echo ""
echo "Results: $passed passed, $failed failed"

# Run stress test if generator exists and all tests passed
if [ $failed -eq 0 ] && [ -f "stress_gen" ]; then
    echo ""
    echo "Running stress test (maximum constraints)..."
    ./stress_gen > /tmp/stress_input.txt
    time ./solution < /tmp/stress_input.txt > /tmp/stress_output.txt
    lines=$(wc -l < /tmp/stress_output.txt)
    echo "Stress test completed. Output lines: $lines"
    rm -f /tmp/stress_input.txt /tmp/stress_output.txt
fi

exit $failed