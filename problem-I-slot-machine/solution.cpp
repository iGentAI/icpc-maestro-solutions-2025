/*
 * ICPC World Championship - Problem I: Slot Machine
 * Interactive Problem Solution
 * 
 * Problem Summary:
 * - n wheels, each with n symbols (0 to n-1) in the same order
 * - Goal: Make all wheels show the same symbol (k=1)
 * - Only feedback: number of distinct symbols k currently visible
 * - Max 10,000 actions allowed
 * - Time limit: 2 seconds
 * 
 * Algorithm Strategy:
 * 1. Use wheel 1 as reference (never rotate it)
 * 2. For each wheel i (2 to n):
 *    - Scan all n positions by rotating +1 repeatedly
 *    - Track which position gives minimum k
 *    - Move to that optimal position with proper repositioning
 * 3. Repeat passes until k=1 or no improvement
 * 
 * Complexity: O(n^2) actions per pass, typically 2-3 passes needed
 * Worst case for n=50: ~7500 actions (well within 10,000 limit)
 */

#include <bits/stdc++.h>
using namespace std;

// Global action counter for debugging/monitoring
int total_actions = 0;
const int MAX_ACTIONS = 10000;

// Send a rotation command and receive the new k value
int rotate_and_query(int wheel, int delta) {
    cout << wheel << " " << delta << endl;
    cout.flush();
    
    int new_k;
    cin >> new_k;
    
    total_actions++;
    
    // Debug output to stderr (not visible to judge)
    cerr << "Action " << total_actions << ": Rotate wheel " << wheel 
         << " by " << delta << " -> k=" << new_k << endl;
    
    return new_k;
}

// Find the optimal position for a wheel by scanning all possibilities
pair<int, int> find_best_position(int wheel, int n, int current_k) {
    // Track the best position and its k value
    int best_k = current_k;
    int best_pos = 0;  // Position 0 is where we started
    int last_k = current_k;
    
    // Scan positions 1 to n-1 by rotating +1 each time
    for (int pos = 1; pos < n; pos++) {
        last_k = rotate_and_query(wheel, 1);
        
        if (last_k < best_k) {
            best_k = last_k;
            best_pos = pos;
        }
        
        // Early termination if we achieve the goal
        if (last_k == 1) {
            return {pos, last_k};
        }
    }
    
    // After scanning, we're at position n-1
    // Need to move to best_pos
    
    if (best_pos == n - 1) {
        // We're already at the best position (position n-1)
        // No need to move, return the current k value
        return {best_pos, last_k};
    }
    
    // Calculate how many +1 rotations needed to reach best_pos from position n-1
    // We need to go from n-1 to best_pos, wrapping around at n
    int steps_needed = (best_pos - (n - 1) + n) % n;
    
    if (steps_needed == 0) {
        // Already at best position (shouldn't happen as we handled best_pos == n-1)
        return {best_pos, last_k};
    }
    
    // Move to best position with multiple +1 rotations
    // (The problem allows arbitrary delta in one command, but using +1 for safety)
    int final_k = last_k;
    for (int i = 0; i < steps_needed; i++) {
        final_k = rotate_and_query(wheel, 1);
        if (final_k == 1) {
            // Early termination if we hit the goal during repositioning
            return {(n - 1 + i + 1) % n, final_k};
        }
    }
    
    return {best_pos, final_k};
}

// Alternative implementation using single large rotation (if allowed by protocol)
pair<int, int> find_best_position_optimized(int wheel, int n, int current_k) {
    // Track the best position and its k value
    int best_k = current_k;
    int best_pos = 0;  // Position 0 is where we started
    int last_k = current_k;
    
    // Scan positions 1 to n-1 by rotating +1 each time
    for (int pos = 1; pos < n; pos++) {
        last_k = rotate_and_query(wheel, 1);
        
        if (last_k < best_k) {
            best_k = last_k;
            best_pos = pos;
        }
        
        // Early termination if we achieve the goal
        if (last_k == 1) {
            return {pos, last_k};
        }
    }
    
    // After scanning, we're at position n-1
    
    if (best_pos == n - 1) {
        // Already at the best position
        return {best_pos, last_k};
    }
    
    // Calculate rotation needed from n-1 to best_pos
    // This uses a single rotation command with larger delta
    int delta = (best_pos + 1) % n;  // +1 because we need one more to complete the circle
    
    if (delta == 0) {
        // Position 0 is best, need to rotate by 1
        delta = 1;
    }
    
    int final_k = rotate_and_query(wheel, delta);
    return {best_pos, final_k};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    // Ensure unbuffered I/O for interactive communication
    setvbuf(stdin, nullptr, _IONBF, 0);
    setvbuf(stdout, nullptr, _IONBF, 0);
    
    int n, k;
    cin >> n >> k;
    
    cerr << "Starting: n=" << n << ", initial k=" << k << endl;
    
    // Already solved (shouldn't happen according to problem)
    if (k == 1) {
        cerr << "Already solved!" << endl;
        return 0;
    }
    
    int pass = 0;
    const int MAX_PASSES = 5;  // Usually 2-3 passes suffice
    
    while (k > 1 && pass < MAX_PASSES && total_actions < MAX_ACTIONS - 2*n) {
        pass++;
        cerr << "\n=== Pass " << pass << " ===" << endl;
        
        bool improved = false;
        int initial_k = k;
        
        // Optimize each wheel (except wheel 1 which is our reference)
        for (int wheel = 2; wheel <= n && k > 1; wheel++) {
            cerr << "Optimizing wheel " << wheel << "..." << endl;
            
            // Use the optimized version (single large rotation)
            // Comment out next line and uncomment the one after to use multiple +1 rotations
            auto [best_pos, new_k] = find_best_position_optimized(wheel, n, k);
            // auto [best_pos, new_k] = find_best_position(wheel, n, k);
            
            if (new_k < k) {
                improved = true;
                cerr << "Wheel " << wheel << " improved k from " << k 
                     << " to " << new_k << " at position " << best_pos << endl;
            }
            
            k = new_k;
            
            // Goal achieved
            if (k == 1) {
                cerr << "SUCCESS! Solved in " << total_actions << " actions" << endl;
                return 0;
            }
            
            // Check action limit
            if (total_actions >= MAX_ACTIONS - 2*n) {
                cerr << "Approaching action limit, stopping..." << endl;
                break;
            }
        }
        
        cerr << "Pass " << pass << " complete: k reduced from " 
             << initial_k << " to " << k << endl;
        
        // Stop if no improvement in this pass
        if (!improved) {
            cerr << "No improvement in pass " << pass << ", stopping" << endl;
            break;
        }
    }
    
    if (k == 1) {
        cerr << "SUCCESS! Solved in " << total_actions << " actions" << endl;
    } else {
        cerr << "Failed to solve: k=" << k << " after " << total_actions << " actions" << endl;
    }
    
    return 0;
}