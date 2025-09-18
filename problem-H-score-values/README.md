# Problem H: Score Values

## Problem Summary
Determine the minimum number of digit signs (0-8) needed to display any achievable score in a game, where:
- Scores start at 0
- Can be incremented by fixed amounts (up to 10 different increments, each 1-1000)
- Scores are capped at maximum value m (up to 10^18)
- Digit 6 signs can be flipped upside-down to show 9

## Algorithm Approach
The solution uses BFS to find all reachable scores, then computes maximum digit frequencies:
1. Start with score 0
2. Use BFS to explore all reachable scores with increment and capping mechanics
3. For each reachable score, count the frequency of each digit
4. Track the maximum frequency needed for each digit across all scores
5. Handle the special case where 6 can represent 9 (count combined 6+9 occurrences)

## Time Complexity
- O(m × n) for BFS traversal, where m is the maximum score and n is the number of increments
- O(reachable_scores × log(m)) for digit counting

## Space Complexity
- O(reachable_scores) for storing the set of reachable scores

## Test Results
- Sample 1: Passed ✓
- Sample 2: Passed ✓

## Running Tests
```bash
./run_tests.sh
```