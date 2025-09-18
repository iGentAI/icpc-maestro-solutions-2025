# Problem L - Walking on Sunshine

## Problem Summary
Find the minimum distance you need to walk with the sun shining in your eyes to get from the contest location to the awards ceremony location. The sun shines from the south, and walking southward outside of shaded rectangular areas causes eye pain.

## Key Constraints
- Sun shines from south to north (direction vector (0, 1))
- Walking with any southward component (y < 0) causes eye pain outside shade
- Rectangular shaded areas allow walking in any direction without eye pain
- Up to 10^5 shaded rectangles
- Coordinates up to ±10^6
- Time limit: 2 seconds
- Answer precision: absolute/relative error
 ≤ 10^-7

## Solution Approach

### Key Insight
Since horizontal and northward movements are free (no cost), the optimal path can be arranged so that any southward motion occurs purely vertically. This reduces the problem to a 1D interval merging problem.

### Algorithm
1. If no southward movement is required (ys ≤ yt), return 0
2. Calculate the vertical range [min(ys,yt), max(ys,yt)]
3. For each rectangle, find its intersection with this vertical range
4. Sort and merge overlapping intervals
5. Return (total vertical drop) - (total shaded vertical distance)

### Time Complexity
- **O(n log n)** due to sorting intervals
- Efficient interval merging in O(n)

### Space Complexity
- **O(n)** for storing intervals

## Test Results
All 6 sample test cases pass with exact expected outputs:
- Test 1: 3.0000000000 ✓
- Test 2: 7.0000000000 ✓
- Test 3: 0.0000000000 ✓
- Test 4: 0.0000000000 ✓
- Test 5: 0.0000000000 ✓
- Test 6: 0.0000000000 ✓

## Implementation Notes
- Uses numerical tolerance (EPS = 1e-12) for floating-point comparisons
- Includes proper handling of edge cases
- Optimized for competitive programming time constraints