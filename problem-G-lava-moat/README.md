# Lava Moat Problem Solution (ICPC 2025 World Finals Baku - Problem G)

## Problem Summary

**Time Limit: 4 seconds**

The problem asks us to find the shortest path at a constant elevation (a level path) from the western border to the eastern border of a triangulated terrain. This represents building a lava moat that must be at a single elevation to prevent lava from flowing out.

### Key Concepts:
1. **Triangulated Terrain**: The terrain is represented as a fully triangulated rectangle with vertices having (x, y, z) coordinates where z is the elevation.
2. **Level Curves**: At any elevation z, the intersection of the terrain with the horizontal plane at height z forms a set of line segments.
3. **Shortest Path**: We need to find the minimum length path that stays at constant elevation and connects west to east borders.

## Algorithm Overview

### 1. Critical Elevations
- The topology of level curves changes only at vertex elevations
- We identify all unique z-values from vertices as critical elevations
- We also sample intermediate elevations between critical ones

### 2. Level Curve Extraction
For each test elevation z:
- Compute intersection of each triangle with the horizontal plane at z
- Each triangle contributes 0, 1, or 2 points to the level curve
- Connect intersection points within each triangle to form segments

### 3. Graph Construction
- Build a graph where nodes are intersection points (quantized for numerical stability)
- Edges are the level curve segments with weights equal to their 2D lengths
- Use quantized coordinates to handle floating-point precision issues

### 4. Shortest Path Computation
- Identify nodes on west border (x = 0) and east border (x = w)
- Run multi-source Dijkstra from all west nodes
- Find minimum distance to any east node

### 5. Global Optimization
- Test multiple elevations (critical + intermediate)
- Return the minimum path length across all valid elevations

## Implementation Details

### Input Format
```
t                    # number of test cases
w l n m              # width, length, #vertices, #triangles
x_i y_i z_i          # vertex coordinates (n lines)
a_i b_i c_i          # triangle vertices, 1-indexed (m lines)
```

### Key Optimizations
1. **Coordinate Quantization**: Map floating-point coordinates to integers to avoid precision issues when matching nodes across triangles
2. **Efficient Graph Representation**: Use maps with quantized coordinate pairs as keys
3. **Multi-source Dijkstra**: Start from all west nodes simultaneously

### Complexity Analysis
- O(n log n) for sorting critical elevations
- O(m × k) for processing triangles at k test elevations
- O((n + m) log n) for Dijkstra at each elevation
- Total: O(n² log n) worst case

## Files

- `src/lava_moat.cpp` - Initial complex implementation with advanced optimizations
- `src/lava_moat_fixed.cpp` - Simplified, corrected implementation
- `simple_test.txt` - Basic test case with rectangular terrain
- `sample_input1.txt` - Sample test cases from the problem statement

## Running the Solution

```bash
# Compile
g++ -std=c++17 -O2 -Wall src/lava_moat_fixed.cpp -o lava_moat

# Run with test input
./lava_moat < test_input.txt
```

## Sample Results

For a simple 10×10 rectangular terrain with 4 vertices and 2 triangles:
- Result: 14.1421356237 (diagonal distance across the square)

The solution correctly handles:
- Degenerate cases (horizontal edges, vertices at exact elevation)
- Multiple connected components at different elevations
- Numerical precision issues through coordinate quantization
- Both possible and impossible configurations