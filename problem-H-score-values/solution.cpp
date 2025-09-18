/*
 * Problem H: Score Values
 * 
 * This solution uses BFS to find all reachable scores with the capping mechanics,
 * then computes the maximum digit frequencies across all reachable scores.
 * 
 * Key insight: Scores start at 0 and can be incremented by fixed amounts,
 * but are capped at the maximum value m if an increment would exceed it.
 */

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Read input
    long long m;  // Maximum score
    int n;        // Number of increment types
    cin >> m >> n;
    
    vector<int> increments(n);
    for (int i = 0; i < n; i++) {
        cin >> increments[i];
    }

    // Find all reachable scores using BFS
    // Game mechanics: start at 0, can add any increment, cap at m
    set<long long> reachable;
    queue<long long> q;
    q.push(0);
    reachable.insert(0);
    
    while (!q.empty()) {
        long long current = q.front();
        q.pop();
        
        for (int p : increments) {
            // Calculate next score with capping at maximum
            long long next = min(current + (long long)p, m);
            if (reachable.find(next) == reachable.end()) {
                reachable.insert(next);
                q.push(next);
            }
        }
    }
    
    // Find the maximum count needed for each digit
    // Important: digit 6 can be flipped upside-down to show 9
    vector<int> signs_needed(9, 0);
    
    for (long long score : reachable) {
        string s = to_string(score);
        vector<int> digit_count(10, 0);
        
        // Count each digit in this score
        for (char c : s) {
            digit_count[c - '0']++;
        }
        
        // Update maximum for digits 0-5, 7-8
        for (int d = 0; d <= 8; d++) {
            if (d != 6) {
                signs_needed[d] = max(signs_needed[d], digit_count[d]);
            }
        }
        
        // For digit 6: need enough to cover both 6s and 9s
        // (since a 6 sign can be flipped to show 9)
        signs_needed[6] = max(signs_needed[6], digit_count[6] + digit_count[9]);
    }
    
    // Output the results
    // Format: digit count (one pair per line)
    // Only output digits with non-zero counts
    for (int d = 0; d <= 8; d++) {
        if (signs_needed[d] > 0) {
            cout << d << ' ' << signs_needed[d] << '\n';
        }
    }
    
    return 0;
}