#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double INF = 1e18;

struct Point {
    double x, y, z;
};

struct Triangle {
    int a, b, c; // 0-indexed vertices
};

// Quantize coordinates to handle floating point precision issues
pair<int, int> quantizeCoord(double x, double y, double scale = 1e6) {
    return {(int)round(x * scale), (int)round(y * scale)};
}

// Compute intersection of edge with horizontal plane at height z
pair<bool, Point> edgeIntersection(const Point& p1, const Point& p2, double z) {
    if (abs(p1.z - p2.z) < EPS) {
        // Edge is horizontal
        if (abs(p1.z - z) < EPS) {
            // Edge is at the exact height - degenerate case
            return {false, Point()};
        }
        return {false, Point()};
    }
    
    if ((z < min(p1.z, p2.z) - EPS) || (z > max(p1.z, p2.z) + EPS)) {
        return {false, Point()};
    }
    
    double t = (z - p1.z) / (p2.z - p1.z);
    Point intersection;
    intersection.x = p1.x + t * (p2.x - p1.x);
    intersection.y = p1.y + t * (p2.y - p1.y);
    intersection.z = z;
    
    return {true, intersection};
}

double distance(const Point& p1, const Point& p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

double solve(int w, vector<Point>& vertices, vector<Triangle>& triangles) {
    int n = vertices.size();
    
    // Get all unique z-values
    set<double> zValues;
    for (const auto& v : vertices) {
        zValues.insert(v.z);
    }
    
    double minPath = INF;
    
    // For each unique z-value, plus some intermediate values
    vector<double> testZ(zValues.begin(), zValues.end());
    
    // Add intermediate values - FIX: capture original size
    int originalSize = testZ.size();
    for (int i = 0; i + 1 < originalSize; i++) {
        double mid = (testZ[i] + testZ[i+1]) / 2.0;
        testZ.push_back(mid);
    }
    
    sort(testZ.begin(), testZ.end());
    
    for (double z : testZ) {
        // Build graph of level curves at height z using quantized coordinates
        map<pair<int,int>, vector<pair<pair<int,int>, double>>> graph;
        
        for (const auto& tri : triangles) {
            Point v1 = vertices[tri.a];
            Point v2 = vertices[tri.b];
            Point v3 = vertices[tri.c];
            
            vector<Point> intersections;
            
            // Check each edge of the triangle
            auto [has1, p1] = edgeIntersection(v1, v2, z);
            auto [has2, p2] = edgeIntersection(v2, v3, z);
            auto [has3, p3] = edgeIntersection(v3, v1, z);
            
            if (has1) intersections.push_back(p1);
            if (has2) intersections.push_back(p2);
            if (has3) intersections.push_back(p3);
            
            // Also check vertices at exact height
            if (abs(v1.z - z) < EPS) intersections.push_back(v1);
            if (abs(v2.z - z) < EPS) intersections.push_back(v2);
            if (abs(v3.z - z) < EPS) intersections.push_back(v3);
            
            // Remove duplicates within this triangle
            sort(intersections.begin(), intersections.end(), [](const Point& a, const Point& b) {
                if (abs(a.x - b.x) > EPS) return a.x < b.x;
                return a.y < b.y;
            });
            
            auto last = unique(intersections.begin(), intersections.end(), [](const Point& a, const Point& b) {
                return abs(a.x - b.x) < EPS && abs(a.y - b.y) < EPS;
            });
            intersections.erase(last, intersections.end());
            
            // Connect pairs of intersection points using quantized coordinates
            if (intersections.size() == 2) {
                auto q1 = quantizeCoord(intersections[0].x, intersections[0].y);
                auto q2 = quantizeCoord(intersections[1].x, intersections[1].y);
                double dist = distance(intersections[0], intersections[1]);
                
                graph[q1].push_back({q2, dist});
                graph[q2].push_back({q1, dist});
            }
        }
        
        // Find shortest path from west (x=0) to east (x=w)
        set<pair<int,int>> westNodes, eastNodes;
        for (const auto& [node, _] : graph) {
            double x = node.first / 1e6;  // Unquantize for comparison
            if (abs(x - 0) < EPS) westNodes.insert(node);
            if (abs(x - w) < EPS) eastNodes.insert(node);
        }
        
        if (westNodes.empty() || eastNodes.empty()) continue;
        
        // Run Dijkstra from all west nodes
        map<pair<int,int>, double> dist;
        priority_queue<pair<double, pair<int,int>>, 
                      vector<pair<double, pair<int,int>>>,
                      greater<pair<double, pair<int,int>>>> pq;
        
        for (const auto& node : westNodes) {
            dist[node] = 0;
            pq.push({0, node});
        }
        
        while (!pq.empty()) {
            auto [d, curr] = pq.top();
            pq.pop();
            
            if (d > dist[curr]) continue;
            
            if (graph.find(curr) != graph.end()) {
                for (const auto& [nextNode, edgeDist] : graph[curr]) {
                    double newDist = d + edgeDist;
                    
                    if (dist.find(nextNode) == dist.end() || newDist < dist[nextNode]) {
                        dist[nextNode] = newDist;
                        pq.push({newDist, nextNode});
                    }
                }
            }
        }
        
        // Find minimum distance to any east node
        for (const auto& node : eastNodes) {
            if (dist.find(node) != dist.end()) {
                minPath = min(minPath, dist[node]);
            }
        }
    }
    
    return minPath;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    
    while (t--) {
        int w, l, n, m;
        cin >> w >> l >> n >> m;
        
        vector<Point> vertices(n);
        for (int i = 0; i < n; i++) {
            cin >> vertices[i].x >> vertices[i].y >> vertices[i].z;
        }
        
        vector<Triangle> triangles(m);
        for (int i = 0; i < m; i++) {
            cin >> triangles[i].a >> triangles[i].b >> triangles[i].c;
            // Convert from 1-indexed to 0-indexed
            triangles[i].a--;
            triangles[i].b--;
            triangles[i].c--;
        }
        
        double result = solve(w, vertices, triangles);
        
        if (result >= INF / 2) {
            cout << "impossible\n";
        } else {
            cout << fixed << setprecision(10) << result << "\n";
        }
    }
    
    return 0;
}