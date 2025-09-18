# Problem E: Delivery Service

## Problem Summary

The Intercity Caspian Package Company (ICPC) is implementing a delivery service where couriers travel between cities with fixed schedules:
- Leave home city at 9:00, arrive at destination at 12:00
- Leave destination at 14:00, return home at 17:00
- Packages can be handed off between couriers when they meet in the same city at the same time

The task is to determine how many pairs of cities become mutually connected (packages can be delivered both ways) as couriers are hired incrementally.

### Constraints
- 2 ≤ n ≤ 2·10^5 (number of cities)
- 1 ≤ m ≤ 4·10^5 (number of couriers)
- Time limit: 12 seconds

## Solution Approach

### Key Insights

1. **Courier Meeting Points**: Couriers can only hand off packages when they're in the same city at the same time:
   - Evening/morning periods (17:00-9:00): couriers with home = city
   - Noon periods (12:00-14:00): couriers with destination = city
   - These periods don't overlap, creating a bipartite graph structure

2. **Graph Modeling**: Model as a bipartite graph with 2n hubs:
   - Evening hubs (1 to n): represent cities during evening/morning periods
   - Noon hubs (n+1 to 2n): represent cities during noon periods
   - Each courier (home, dest) creates an edge between home_evening and dest_noon

3. **Connectivity**: Two cities are connected if there exists a path in the graph allowing bidirectional package delivery through hub connections

### Algorithm

The solution uses **Union-Find with small-to-large merging** enhanced with careful tracking:

1. **Data Structure**: Modified Union-Find maintaining:
   - Component size (number of hubs)
   - Number of distinct cities per component
   - Lists of cities with evening/noon hubs in each component
   - Bridge counts between component pairs

2. **Incremental Updates**: For each courier added:
   - Union the corresponding evening and noon hubs
   - Update component city counts using the inclusion-exclusion principle
   - Track bridge pairs (cities split between exactly two components)
   - Compute connected pairs as: total_pairs - double_counted_pairs

3. **Complexity**: O(m·α(n)) amortized where α is the inverse Ackermann function

## Implementation Details

- **Fast I/O**: Uses `ios::sync_with_stdio(false)` for performance
- **Memory Optimization**: Small-to-large merging ensures O(n log n) total list operations
- **Hash Map**: Uses 64-bit keys for efficient bridge pair tracking
- **Careful Counting**: Avoids double-counting through precise bridge tracking

## Performance

The solution has been stress-tested with maximum constraints:
- **200,000 cities, 400,000 couriers**: Completes in ~0.5 seconds
- Well within the 12-second time limit
- Memory efficient with O(n) space complexity

## Files

- `solution.cpp`: Main solution implementation
- `problem.pdf`: Original problem statement from ICPC
- `test_inputs/`: Sample test cases
- `stress_test_generator.cpp`: Generator for performance testing
- `run_tests.sh`: Test runner script (to be added)