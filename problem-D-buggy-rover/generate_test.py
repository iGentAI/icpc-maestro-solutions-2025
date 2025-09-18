grid_size = 200
# First line: dimensions
lines = ["200 200"]

# Next 200 lines: the grid
for i in range(grid_size):
    if i == 99:  # Put 'S' at row 100, column 0
        line = 'S' + '.' * (grid_size - 1)
    else:
        line = '.' * grid_size
    lines.append(line)

# Last line: move sequence (1000 moves alternating directions)
moves = 'E' * 99 + 'N' * 50 + 'E' * 99 + 'S' * 50 + 'W' * 99 + 'N' * 50 + 'W' * 99 + 'S' * 50
moves += 'E' * 50 + 'N' * 50 + 'S' * 50 + 'W' * 50  # Add more moves
lines.append(moves)

# Write to file
with open('test_large.txt', 'w') as f:
    f.write('\n'.join(lines))
    
print(f"Generated test_large.txt with {len(lines)-2} grid lines and {len(moves)} moves")