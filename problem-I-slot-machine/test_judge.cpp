#include <bits/stdc++.h>
using namespace std;

// Simple judge for testing the slot machine solution
// Usage: run via test script with bidirectional pipes

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <n> <initial_config>" << endl;
        cerr << "Example: " << argv[0] << " 5 01234" << endl;
        cerr << "  where initial_config is n digits representing wheel positions" << endl;
        return 1;
    }
    
    int n = atoi(argv[1]);
    string initial = argv[2];
    
    if ((int)initial.length() != n) {
        cerr << "Error: initial config must have exactly n characters" << endl;
        return 1;
    }
    
    // Current configuration of wheels (position 0 to n-1 on each wheel)
    vector<int> wheels(n);
    for (int i = 0; i < n; i++) {
        if (initial[i] >= '0' && initial[i] <= '9') {
            wheels[i] = initial[i] - '0';
        } else {
            cerr << "Error: initial config must be digits 0-9" << endl;
            return 1;
        }
        if (wheels[i] >= n) {
            cerr << "Error: wheel position " << wheels[i] << " >= n=" << n << endl;
            return 1;
        }
    }
    
    // Calculate initial k (distinct symbols visible)
    auto calculate_k = [&]() {
        set<int> distinct;
        for (int w : wheels) {
            distinct.insert(w);
        }
        return (int)distinct.size();
    };
    
    int k = calculate_k();
    
    // Output initial configuration
    cout << n << " " << k << endl;
    cout.flush();
    
    cerr << "Judge: Started with n=" << n << ", initial k=" << k << ", config=";
    for (int w : wheels) cerr << w;
    cerr << endl;
    
    int actions = 0;
    const int MAX_ACTIONS = 10000;
    
    while (k > 1 && actions < MAX_ACTIONS) {
        int i, j;
        if (!(cin >> i >> j)) {
            cerr << "Judge: Solution terminated early or failed to read input" << endl;
            break;
        }
        
        if (i < 1 || i > n) {
            cerr << "Judge: Invalid wheel index: " << i << " (must be 1 to " << n << ")" << endl;
            cout << -1 << endl;
            cout.flush();
            break;
        }
        
        // Apply rotation to wheel i-1 (0-indexed)
        // Rotation by j positions (can be negative)
        wheels[i-1] = ((wheels[i-1] + j) % n + n) % n;
        
        k = calculate_k();
        actions++;
        
        cout << k << endl;
        cout.flush();
        
        cerr << "Judge: Action " << actions << ": Rotated wheel " << i 
             << " by " << j << ", new k=" << k << ", config=";
        for (int w : wheels) cerr << w;
        cerr << endl;
    }
    
    if (k == 1) {
        cerr << "Judge: SUCCESS! Solved in " << actions << " actions" << endl;
    } else if (actions >= MAX_ACTIONS) {
        cerr << "Judge: FAILED - Exceeded " << MAX_ACTIONS << " actions (k=" << k << ")" << endl;
    } else {
        cerr << "Judge: FAILED - Solution terminated with k=" << k << " after " << actions << " actions" << endl;
    }
    
    return 0;
}