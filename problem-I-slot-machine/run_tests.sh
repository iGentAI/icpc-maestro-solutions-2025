#!/bin/bash
set -euo pipefail

# Compile solution if not already compiled
if [ ! -f solution ]; then
    echo "Compiling solution..."
    g++ -O2 -std=c++17 -Wall solution.cpp -o solution
fi

# Compile test judge if not already compiled
if [ ! -f test_judge ]; then
    echo "Compiling test judge..."
    g++ -O2 test_judge.cpp -o test_judge
fi

# Function to run a single test case
run_case() {
    local n="$1"
    local initial="$2"
    local desc="$3"
    
    echo "=== $desc ==="
    
    # Create temp directory for FIFOs
    local tmp
    tmp="$(mktemp -d)"
    local j2s="$tmp/j2s"  # judge to solution
    local s2j="$tmp/s2j"  # solution to judge
    
    # Create named pipes
    mkfifo "$j2s" "$s2j"
    
    # Start solution in background reading from j2s, writing to s2j
    ./solution <"$j2s" >"$s2j" 2>"$tmp/sol.err" &
    local sol_pid=$!
    
    # Run judge reading from s2j, writing to j2s
    ./test_judge "$n" "$initial" <"$s2j" >"$j2s" 2>"$tmp/judge.log" || true
    
    # Wait for solution to finish
    wait "$sol_pid" 2>/dev/null || true
    
    # Display results
    echo "--- Judge output ---"
    grep "Judge:" "$tmp/judge.log" | tail -1 || true
    
    # Clean up
    rm -rf "$tmp"
    echo
}

echo "Running interactive tests..."
echo

# Test cases from the problem samples
run_case 3 012 "Sample Test 1: n=3, initial=012 (3 distinct)"
run_case 5 01234 "Sample Test 2: n=5, initial=01234 (5 distinct)"

# Additional test cases
run_case 4 0123 "Test 3: n=4, all different"
run_case 5 00111 "Test 4: n=5, partial alignment"
run_case 10 0123456789 "Test 5: n=10, worst case"

echo "All tests completed."