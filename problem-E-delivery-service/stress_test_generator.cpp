#include <iostream>
#include <random>

int main() {
    // Fast I/O
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n = 200000;  // Maximum cities
    int m = 400000;  // Maximum couriers
    
    std::cout << n << " " << m << '\n';
    
    // Use a fixed seed for reproducibility
    std::mt19937 gen(42);
    std::uniform_int_distribution<> dis(1, n);
    
    for (int i = 0; i < m; i++) {
        int home = dis(gen);
        int dest;
        
        // Efficient generation of dest != home
        do {
            dest = dis(gen);
        } while (dest == home);
        
        std::cout << home << " " << dest << '\n';
    }
    
    return 0;
}