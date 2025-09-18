#!/bin/bash

echo "Running tests for Problem B: Blackboard Game"
echo "=========================================="

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Test counter
total=0
passed=0

# Run tests
for test_file in test_inputs/test*.txt; do
    if [ -f "$test_file" ]; then
        test_name=$(basename "$test_file" .txt)
        expected_file="test_inputs/expected${test_name#test}.txt"
        
        if [ -f "$expected_file" ]; then
            total=$((total + 1))
            echo -n "Running $test_name... "
            
            # Run the solution
            output=$(python3 solution.py < "$test_file" 2>/dev/null)
            expected=$(cat "$expected_file")
            
            if [ "$output" = "$expected" ]; then
                echo -e "${GREEN}PASSED${NC}"
                passed=$((passed + 1))
            else
                echo -e "${RED}FAILED${NC}"
                echo "  Expected: $expected"
                echo "  Got: $output"
            fi
        fi
    fi
done

echo "=========================================="
echo "Results: $passed/$total tests passed"

if [ $passed -eq $total ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed.${NC}"
    exit 1
fi