#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m, k, tx, ty;
    cin >> n >> m >> k >> tx >> ty;
    
    // Convert to 0-based indexing
    tx--; ty--;
    
    const int ROWS = n;
    const int COLS = m;
    const int N = ROWS + COLS;
    
    vector<vector<pair<int, long long>>> adj(N);
    
    auto rowNode = [&](int i) { return i; };
    auto colNode = [&](int j) { return ROWS + j; };
    
    bool impossible = false;
    
    for (int i = 0; i < k; i++) {
        int x, y;
        long long d;
        cin >> x >> y >> d;
        x--; y--; // Convert to 0-based
        
        if (x < 0 || x >= ROWS || y < 0 || y >= COLS) {
            impossible = true;
            continue;
        }
        
        if (d < 0) {
            impossible = true;
            continue;
        }
        
        int u = rowNode(x);
        int v = colNode(y);
        adj[u].push_back({v, d});
        adj[v].push_back({u, d});
    }
    
    if (impossible) {
        cout << "impossible\n";
        return 0;
    }
    
    // BFS to find connected components and assign values
    vector<long long> val(N, 0);
    vector<int> comp(N, -1);
    vector<bool> visited(N, false);
    
    struct CompInfo {
        bool hasRow = false;
        bool hasCol = false;
        long long minRow = LLONG_MAX;
        long long minCol = LLONG_MAX;
    };
    
    vector<CompInfo> comps;
    
    for (int start = 0; start < N && !impossible; start++) {
        if (comp[start] != -1) continue;
        
        int cid = comps.size();
        comps.push_back(CompInfo{});
        
        queue<int> q;
        q.push(start);
        comp[start] = cid;
        visited[start] = true;
        val[start] = 0;
        
        while (!q.empty() && !impossible) {
            int u = q.front();
            q.pop();
            
            if (u < ROWS) {
                comps[cid].hasRow = true;
                comps[cid].minRow = min(comps[cid].minRow, val[u]);
            } else {
                comps[cid].hasCol = true;
                comps[cid].minCol = min(comps[cid].minCol, val[u]);
            }
            
            for (auto [v, z] : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    comp[v] = cid;
                    val[v] = z - val[u];
                    q.push(v);
                } else {
                    // Check consistency
                    if (val[u] + val[v] != z) {
                        impossible = true;
                        break;
                    }
                }
            }
        }
        
        if (impossible) break;
        
        // Check non-negativity constraint
        if (comps[cid].hasRow && comps[cid].hasCol) {
            if (comps[cid].minRow + comps[cid].minCol < 0) {
                impossible = true;
                break;
            }
        }
    }
    
    if (impossible) {
        cout << "impossible\n";
        return 0;
    }
    
    // Calculate minimum depth at treasure location
    // The treasure is at grid point (tx, ty)
    // Depth = a_tx + b_ty = val[rowNode(tx)] + val[colNode(ty)] + u_comp(tx) - u_comp(ty)
    
    int row_comp = comp[rowNode(tx)];
    int col_comp = comp[colNode(ty)];
    
    long long base_depth = val[rowNode(tx)] + val[colNode(ty)];
    
    if (row_comp == col_comp) {
        // Same component, u cancels out
        cout << max(0LL, base_depth) << "\n";
    } else {
        // Different components
        long long min_depth = base_depth;
        
        // We want to minimize: base_depth + u_row - u_col
        // Subject to:
        // - If comp[row] has rows: u_row >= -minRow[row_comp]
        // - If comp[row] has cols: u_row <= minCol[row_comp]
        // - If comp[col] has rows: u_col >= -minRow[col_comp]
        // - If comp[col] has cols: u_col <= minCol[col_comp]
        
        // To minimize, set u_row to minimum, u_col to maximum
        if (comps[row_comp].hasRow) {
            min_depth += (-comps[row_comp].minRow);
        }
        if (comps[col_comp].hasCol) {
            min_depth -= comps[col_comp].minCol;
        }
        
        cout << max(0LL, min_depth) << "\n";
    }
    
    return 0;
}