# Problem B: Blackboard Game

## Problem Summary
Two players play a game on numbers 1 to n. The first player chooses an even starting number, then players alternate choosing numbers that are either the current number multiplied or divided by a prime. The player who cannot make a valid move loses.

## Solution Approach
- **Small n (≤ 22)**: Exact game tree analysis with memoization
- **Large n (> 22)**: Pattern-based O(1) solution using remainder classification

The solution discovers a mathematical pattern based on binary decomposition of n and provides constructive first moves for winning positions.

## Complexity
- Time: O(1) for n > 22, O(n × 2^n) for n ≤ 22
- Space: O(1) for n > 22, O(2^n) for n ≤ 22

## Test Results
- Sample 1 (n=5): second ✓
- Sample 2 (n=12): first 2 ✓
- Sample 3 (n=17): first 4 ✓
- Large values up to 10^7: Computed instantly ✓