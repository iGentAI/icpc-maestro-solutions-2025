#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    double xs, ys, xt, yt;
    cin >> xs >> ys >> xt >> yt;
    
    vector<pair<double,double>> intervals;
    intervals.reserve(N);

    double Ymin = min(ys, yt);
    double Ymax = max(ys, yt);

    // If no southward movement required
    if (ys <= yt) {
        cout.setf(std::ios::fixed); 
        cout << setprecision(10) << 0.0 << "\n";
        // Still need to consume rectangle input
        for (int i = 0; i < N; ++i) {
            double x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;
        }
        return 0;
    }

    const double EPS = 1e-12;

    for (int i = 0; i < N; ++i) {
        double x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        double ylo = min(y1, y2);
        double yhi = max(y1, y2);
        // Clip to [Ymin, Ymax]
        double l = max(ylo, Ymin);
        double r = min(yhi, Ymax);
        if (r > l + EPS) {
            intervals.emplace_back(l, r);
        }
    }

    if (intervals.empty()) {
        double ans = Ymax - Ymin;
        cout.setf(std::ios::fixed);
        cout << setprecision(10) << ans << "\n";
        return 0;
    }

    sort(intervals.begin(), intervals.end(), [&](const auto& a, const auto& b){
        if (a.first < b.first - EPS) return true;
        if (a.first > b.first + EPS) return false;
        return a.second < b.second;
    });

    double covered = 0.0;
    double curL = intervals[0].first;
    double curR = intervals[0].second;

    for (size_t i = 1; i < intervals.size(); ++i) {
        double l = intervals[i].first;
        double r = intervals[i].second;
        if (l <= curR + EPS) {
            if (r > curR) curR = r;
        } else {
            covered += max(0.0, curR - curL);
            curL = l; curR = r;
        }
    }
    covered += max(0.0, curR - curL);

    double totalDrop = Ymax - Ymin;
    double ans = totalDrop - covered;
    if (ans < 0) ans = 0; // clamp due to numeric tolerance

    cout.setf(std::ios::fixed);
    cout << setprecision(10) << ans << "\n";
    return 0;
}