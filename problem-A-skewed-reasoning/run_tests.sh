#!/bin/bash

# Test runner for Problem A: A-Skew-ed Reasoning

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo "Running tests for Problem A: A-Skew-ed Reasoning"
echo "================================================"

# Test directory
TEST_DIR="test_inputs"
SOLUTION="solution.py"

# Check if solution file exists
if [ ! -f "$SOLUTION" ]; then
    echo -e "${RED}Error: Solution file $SOLUTION not found${NC}"
    exit 1
fi

# Check if test directory exists
if [ ! -d "$TEST_DIR" ]; then
    echo -e "${RED}Error: Test directory $TEST_DIR not found${NC}"
    exit 1
fi

# Run tests
total_tests=0
passed_tests=0

for test_file in "$TEST_DIR"/test*.txt; do
    if [ -f "$test_file" ]; then
        test_name=$(basename "$test_file" .txt)
        expected_file="$TEST_DIR/expected${test_name#test}.txt"
        
        if [ ! -f "$expected_file" ]; then
            echo -e "${RED}Warning: Expected output file $expected_file not found${NC}"
            continue
        fi
        
        echo -n "Test $test_name: "
        
        # Run the solution and capture output
        output=$(python3 "$SOLUTION" < "$test_file" 2>&1)
        expected=$(cat "$expected_file")
        
        total_tests=$((total_tests + 1))
        
        if [ "$output" == "$expected" ]; then
            echo -e "${GREEN}PASSED${NC}"
            passed_tests=$((passed_tests + 1))
        else
            echo -e "${RED}FAILED${NC}"
            echo "  Input:"
            head -5 "$test_file"
            if [ $(wc -l < "$test_file") -gt 5 ]; then
                echo "  ..."
            fi
            echo "  Expected:"
            echo "  $expected"
            echo "  Got:"
            echo "  $output"
            echo ""
        fi
    fi
done

echo "================================================"
echo "Results: $passed_tests/$total_tests tests passed"

if [ $passed_tests -eq $total_tests ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed${NC}"
    exit 1
fi