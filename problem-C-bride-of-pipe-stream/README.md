# Problem C: Bride of Pipe Stream

## Problem Summary

The Bride of Pipe Stream problem from the 49th ICPC World Championship involves optimizing Flubber flow through a network of stations and ducts to maximize the minimum flow percentage that reaches all reservoirs.

Key aspects:
- Flow starts at station 1 with 100% of factory output
- Stations have controllable flow splits to outgoing ducts
- Ducts have fixed percentage splits to downstream stations/reservoirs
- Percentages sum to "at most 100" (flow can be lost)
- Objective: Maximize the minimum flow percentage to all reservoirs

### Constraints
- Stations: 1 ≤ s ≤ 10,000
- Reservoirs: 1 ≤ r ≤ 3
- Ducts: s ≤ d ≤ 20,000
- Time limit: 12 seconds

## Solution Approach

### Key Insights

1. **Flow Loss**: Duct percentages don't necessarily sum to 100%, allowing flow to be lost in the system

2. **Two-Level Control**: Stations have controllable splits (optimization variables), ducts have fixed splits

3. **Max-Min Optimization**: The problem is to maximize the minimum flow to all reservoirs

4. **Convex Optimization**: For small r (≤ 3), the problem can be solved efficiently using convex minimization

### Algorithm

The solution uses **Dynamic Programming with Convex Optimization**:

1. **State Representation**: `evaluatePhiAtZ(z)` computes the maximum achievable weighted sum z·f of reservoir flows

2. **Network Traversal**: Uses topological ordering on the DAG of reachable stations

3. **Optimization Strategy**:
   - **R = 1**: Trivial case, maximize flow to single reservoir
   - **R = 2**: 1D ternary search over z = {a, 1-a} for a ∈ [0,1]
   - **R = 3**: Nested ternary search over the 2D simplex

4. **Complexity**:
   - Time: O(D × S × iterations) where D = ducts, S = stations
   - Space: O(S + D + R)

## Implementation Details

- **Fast I/O**: Uses `ios::sync_with_stdio(false)` for performance
- **Precision Handling**: Uses double with EPS = 1e-12 for comparisons
- **Flow Conservation**: Properly handles flow loss when percentages < 100
- **DAG Processing**: Topological sort for correct flow propagation
- **Optimizations**:
  - Precomputes reachable stations
  - Uses efficient data structures (vector of pairs)
  - 80 iterations for ternary search (ensures 1e-9 precision)

## Files

- `README.md`: This documentation file
- `solution.cpp`: Main solution implementation
- `problem.pdf`: Original problem statement from ICPC
- `test_inputs/`: Directory containing test cases
  - `test1.txt`: Sample input 1 from problem statement
  - `test2.txt`: Sample input 2 from problem statement
  - `expected1.txt`: Expected output for test 1
  - `expected2.txt`: Expected output for test 2
- `run_tests.sh`: Test runner script

## Building and Running

```bash
# Compile the solution
g++ -std=c++17 -O2 -Wall -o flubber_flow solution.cpp

# Run with sample input
./flubber_flow < test_inputs/test1.txt

# Run all tests
./run_tests.sh
```

## Test Results

| Test | Expected | Output | Result |
|------|----------|--------|--------|
| test1 | 24.000000 | 24.000000 | ✓ |
| test2 | 42.857143 | 42.857143 | ✓ |

## Critical Implementation Notes

The key to solving this problem correctly was understanding that:
1. Percentages in ducts don't normalize - they sum to "at most 100"
2. Flow can be lost when percentages sum to less than 100
3. The R=2 case requires proper convex minimization, not assuming complementary flows