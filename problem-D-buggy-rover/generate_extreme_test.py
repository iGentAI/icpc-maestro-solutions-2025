grid_size = 200
# First line: dimensions
lines = ["200 200"]

# Next 200 lines: the grid - put S at center for maximum movement freedom
for i in range(grid_size):
    if i == 100:  # Put 'S' at row 100, column 100 (center)
        line = '.' * 100 + 'S' + '.' * 99
    else:
        line = '.' * grid_size
    lines.append(line)

# Last line: 10,000 moves with frequent direction changes to stress test the DP
moves = []
directions = ['N', 'E', 'S', 'W']
current_pos = [100, 100]  # Starting position

# Generate moves that stay within bounds
for i in range(10000):
    # Create a pattern that changes frequently
    if i % 100 < 25:
        move = 'E' if current_pos[1] < 180 else 'W'
    elif i % 100 < 50:
        move = 'S' if current_pos[0] < 180 else 'N'
    elif i % 100 < 75:
        move = 'W' if current_pos[1] > 20 else 'E'
    else:
        move = 'N' if current_pos[0] > 20 else 'S'
    
    # Update position
    if move == 'N':
        current_pos[0] -= 1
    elif move == 'S':
        current_pos[0] += 1
    elif move == 'E':
        current_pos[1] += 1
    elif move == 'W':
        current_pos[1] -= 1
    
    moves.append(move)

lines.append(''.join(moves))

# Write to file
with open('test_extreme.txt', 'w') as f:
    f.write('\n'.join(lines))
    
print(f"Generated test_extreme.txt with {len(lines)-2} grid lines and {len(moves)} moves")
print(f"Final position would be at approximately ({current_pos[0]}, {current_pos[1]})")