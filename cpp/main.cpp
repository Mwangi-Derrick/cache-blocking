#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include "../experimental/cache_info.hpp"

int main() {
    // Increase N to 1,000,000 to better observe cache effects
    const size_t N = 1000000;
    
    // Detect cache layout and suggest block size
    auto layout = cache_info::detect();
    const size_t BLOCK_SIZE = cache_info::suggest_block_size<double>(layout);

    std::vector<double> data(N);
    for (size_t i = 0; i < N; ++i) {
        data[i] = static_cast<double>(i);
    }

    std::cout << "--- Dynamic Cache Blocking (C++) ---" << std::endl;
    std::cout << "Detection Method: " << layout.source << std::endl;
    std::cout << "L1 Data Cache:    " << (layout.l1_size / 1024) << " KB" << std::endl;
    std::cout << "Array size:       " << N << " elements (" << (N * sizeof(double) / (1024 * 1024)) << " MB)" << std::endl;
    std::cout << "Dynamic Block:    " << BLOCK_SIZE << " elements (" << (BLOCK_SIZE * sizeof(double) / 1024) << " KB)" << std::endl;
    std::cout << "------------------------------------" << std::endl;

    // Non-blocked sum
    auto start1 = std::chrono::high_resolution_clock::now();
    double sum1 = 0.0;
    for (size_t i = 0; i < N; ++i) {
        sum1 += data[i];
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - start1);
    std::cout << "Non-blocked sum: " << sum1 << ", time: " << duration1.count() << " ns" << std::endl;

    // Blocked sum
    auto start2 = std::chrono::high_resolution_clock::now();
    double sum2 = 0.0;
    for (size_t block_start = 0; block_start < N; block_start += BLOCK_SIZE) {
        size_t block_end = std::min(block_start + BLOCK_SIZE, N);
        for (size_t i = block_start; i < block_end; ++i) {
            sum2 += data[i];
        }
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - start2);
    std::cout << "Blocked sum: " << sum2 << ", time: " << duration2.count() << " ns" << std::endl;

    double speedup = static_cast<double>(duration1.count()) / duration2.count();
    std::cout << "Speedup: " << speedup << "x" << std::endl;

    return 0;
}
