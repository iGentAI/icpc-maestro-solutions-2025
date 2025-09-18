#include <bits/stdc++.h>
using namespace std;

/*
Buggy Rover - Minimum ordering changes

Problem model (inferred from specification):
- The rover lives on a grid with obstacles ('#') and a starting cell 'S'.
- At each step, the rover uses an internal priority ordering of the 4 directions
  {N, E, S, W} (a permutation of size 4) and moves in the first direction, in that
  order, that is legal (stays within the grid and does not hit a rock).
- We are given the observed sequence of moves (each of N/E/S/W) the rover made.
- We must find the minimum number of times the rover changed its direction ordering
  (i.e., modified the permutation) across the whole sequence to be consistent with
  the observed moves. Changing to any different permutation costs 1 at that step;
  keeping the same permutation costs 0. The initial permutation choice costs 0.

Key insights:
- There are only 24 permutations of the 4 directions.
- For a given position (r,c) and a desired move d in {N,E,S,W}, the set of permutations
  that produce move d are exactly those for which d is the first legal direction
  at (r,c) in that permutation.
- Let L = length of the move sequence (<= 10,000).
  We can build a mask per step i (0..L-1): mask[i] is a 24-bit bitset of permutations
  that yield the observed move at position pos[i].
- Dynamic Programming:
  dp[i][p] = minimum number of ordering changes up to step i if we use permutation p
             at step i (and it must be valid per mask[i]).
  Transitions:
    dp[i][p] = min( dp[i-1][p], min_q dp[i-1][q] + 1 ) over all q,
               but only if p is allowed at step i.
  We can optimize to O(24) per step using the trick:
    bestPrev = min_q dp[i-1][q]
    dp[i][p] = min(dp[i-1][p], bestPrev + 1), if p is allowed at step i; INF otherwise.
- We must also verify that the observed move sequence is physically possible:
  starting from 'S', following the moves must never go out of bounds or into a rock.
  If that happens, the answer is impossible (-1).

Complexity:
- Precompute masks per step: for each i, check all 24 permutations and up to 4 directions
  per permutation: O(L * 24 * 4) ~ O(L). With L up to 10^4, this is trivial.
- DP transitions: O(L * 24).

Implementation details:
- Directions are indexed as 0:N, 1:E, 2:S, 3:W.
- We'll generate all 24 permutations in orders[24][4].
- For each step i at position pos[i], to compute mask[i], we evaluate, for each
  permutation p, the first legal direction at pos[i]. If it equals the observed move,
  we set bit p in mask[i].
- If any observed move from pos[i] is illegal, we can immediately return -1.
- If L == 0, the answer is 0.

Input format (assumed from common conventions and the specification):
- First line: H W (integers).
- Next H lines: grid of length W: '.' empty, '#' rock, 'S' start.
- Next line: a string of moves consisting of characters 'N', 'E', 'S', 'W'.
  Additionally, we'll accept 'U','R','D','L' as aliases for N,E,S,W for robustness.

Output:
- A single integer: minimal number of ordering changes, or -1 if impossible.

*/

static inline int dirFromChar(char c) {
    switch (c) {
        case 'N': case 'U': return 0; // North / Up
        case 'E': case 'R': return 1; // East / Right
        case 'S': case 'D': return 2; // South / Down
        case 'W': case 'L': return 3; // West / Left
        default: return -1;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int H, W;
    if (!(cin >> H >> W)) {
        return 0;
    }
    vector<string> grid(H);
    pair<int,int> S = {-1, -1};
    for (int r = 0; r < H; ++r) {
        cin >> grid[r];
        for (int c = 0; c < W; ++c) {
            if (grid[r][c] == 'S') {
                S = {r, c};
            }
        }
    }
    string moves;
    cin >> moves;

    if (S.first == -1) {
        // No start found; impossible.
        cout << -1 << '\n';
        return 0;
    }

    const int dr[4] = {-1, 0, 1, 0};
    const int dc[4] = {0, 1, 0, -1};

    // Build the path positions pos[0..L], verifying that the observed moves are legal.
    const int L = (int)moves.size();
    vector<pair<int,int>> pos(L + 1);
    pos[0] = S;
    auto inb = [&](int r, int c) {
        return (0 <= r && r < H && 0 <= c && c < W);
    };

    for (int i = 0; i < L; ++i) {
        int d = dirFromChar(moves[i]);
        if (d < 0) {
            // Unexpected move character
            cout << -1 << '\n';
            return 0;
        }
        int r = pos[i].first;
        int c = pos[i].second;
        int nr = r + dr[d], nc = c + dc[d];
        if (!inb(nr, nc) || grid[nr][nc] == '#') {
            // Observed move is physically impossible
            cout << -1 << '\n';
            return 0;
        }
        pos[i+1] = {nr, nc};
    }

    if (L == 0) {
        cout << 0 << '\n';
        return 0;
    }

    // Enumerate all 24 permutations of directions {0,1,2,3}
    array<array<int,4>, 24> orders;
    {
        array<int,4> base = {0,1,2,3};
        int idx = 0;
        // Convert array to vector for next_permutation convenience
        vector<int> v(base.begin(), base.end());
        do {
            for (int k = 0; k < 4; ++k) {
                orders[idx][k] = v[k];
            }
            ++idx;
        } while (next_permutation(v.begin(), v.end()));
        // Sanity check
        // idx must be 24
        if (idx != 24) {
            cout << -1 << '\n';
            return 0;
        }
    }

    // Precompute per-step masks: mask[i] is a 24-bit mask of permutations p
    // such that at position pos[i], the first legal direction in orders[p]
    // equals the observed move moves[i].
    vector<uint32_t> mask(L, 0);
    for (int i = 0; i < L; ++i) {
        int r = pos[i].first, c = pos[i].second;
        // Desired direction at this step:
        int want = dirFromChar(moves[i]); // already validated above
        // Precompute legal neighbors at (r,c)
        bool legal[4];
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d], nc = c + dc[d];
            legal[d] = (inb(nr, nc) && grid[nr][nc] != '#');
        }

        uint32_t m = 0;
        for (int p = 0; p < 24; ++p) {
            int first = -1;
            // Scan the permutation until we hit a legal direction
            for (int k = 0; k < 4; ++k) {
                int d = orders[p][k];
                if (legal[d]) { first = d; break; }
            }
            if (first == want) {
                m |= (1u << p);
            }
        }
        mask[i] = m;

        // If the observed move is legal, there must exist at least one permutation (e.g., one starting with want).
        // As we already verified the observed move is physically legal (pos path built),
        // mask[i] should be non-zero. Still, keep a defensive check in case of logic changes.
        if (mask[i] == 0) {
            cout << -1 << '\n';
            return 0;
        }
    }

    // DP over steps and permutations.
    // dpPrev[p] = min changes up to previous step using permutation p (and p was valid there).
    // For step 0: any p allowed by mask[0] with cost 0, others INF.
    const int INF = 1e9;
    array<int, 24> dpPrev, dpNext;
    dpPrev.fill(INF);
    for (int p = 0; p < 24; ++p) {
        if (mask[0] & (1u << p)) dpPrev[p] = 0;
    }

    for (int i = 1; i < L; ++i) {
        int bestPrev = INF;
        for (int p = 0; p < 24; ++p) bestPrev = min(bestPrev, dpPrev[p]);

        dpNext.fill(INF);
        for (int p = 0; p < 24; ++p) {
            if (mask[i] & (1u << p)) {
                // Stay with same permutation p (no change) if it was feasible previously
                dpNext[p] = min(dpNext[p], dpPrev[p]);
                // Or switch from best feasible previous permutation
                if (bestPrev < INF) {
                    dpNext[p] = min(dpNext[p], bestPrev + 1);
                }
            }
        }
        dpPrev = dpNext;
    }

    int ans = INF;
    for (int p = 0; p < 24; ++p) ans = min(ans, dpPrev[p]);
    if (ans >= INF) ans = -1;

    cout << ans << '\n';
    return 0;
}