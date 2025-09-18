#include <bits/stdc++.h>
using namespace std;

struct DSU {
    int N; // number of hubs (2n)
    vector<int> parent;
    vector<int> sz_hubs;   // number of hubs in component (union-by-size)
    vector<int> s_cities;  // number of distinct cities in component
    vector<vector<int>> L; // cities whose evening hub (left) is in this component
    vector<vector<int>> R; // cities whose noon hub (right) is in this component

    DSU(int n) {
        N = 2 * n;
        parent.resize(N + 1);
        sz_hubs.assign(N + 1, 1);
        s_cities.assign(N + 1, 1);
        L.resize(N + 1);
        R.resize(N + 1);
        for (int i = 1; i <= N; ++i) parent[i] = i;
        // Initialize per-node lists: node i<=n is left city i, node i>n is right city (i-n)
        for (int i = 1; i <= n; ++i) {
            L[i].push_back(i); // evening hub for city i
        }
        for (int i = 1; i <= n; ++i) {
            R[n + i].push_back(i); // noon hub for city i
        }
    }

    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    }
};

static inline long long comb2(long long x) {
    return x <= 1 ? 0LL : (x * (x - 1)) >> 1;
}

// Use 64-bit key: (minRoot << 32) | maxRoot
static inline uint64_t make_key(uint32_t a, uint32_t b) {
    if (a > b) swap(a, b);
    return (uint64_t(a) << 32) | uint64_t(b);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) {
        return 0;
    }

    DSU dsu(n);

    // sum over components of C(|S_c|, 2)
    long long sum_binom = 0;
    // sum over unordered component pairs {c1,c2} of C(k,2), where k = #cities bridging exactly those two comps
    long long sum_doubles = 0;

    // Bridge counts between component pairs
    unordered_map<uint64_t, int> bridge;
    bridge.reserve(size_t(n) * 3);

    // Initialize bridges: each city i initially bridges its two singleton components (i, n+i) with count 1
    // This keeps the counts consistent for the first updates.
    for (int i = 1; i <= n; ++i) {
        uint64_t key = make_key(uint32_t(i), uint32_t(n + i));
        bridge.emplace(key, 1);
        // sum_doubles contribution for k=1 is 0, so no change needed
    }

    auto add_pair_count = [&](int a, int b, int delta) {
        // a != b must hold
        if (a == b) return;
        uint64_t key = make_key(uint32_t(a), uint32_t(b));
        auto it = bridge.find(key);
        long long oldv = (it == bridge.end() ? 0 : it->second);
        long long newv = oldv + delta;
        // Update sum_doubles by difference in comb2
        sum_doubles += comb2(newv) - comb2(oldv);
        if (newv == 0) {
            if (it != bridge.end()) bridge.erase(it);
        } else {
            bridge[key] = int(newv);
        }
    };

    auto unite_e_noon = [&](int e_hub, int n_hub) {
        int u = dsu.find(e_hub);
        int v = dsu.find(n_hub);
        if (u == v) {
            return; // no changes to sums
        }
        // Small-to-large by number of hubs
        int small = u, large = v;
        if (dsu.sz_hubs[small] > dsu.sz_hubs[large]) swap(small, large);

        // Compute inter (number of cities appearing in both components)
        long long inter = 0;

        // Pass 1: remove old bridges involving 'small', compute inter
        // For every city with a hub in 'small', we adjust mapping for the pair {small, other}.
        // Left side cities (evening hubs)
        for (int city : dsu.L[small]) {
            int other = dsu.find(n + city);
            if (other == small) continue; // both hubs in same comp -> no bridging to remove
            if (other == large) ++inter;  // city counted in both comps
            add_pair_count(small, other, -1);
        }
        // Right side cities (noon hubs)
        for (int city : dsu.R[small]) {
            int other = dsu.find(city); // evening hub index
            if (other == small) continue;
            if (other == large) ++inter;
            add_pair_count(small, other, -1);
        }

        // Update sum_binom for merging components
        long long s1 = dsu.s_cities[small];
        long long s2 = dsu.s_cities[large];
        sum_binom += comb2(s1 + s2 - inter) - comb2(s1) - comb2(s2);

        // Union small -> large (update DSU metadata)
        dsu.parent[small] = large;
        dsu.sz_hubs[large] += dsu.sz_hubs[small];
        dsu.s_cities[large] = int(s1 + s2 - inter);

        // Pass 2: add new bridges from 'large' to 'other' (for cities that still have split hubs)
        for (int city : dsu.L[small]) {
            int other = dsu.find(n + city);
            if (other == large) continue; // both hubs in same comp now
            add_pair_count(large, other, +1);
        }
        for (int city : dsu.R[small]) {
            int other = dsu.find(city);
            if (other == large) continue;
            add_pair_count(large, other, +1);
        }

        // Move city lists from small to large (small-to-large merging)
        if (!dsu.L[small].empty()) {
            dsu.L[large].insert(dsu.L[large].end(), dsu.L[small].begin(), dsu.L[small].end());
            dsu.L[small].clear();
            dsu.L[small].shrink_to_fit(); // optional, may reduce memory
        }
        if (!dsu.R[small].empty()) {
            dsu.R[large].insert(dsu.R[large].end(), dsu.R[small].begin(), dsu.R[small].end());
            dsu.R[small].clear();
            dsu.R[small].shrink_to_fit();
        }
    };

    for (int i = 0; i < m; ++i) {
        int h, d;
        cin >> h >> d;
        // Connect h_evening (h) to d_noon (n + d)
        unite_e_noon(h, n + d);

        long long unique_pairs = sum_binom - sum_doubles;
        cout << unique_pairs << '\n';
    }

    return 0;
}