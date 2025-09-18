#include <bits/stdc++.h>
using namespace std;

struct Duct {
    int src; // station id (1..S)
    vector<pair<int, double>> toStation; // (station id, fraction in [0,1])
    vector<double> toReservoir; // size R, fractions in [0,1]
};

static const double INF = 1e100;
static const double EPS = 1e-12;

int S, R, Dn;
vector<Duct> ducts;
vector<vector<int>> outDuctIds;  // per station: indices of ducts
vector<vector<int>> stAdj;       // station-to-station adjacency
vector<int> topoRev;             // reverse topological order of reachable stations
vector<char> reachable;

// Build reachable set from source (station 1) through station-level edges
void computeReachable() {
    reachable.assign(S + 1, 0);
    deque<int> dq;
    reachable[1] = 1;
    dq.push_back(1);
    while (!dq.empty()) {
        int u = dq.front();
        dq.pop_front();
        for (int v : stAdj[u]) {
            if (!reachable[v]) {
                reachable[v] = 1;
                dq.push_back(v);
            }
        }
    }
}

// Topological sort on reachable subgraph; fills topoRev with reachable nodes in reverse topo order.
// Returns true if successful (DAG on reachable subgraph), false if cycle detected.
bool topologicalOrderReachable() {
    computeReachable();
    vector<int> indeg(S + 1, 0);
    for (int u = 1; u <= S; ++u) {
        if (!reachable[u]) continue;
        for (int v : stAdj[u]) {
            if (reachable[v]) indeg[v]++;
        }
    }
    deque<int> dq;
    for (int u = 1; u <= S; ++u) {
        if (reachable[u] && indeg[u] == 0) dq.push_back(u);
    }
    vector<int> order;
    order.reserve(S);
    while (!dq.empty()) {
        int u = dq.front();
        dq.pop_front();
        order.push_back(u);
        for (int v : stAdj[u]) {
            if (!reachable[v]) continue;
            indeg[v]--;
            if (indeg[v] == 0) dq.push_back(v);
        }
    }
    int reachableCount = 0;
    for (int u = 1; u <= S; ++u) if (reachable[u]) reachableCount++;
    if ((int)order.size() != reachableCount) {
        // Cycle detected in reachable subgraph
        return false;
    }
    topoRev = order;
    reverse(topoRev.begin(), topoRev.end());
    return true;
}

// Evaluate Φ(z) = max_policy z·f from a given station using DP on reverse topo order (DAG).
double evaluatePhiAtZ(const vector<double>& z) {
    vector<double> val(S + 1, 0.0);
    // Only compute for reachable nodes; others stay 0 and won't be used from source.
    for (int s : topoRev) {
        if (!reachable[s]) continue;
        double best = 0.0; // baseline: can get 0 if no path to reservoirs
        for (int did : outDuctIds[s]) {
            const Duct& d = ducts[did];
            // c_d(z): immediate expected reward to reservoirs
            double sum = 0.0;
            // to reservoirs
            for (int k = 0; k < R; ++k) {
                if (d.toReservoir[k] > 0.0) sum += d.toReservoir[k] * z[k];
            }
            // plus continuation via stations
            for (auto [t, p] : d.toStation) {
                if (p > 0.0) sum += p * val[t];
            }
            if (sum > best) best = sum;
        }
        val[s] = best;
    }
    return val[1]; // source station is 1
}

// Specialized exact solver for R=2 using 1D convex minimization
double solveR2Exact() {
    // For R=2, we minimize Φ(z) = max_policy z·f over z in the 1-simplex
    // z = {z1, z2} with z1 + z2 = 1, z1, z2 >= 0
    // Parametrize: z1 = a ∈ [0,1], z2 = 1 - a
    // Find a that minimizes Φ({a, 1-a})
    
    // Ternary search on a ∈ [0,1]
    double L = 0.0, R = 1.0;
    const int ITERATIONS = 80;  // Sufficient for 1e-9 precision
    
    for (int iter = 0; iter < ITERATIONS; ++iter) {
        double a1 = L + (R - L) / 3.0;
        double a2 = R - (R - L) / 3.0;
        
        vector<double> z1 = {a1, 1.0 - a1};
        vector<double> z2 = {a2, 1.0 - a2};
        
        double phi1 = evaluatePhiAtZ(z1);
        double phi2 = evaluatePhiAtZ(z2);
        
        if (phi1 < phi2) {
            R = a2;
        } else {
            L = a1;
        }
    }
    
    // Final evaluation at midpoint
    double a_final = (L + R) / 2.0;
    vector<double> z_final = {a_final, 1.0 - a_final};
    return evaluatePhiAtZ(z_final);
}

// Minimize Φ(z) over z in simplex for R=3 by nested ternary search on the triangle.
//
// Parametrization: z1 = a in [0,1], z2 = b in [0, 1 - a], z3 = 1 - a - b.
// The function Φ is convex and piecewise linear; nested ternary yields the global minimum to required precision.
double solveR3Minimize() {
    auto innerMin = [&](double a) -> double {
        double L = 0.0, Rr = 1.0 - a;
        // If Rr==0, only z2=0 allowed; return Φ at that point
        if (Rr <= 0.0) {
            vector<double> z = {a, 0.0, 1.0 - a};
            return evaluatePhiAtZ(z);
        }
        const int IT_IN = 60;
        for (int it = 0; it < IT_IN; ++it) {
            double b1 = L + (Rr - L) / 3.0;
            double b2 = Rr - (Rr - L) / 3.0;
            vector<double> z1 = {a, b1, 1.0 - a - b1};
            vector<double> z2 = {a, b2, 1.0 - a - b2};
            double f1 = evaluatePhiAtZ(z1);
            double f2 = evaluatePhiAtZ(z2);
            if (f1 < f2) {
                Rr = b2;
            } else {
                L = b1;
            }
        }
        double b = (L + Rr) / 2.0;
        vector<double> z = {a, b, 1.0 - a - b};
        return evaluatePhiAtZ(z);
    };

    double L = 0.0, Rr = 1.0;
    const int IT_OUT = 60;
    for (int it = 0; it < IT_OUT; ++it) {
        double a1 = L + (Rr - L) / 3.0;
        double a2 = Rr - (Rr - L) / 3.0;
        double g1 = innerMin(a1);
        double g2 = innerMin(a2);
        if (g1 < g2) {
            Rr = a2;
        } else {
            L = a1;
        }
    }
    double a = (L + Rr) / 2.0;
    return innerMin(a);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> S >> R >> Dn)) {
        return 0;
    }
    ducts.clear();
    ducts.reserve(Dn);
    outDuctIds.assign(S + 1, {});
    stAdj.assign(S + 1, {});

    for (int i = 0; i < Dn; ++i) {
        int src, n;
        cin >> src >> n;
        
        Duct d;
        d.src = src;
        d.toReservoir.assign(R, 0.0);
        
        // Remove the normalization approach - directly process each output
        for (int j = 0; j < n; ++j) {
            int output_id;
            double percentage;
            cin >> output_id >> percentage;
            
            // Convert percentage to fraction (divide by 100) without normalization
            // Flow can be lost if percentages sum to less than 100
            double fraction = percentage / 100.0;
            
            if (fraction > 0.0) {  // Only add non-zero edges
                if (output_id >= 1 && output_id <= S) {
                    // This is a station
                    d.toStation.push_back({output_id, fraction});
                    stAdj[src].push_back(output_id);
                } else if (output_id > S && output_id <= S + R) {
                    // This is a reservoir
                    int reservoir_idx = output_id - S - 1;  // Convert to 0-based reservoir index
                    d.toReservoir[reservoir_idx] += fraction;  // Use += in case multiple outputs to same reservoir
                }
            }
        }
        
        // Push duct and record in outDuctIds
        int idx = (int)ducts.size();
        ducts.push_back(std::move(d));
        if (src >= 1 && src <= S) {
            outDuctIds[src].push_back(idx);
        }
    }

    // Build reverse topo order on reachable subgraph (assumes DAG)
    bool ok = topologicalOrderReachable();
    if (!ok) {
        // If cycles exist in reachable subgraph, we proceed anyway with a heuristic order:
        topoRev.clear();
        for (int i = S; i >= 1; --i) topoRev.push_back(i);
        computeReachable();
    }

    double resultFraction = 0.0;
    if (R == 1) {
        // Only one reservoir: all flow that reaches reservoirs must end there
        vector<double> z = {1.0};
        resultFraction = evaluatePhiAtZ(z);
    } else if (R == 2) {
        // Exact O(S + D) computation via 1D convex minimization
        resultFraction = solveR2Exact();
    } else {
        // R == 3: minimize Φ(z) on 2D simplex via nested ternary search
        resultFraction = solveR3Minimize();
    }

    // Output in percentage with 1e-6 precision
    double ansPercent = resultFraction * 100.0;
    cout.setf(std::ios::fixed); 
    cout << setprecision(6) << ansPercent << "\n";
    return 0;
}