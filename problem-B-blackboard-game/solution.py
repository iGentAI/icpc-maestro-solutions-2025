#!/usr/bin/env python3
"""
Blackboard Game - Competition Submission Version
Handles n up to 10^7 in O(1) time for large n
"""

def solve(n):
    """Main solver combining exact and pattern approaches"""
    if n <= 1:
        return "second", None
    
    # Use exact computation for small n
    if n <= 22:
        # Generate primes
        is_prime = [True] * (n + 1)
        is_prime[0] = is_prime[1] = False
        primes = []
        
        for i in range(2, int(n**0.5) + 1):
            if is_prime[i]:
                for j in range(i * i, n + 1, i):
                    is_prime[j] = False
        
        primes = [i for i in range(2, n + 1) if is_prime[i]]
        
        # Build neighbors function
        def get_neighbors(num):
            if num < 1 or num > n:
                return []
            neighbors = set()
            
            # Division by prime factors
            temp = num
            for p in primes:
                if p * p > temp:
                    break
                if temp % p == 0:
                    neighbors.add(num // p)
                    while temp % p == 0:
                        temp //= p
            
            if temp > 1:  # Large prime factor
                neighbors.add(num // temp)
            
            # Multiplication by primes
            for p in primes:
                result = num * p
                if result > n:
                    break
                neighbors.add(result)
            
            return sorted([x for x in neighbors if 1 <= x <= n])
        
        # Game tree with memoization
        memo = {}
        
        def is_losing(current, visited_mask):
            state = (current, visited_mask)
            if state in memo:
                return memo[state]
            
            found_winning = False
            for neighbor in get_neighbors(current):
                if visited_mask & (1 << neighbor):
                    continue
                
                new_mask = visited_mask | (1 << current) | (1 << neighbor)
                if is_losing(neighbor, new_mask):
                    found_winning = True
                    break
            
            memo[state] = not found_winning
            return not found_winning
        
        # Check even starts
        for start in range(2, n + 1, 2):
            if is_losing(start, 1 << start):
                return "first", start
        
        return "second", None
    
    # Pattern-based solution for n > 22
    k = n.bit_length() - 1
    power_of_2 = 1 << k
    r = n - power_of_2
    
    # Check winning pattern
    is_first_win = False
    
    if n == 3:
        return "first", 2
    
    if k == 3:  # 8 <= n < 16
        if r in {0, 4, 5, 6}:
            is_first_win = True
    elif k == 4:  # 16 <= n < 32
        if r in {0, 1, 4, 8, 9, 10, 11, 14}:
            is_first_win = True
    elif k >= 5:
        if r in {0, 1} or r == (1 << k) - 2:
            is_first_win = True
        elif (1 << (k - 2)) <= r <= (1 << (k - 1)) - 4:
            is_first_win = True
    
    if is_first_win:
        # Compute optimal first move
        A_k = power_of_2 >> 1  # 2^(k-1)
        
        if r == 0:
            s = A_k if k >= 4 else 2
        elif r == 1:
            s = A_k - 2 if k >= 4 else 4
        elif r == (1 << k) - 2:
            s = (1 << k) - 2
        elif (1 << (k - 2)) <= r <= (1 << (k - 1)) - 4:
            s = A_k + 2
        elif k == 4 and 8 <= r <= 11:
            s = A_k + 2
        else:
            s = 4 if k >= 4 else 2
        
        return "first", min(s, n) if s % 2 == 0 else 2
    
    return "second", None


def main():
    """Process input and output results"""
    import sys
    
    data = sys.stdin.read().strip().split()
    if not data:
        return
    
    t = int(data[0])
    idx = 1
    
    for _ in range(t):
        if idx < len(data):
            n = int(data[idx])
            idx += 1
            
            winner, first_move = solve(n)
            
            if winner == "first":
                print(f"first {first_move}")
            else:
                print("second")


if __name__ == "__main__":
    main()