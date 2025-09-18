#!/bin/bash

# Compile the solution
echo "Compiling solution..."
g++ -O2 -std=c++17 solution.cpp -o solution
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# Run tests
test_dir="test_inputs"
passed=0
failed=0

for test_file in $test_dir/test*.txt; do
    test_name=$(basename "$test_file" .txt)
    expected_file="$test_dir/expected${test_name#test}.txt"
    
    if [ -f "$expected_file" ]; then
        echo -n "Running $test_name... "
        ./solution < "$test_file" > temp_output.txt
        
        # Use diff with whitespace flags to compare
        if diff -B -w "$expected_file" temp_output.txt > /dev/null 2>&1; then
            echo "PASSED"
            ((passed++))
        else
            echo "FAILED"
            echo "  Input: $(cat "$test_file")"
            echo "  Expected: $(cat "$expected_file")"
            echo "  Got: $(cat temp_output.txt)"
            ((failed++))
        fi
    fi
done

# Clean up
rm -f temp_output.txt solution

echo ""
echo "Results: $passed passed, $failed failed"

if [ $failed -eq 0 ]; then
    exit 0
else
    exit 1
fi