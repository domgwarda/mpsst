#include <iostream>
#include <chrono>
#include <vector>

int main() {

    int iterations = 5;
    for(int i = 0; i < iterations; i++)
    {
        auto start = std::chrono::steady_clock::now();

        std::vector<int> v(1000000);

        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        std::cout << i+1 << ": " << duration << "ms" << std::endl;
    }
    return 0;
}
