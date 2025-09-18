#include <bits/stdc++.h>
using namespace std;

/*
Herding Cats - Bipartite Matching solution (Kuhn's algorithm)

Model:
- m pots (1..m), m plants (1..m).
- n cats; cat i likes set S_i and targets pot t_i, stopping at the first pot
  that contains a liked plant.

Constraints conversion:
- For each plant p, it cannot be placed before the maximum target among cats who like p:
  min_pos[p] = max(t_i | p in S_i), or 1 if no such cats.
- For each pot k, if one or more cats target k, the plant at k must be liked by all those cats:
  Inter_k = intersection of liked sets of cats with t_i = k.

Graph:
- Left nodes: constrained pots Kc = { k | there exists a cat with t_i = k }.
- Right nodes: plants p in [1..m].
- Edge (k, p) if p ∈ Inter_k and k >= min_pos[p].

We need to check if a perfect matching for all constrained pots exists. If yes, and if the global
availability constraint T_k = #{p | min_pos[p] <= k} satisfies T_k >= k for all k, then the remaining
(unconstrained) pots can always be filled greedily with remaining plants.

Algorithm steps per test:
1) Read input, deduplicate each cat's liked set.
2) Compute min_pos[p].
3) Early feasibility check: for all k in [1..m], T_k >= k where T_k = sum_{x<=k} count(min_pos == x).
4) For each constrained pot k, compute Inter_k using counting over the cats in its group, and build edges
   to plants p in Inter_k with min_pos[p] <= k.
5) Run Kuhn's algorithm to attempt to match all constrained pots.
6) Output "yes" if matching size equals number of constrained pots, else "no".

Complexity:
- Preprocessing and intersections: O(sum |S_i|).
- Graph edges: sum_k |Inter_k| filtered by min_pos, which is <= sum |S_i|.
- Kuhn's algorithm: typically O(L * E) where L is #constrained pots and E is total edges, acceptable with sparse graph.
- Memory: O(m + sum |S_i| + E).

Notes:
- We avoid building edges for unconstrained pots to prevent dense graphs.
- We deduplicate each cat's liked list to maintain set semantics.
*/

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) {
        return 0;
    }
    while (T--) {
        int n, m;
        cin >> n >> m;

        vector<vector<int>> likes(n);
        vector<int> target(n);
        vector<vector<int>> cats_by_target(m + 1);
        vector<int> min_pos(m + 1, 1);

        for (int i = 0; i < n; ++i) {
            int t, s;
            cin >> t >> s;
            target[i] = t;
            likes[i].resize(s);
            for (int j = 0; j < s; ++j) {
                int p;
                cin >> p;
                likes[i][j] = p;
            }
            // Deduplicate liked plants per cat to preserve set semantics
            auto &v = likes[i];
            sort(v.begin(), v.end());
            v.erase(unique(v.begin(), v.end()), v.end());

            // Update min_pos for unique liked plants
            for (int p : v) {
                if (p >= 1 && p <= m)
                    min_pos[p] = max(min_pos[p], t);
            }
            // Group cat by its target pot
            if (t >= 1 && t <= m)
                cats_by_target[t].push_back(i);
        }

        // Early feasibility check for unconstrained positions via min_pos
        // T_k = number of plants with min_pos <= k should be >= k for all k
        vector<int> freq(m + 2, 0);
        for (int p = 1; p <= m; ++p) {
            int mp = min_pos[p];
            if (mp < 1) mp = 1;
            if (mp > m) mp = m; // though min_pos cannot exceed m by definition
            freq[mp]++;
        }
        bool feasible = true;
        long long pref = 0;
        for (int k = 1; k <= m; ++k) {
            pref += freq[k];
            if (pref < k) {
                feasible = false;
                break;
            }
        }
        if (!feasible) {
            cout << "no\n";
            continue;
        }

        // Build bipartite graph only for constrained pots
        vector<vector<int>> adj;
        adj.reserve(m);
        vector<int> cnt(m + 1, 0);
        vector<int> touched;
        bool ok = true;

        for (int k = 1; k <= m; ++k) {
            const auto &group = cats_by_target[k];
            if (group.empty()) continue;

            touched.clear();
            // Count frequency of plants across all cats in the group
            for (int cat : group) {
                for (int p : likes[cat]) {
                    if (cnt[p] == 0) touched.push_back(p);
                    ++cnt[p];
                }
            }

            vector<int> allowed;
            allowed.reserve(touched.size());
            for (int p : touched) {
                if (cnt[p] == (int)group.size() && min_pos[p] <= k) {
                    allowed.push_back(p);
                }
                cnt[p] = 0; // reset for next group
            }

            if (allowed.empty()) {
                ok = false; // No plant can be placed at this constrained pot
                break;
            }
            adj.push_back(move(allowed));
        }

        if (!ok) {
            cout << "no\n";
            continue;
        }

        // If there are no constrained pots, min_pos check already guarantees feasibility
        int L = (int)adj.size();
        if (L == 0) {
            cout << "yes\n";
            continue;
        }

        // Kuhn's algorithm (DFS-based) to match all constrained pots
        vector<int> matchR(m + 1, -1); // match on plant side: plant p -> left index u
        vector<int> seen(m + 1, 0);
        int seenIter = 0;

        function<bool(int, int)> dfs = [&](int u, int iter) -> bool {
            for (int v : adj[u]) {
                if (seen[v] == iter) continue;
                seen[v] = iter;
                if (matchR[v] == -1 || dfs(matchR[v], iter)) {
                    matchR[v] = u;
                    return true;
                }
            }
            return false;
        };

        int matched = 0;
        for (int u = 0; u < L; ++u) {
            int iter = ++seenIter;
            if (dfs(u, iter)) {
                ++matched;
            } else {
                break; // cannot match this constrained pot
            }
        }

        cout << (matched == L ? "yes\n" : "no\n");
    }

    return 0;
}