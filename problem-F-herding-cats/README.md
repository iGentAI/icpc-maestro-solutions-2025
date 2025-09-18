# Problem F - Herding Cats

## Problem Summary
Place m distinct catnip plants in m pots (numbered 1 to m) such that n cats each stop at their desired pot. Each cat likes certain plants and walks along pots 1 to m, stopping at the first pot containing a liked plant. Must determine if a valid arrangement exists.

## Key Constraints
- Test cases: t ≤ 10,000
- Cats per test: n ≤ 2×10⁵
- Plants/pots per test: m ≤ 2×10⁵
- Each cat likes k plants (1 ≤ k ≤ m)
- Sum of all n, m, and k values across tests are bounded
- Time limit: 2 seconds

## Solution Approach

### Key Insight
This is a **bipartite matching problem** with position constraints. The problem reduces to finding a perfect matching between pots and plants while respecting:
1. **Position constraints**: Each plant p cannot be placed before the rightmost target pot among cats that like p
2. **Intersection constraints**: For pots with multiple cats targeting them, only plants liked by ALL those cats can be placed there

### Algorithm
1. **Preprocessing**:
   - Compute `min_pos[p]` for each plant: maximum target pot among cats liking plant p
   - Group cats by target pot
   - Compute intersection of liked plants for each pot with cats

2. **Early Feasibility Check**:
   - Verify T_k ≥ k for all k, where T_k = count of plants with min_pos ≤ k
   - If this fails, no solution exists

3. **Bipartite Graph Construction**:
   - Left nodes: constrained pots (pots with at least one cat targeting them)
   - Right nodes: plants
   - Edge (pot k, plant p) exists if:
     - p is in the intersection of liked plants for cats targeting k
     - k ≥ min_pos[p]

4. **Matching**:
   - Use Kuhn's algorithm (DFS-based) to find maximum matching
   - Solution exists if all constrained pots can be matched

### Time Complexity
- **Preprocessing**: O(sum |S_i|) where S_i is the liked set for cat i
- **Graph construction**: O(sum_k |Inter_k|) ≤ O(sum |S_i|)
- **Matching (Kuhn's)**: O(L × E) where L is constrained pots, E is edges
- **Overall**: O(n×m) in worst case, but typically much better due to sparse graphs

### Space Complexity
- **O(m + sum |S_i| + E)** for storing constraints, liked sets, and graph edges

## Test Results
Solution correctly handles the sample test cases:
- Test 1: `yes` - Valid arrangement exists
- Test 2: `no` - No valid arrangement possible

## Implementation Notes
- **Optimizations**:
  - Only build graph for constrained pots (avoids dense graphs)
  - Use counting approach for intersection computation
  - Deduplicate liked plants per cat to maintain set semantics
  - Early termination via feasibility check
- **Algorithmic choices**:
  - Kuhn's algorithm chosen for simplicity and efficiency on sparse graphs
  - Could use Hopcroft-Karp for denser instances if needed
- **Edge case handling**:
  - Handles cats with duplicate liked plants
  - Correctly processes unconstrained pots (no cats targeting them)
  - Validates input ranges

## Critical Insights
1. **Not a greedy problem**: Simple greedy approaches fail due to interdependent constraints
2. **Sparse graph property**: Most real test cases produce sparse bipartite graphs, making DFS-based matching efficient
3. **Feasibility check importance**: The T_k check eliminates many impossible cases early, avoiding expensive matching attempts