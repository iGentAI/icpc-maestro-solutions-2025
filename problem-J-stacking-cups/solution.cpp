#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, h;
    if (!(cin >> n >> h)) return 0;

    if (n <= 0) {
        cout << "impossible\n";
        return 0;
    }

    // Feasibility bounds under the correct model:
    // Total height H for a permutation partitioned into decreasing groups
    // with leaders T is: H = sum(T) + n - |T|.
    // Therefore:
    //   Hmin = 2n - 1  (single group with leader n),
    //   Hmax = n(n + 1)/2 (all singleton groups).
    __int128 Hmin = 2 * (__int128)n - 1;
    __int128 Hmax = ((__int128)n * (n + 1)) / 2;

    if ((__int128)h < Hmin || (__int128)h > Hmax) {
        cout << "impossible\n";
        return 0;
    }

    // Extra height beyond minimal configuration:
    // E = h - Hmin = sum_{t in additional leaders}(t - 1).
    // Represent E as sum of distinct integers from {1, 2, ..., n-2}.
    long long s = (long long)((__int128)h - Hmin);

    // Greedy subset selection from {1, 2, ..., n-2}.
    // This always succeeds since the set is consecutive and
    // sum_{a=1}^{n-2} a = (n-1)(n-2)/2 = Hmax - Hmin.
    vector<long long> moves;
    for (long long a = n - 2; a >= 1; --a) {
        if (s >= a) {
            moves.push_back(a);
            s -= a;
        }
    }
    if (s != 0) {
        // This should be unreachable due to the feasibility check above.
        cout << "impossible\n";
        return 0;
    }

    // Construct the permutation by starting from the fully decreasing order
    // [n, n-1, ..., 1]. For each a in moves (processed in descending order),
    // move cup 'a' to immediately before cup 'a+1'.
    // This creates a new group boundary just before (a+1), making (a+1) a leader,
    // and increases the total height by exactly 'a'.
    vector<long long> L(n + 2, 0), R(n + 2, 0);
    long long head = n;
    for (long long i = n; i >= 1; --i) {
        R[i] = (i - 1 >= 1 ? i - 1 : 0);
        L[i] = (i + 1 <= n ? i + 1 : 0);
    }

    auto detach = [&](long long x) {
        long long l = L[x], r = R[x];
        if (l != 0) R[l] = r; else head = r;
        if (r != 0) L[r] = l;
        L[x] = R[x] = 0;
    };

    auto insert_before = [&](long long x, long long t) {
        long long l = L[t];
        L[x] = l; R[x] = t;
        if (l != 0) R[l] = x; else head = x;
        L[t] = x;
    };

    // Process moves in descending 'a' (already constructed that way).
    for (long long a : moves) {
        long long t = a + 1;
        if (R[a] == t) continue; // already positioned
        detach(a);
        insert_before(a, t);
    }

    // Output cup heights (2*i - 1) from bottom (head) to top.
    bool first = true;
    for (long long cur = head; cur != 0; cur = R[cur]) {
        if (!first) cout << ' ';
        first = false;
        cout << (2 * cur - 1);
    }
    cout << '\n';
    return 0;
}