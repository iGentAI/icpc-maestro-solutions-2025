# Problem J - Stacking Cups

## Problem Description

You have a collection of n cylindrical cups, where the i-th cup is (2i - 1) cm tall. The cups have increasing diameters, such that cup i fits inside cup j if and only if i < j. The base of each cup is 1 cm thick.

After washing all the cups, you stack them in a tower with centers aligned vertically. The height of the tower is defined as the vertical distance from the lowest point on any of the cups to the highest.

Your task is to find an ordering of the cups such that the final height equals your favorite number h, or determine if it's impossible.

## Solution Approach

The solution uses a mathematical model where cups are arranged into maximal strictly decreasing segments (groups). Each group's height contribution equals the maximum cup index in the group plus (group_size - 1).

### Key Insights:
1. **Height Formula**: For a group with max cup index m and size k: height = m + k - 1
2. **Minimum Height**: 2n - 1 (all cups nested in one group)
3. **Maximum Height**: n(n+1)/2 (all cups as singleton groups)

### Algorithm:
1. Check if h is within feasible bounds [min_height, max_height]
2. Calculate extra height E = h - min_height
3. Represent E as a sum of distinct integers from {1, ..., n-2} using a greedy approach
4. Construct the permutation using a linked-list based manipulation
5. Output cup heights (2*i - 1) in the resulting order

## Complexity

- Time Complexity: O(n)
- Space Complexity: O(n)

## Test Cases

### Sample Input 1
```
4 9
```

### Sample Output 1
```
7 3 5 1
```

### Sample Input 2
```
4 100
```

### Sample Output 2
```
impossible
```