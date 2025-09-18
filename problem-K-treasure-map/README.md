# Problem K - Treasure Map

## Problem Summary
Find the minimum possible depth at a treasure location on Captain Blackbeard's hypsometric map. The map has a special property: for each unit square, two different triangulation methods for bilinear interpolation yield the same result. Given partial depth data and treasure coordinates, determine the minimum treasure depth or if no valid map exists.

## Key Constraints
- Grid size: 2 ≤ n, m ≤ 3×10^5
- Known depths: 1 ≤ k ≤ 3×10^5  
- Depths: 0 ≤ d ≤ 10^9
- All depths must be non-negative
- Both triangulation methods must yield identical results for every unit square
- Time limit: 4 seconds

## Solution Approach

### Key Insight
The constraint that both triangulation methods yield the same result implies that depths must follow the form z_{i,j} = a_i + b_j, effectively decomposing the 2D grid into independent row and column components.

### Algorithm
1. Model the problem as a bipartite graph between rows and columns
2. Each known depth creates an edge with constraint a_i + b_j = depth
3. Use BFS/DFS to find connected components and assign relative values
4. Check consistency of constraints and non-negativity requirements
5. Compute minimum treasure depth using component-wise optimization

### Time Complexity
- **O(k + n + m)** for graph construction and BFS/DFS
- Efficient for large grids with sparse known depths

### Space Complexity
- **O(k + n + m)** for graph representation

## Test Results
All 5 sample test cases pass:
- Test 1: 3 ✓
- Test 2: 1 ✓
- Test 3: 0 ✓
- Test 4: impossible ✓
- Test 5: impossible ✓

## Implementation Notes
- Uses graph-based approach with connected components
- Handles edge cases: negative depths, conflicting constraints, no known depths
- Ensures numerical stability with long long for all depth calculations
- Efficiently detects impossible configurations