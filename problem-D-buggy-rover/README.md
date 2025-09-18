# Problem D: Buggy Rover

## Problem Summary

The Buggy Rover problem from the 49th ICPC World Championship involves a rover exploring a grid-based planet using an internal direction ordering algorithm. The rover maintains a permutation of four directions (North, East, South, West) and for each move, selects the first valid direction from its ordering.

Key aspects:
- Grid can contain rocks (#), flat spaces (.), and a starting position (S)
- Rover uses the first valid direction from its internal ordering (permutation)
- Cosmic rays can change the rover's direction ordering between moves
- Given a sequence of moves, find the minimum number of ordering changes required

### Constraints
- Grid dimensions: 1 ≤ r, c ≤ 200
- Move sequence: 1-10,000 characters
- Time limit: 2 seconds

## Solution Approach

### Key Insights

1. **Finite State Space**: With only 4 directions, there are exactly 24 possible permutations

2. **Deterministic Movement**: For any position and permutation, the next move is deterministic based on valid directions

3. **Dynamic Programming**: The problem reduces to finding the minimum path through permutation transitions

### Algorithm

The solution uses **Dynamic Programming over permutations**:

1. **State Representation**: `dp[move_index][permutation]` = minimum changes to reach this state

2. **Precomputation**: For each position and move, determine which permutations could produce that move

3. **DP Transitions**:
   - Stay with same permutation (cost = 0)
   - Switch to different permutation (cost = 1)
   - Only consider valid permutations for each move

4. **Complexity**:
   - Time: O(L × 24) where L is the move sequence length
   - Space: O(24) using rolling array optimization

## Implementation Details

- **Fast I/O**: Uses `ios::sync_with_stdio(false)` for performance
- **Bit Masking**: 24-bit masks efficiently track valid permutations
- **Defensive Coding**: Validates all moves and handles edge cases
- **Optimizations**:
  - Precomputes legal moves for each position
  - Uses array instead of vector for permutations
  - Rolling DP array to save memory

## Performance

Tested with various input sizes:
- **Sample cases**: All pass correctly with expected outputs
- **Large test** (200×200 grid, 796 moves): 0.002 seconds
- **Extreme test** (200×200 grid, 10,000 moves): 0.003 seconds

Well within the 2-second time limit with excellent performance characteristics.

## Files

- `solution.cpp`: Main solution implementation
- `problem.pdf`: Original problem statement from ICPC
- `test_inputs/`: Directory containing test cases
  - `test1.txt`, `test2.txt`, `test3.txt`: Sample inputs from problem
  - `test_large.txt`: Large grid stress test
  - `test_extreme.txt`: Maximum constraint test (10,000 moves)
- `generate_test.py`: Script to generate large test cases
- `generate_extreme_test.py`: Script to generate extreme test cases
- `run_tests.sh`: Test runner script

## Building and Running

```bash
# Compile the solution
g++ -std=c++17 -O2 -Wall -o buggy_rover solution.cpp

# Run with sample input
./buggy_rover < test_inputs/test1.txt

# Run all tests
./run_tests.sh
```

## Test Results

| Test | Expected | Output | Time |
|------|----------|--------|------|
| test1 | 1 | 1 | < 0.01s |
| test2 | 0 | 0 | < 0.01s |
| test3 | 4 | 4 | < 0.01s |
| test_large | - | 10 | 0.002s |
| test_extreme | - | 399 | 0.003s |