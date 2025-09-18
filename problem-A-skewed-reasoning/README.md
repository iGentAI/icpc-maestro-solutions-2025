# Skew Heap Problem Solution - ICPC World Championship Problem A

## Problem Summary

Given a binary tree with nodes labeled 1 to n, determine if this tree could have been produced by inserting some permutation of values 1 to n using a specific skew heap insertion algorithm. If yes, find the lexicographically minimal and maximal such permutations.

## Skew Heap Insertion Algorithm

The insertion algorithm for value `x` into skew heap `H` with root value `y`:
1. If `H` is empty: Create a single node containing `x`
2. If `y < x`: Swap the two children of the root and recursively insert `x` into the new left subtree
3. If `y >= x`: Create a new node with value `x` and make `H` the left subtree of this new node

## Solution Approach

### Key Insights

1. **Heap Property**: The tree must satisfy the min-heap property (parent ≤ children)
2. **Structural Constraint**: No node can have a right child without a left child (due to the insertion algorithm's swap mechanism)
3. **Reverse Engineering**: We work backwards from the final tree to determine possible insertion sequences

### Algorithm Overview

1. **Validation Phase**:
   - Check if the tree satisfies the heap property
   - Verify structural constraints (no right child without left child)
   - Ensure the tree is properly connected with a single root

2. **Undo Operation**:
   Two cases for undoing the last insertion:
   - **Case 1**: Root was inserted last (if right subtree is empty and root < left subtree root)
   - **Case 2**: Value was inserted into left subtree after swapping (recursively check left subtree)

3. **Finding Extremal Permutations**:
   - For lexicographically minimal insertion: Find the lexicographically maximal removal sequence, then reverse it
   - For lexicographically maximal insertion: Find the lexicographically minimal removal sequence, then reverse it
   - Use greedy selection with producibility checks and memoization for efficiency

## Implementation Details

The solution (`main.py`) implements:
- Tree parsing and validation
- Recursive undo operations with memoization
- Greedy algorithms for finding extremal permutations
- Efficient caching using tree structure digests

## Usage

```bash
python3 main.py < input.txt
```

### Input Format
- First line: integer `n` (number of nodes)
- Next `n` lines: for each node `i`, two integers representing left and right child indices (0 = no child)

### Output Format
- If possible: Two lines with lexicographically minimal and maximal permutations
- If impossible: Single line with "impossible"

## Test Cases

### Test 1: Valid tree with multiple permutations
```
Input:
7
2 3
4 5
6 7
0 0
0 0
0 0
0 0

Output:
1 3 2 7 5 6 4
7 1 5 3 2 6 4
```

### Test 2: Impossible tree (structural violation)
```
Input:
2
0 2
0 0

Output:
impossible
```

### Test 3: Valid tree with specific permutations
```
Input:
3
2 0
3 0
0 0

Output:
2 3 1
3 2 1
```

## Complexity Analysis

- **Time Complexity**: O(n²) worst case with memoization (often better in practice)
- **Space Complexity**: O(n) for tree representation and memoization cache
- Handles inputs up to n = 200,000 efficiently

## Key Edge Cases

1. Single node tree: Always valid with permutation `[value]`
2. Tree with structural violations (right child without left): Impossible
3. Tree violating heap property: Impossible
4. Disconnected or cyclic structures: Impossible

## Author

This solution was developed for the 49th ICPC World Championship Problem A: A-Skew-ed Reasoning.