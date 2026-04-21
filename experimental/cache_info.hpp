#ifndef CACHE_INFO_HPP
#define CACHE_INFO_HPP

#include <cstddef>
#include <string>

namespace cache_info {

struct CacheLayout {
    size_t l1_size = 0; // Data or Unified L1 (fallback used if 0)
    size_t l2_size = 0;
    size_t l3_size = 0;
    size_t line_size = 64; // Common cache line size (64 bytes)
    std::string source;    // Method used to detect (Windows, Linux, macOS, Fallback)
};

/**
 * Detects the CPU cache layout at runtime.
 * Returns a CacheLayout struct with sizes in bytes.
 */
CacheLayout detect();

/**
 * Suggests an optimal block size (number of elements) for a given type.
 * Aims to fit ~50-75% of the L1 or L2 cache.
 */
template <typename T>
size_t suggest_block_size(const CacheLayout& layout) {
    // Heuristic: Use 50% of L1 cache if possible, otherwise 512 as a safe default
    size_t target_size = (layout.l1_size > 0) ? (layout.l1_size / 2) : 32768;
    size_t elements = target_size / sizeof(T);
    
    // Ensure it's a multiple of 8 for alignment/SIMD friendliness
    return (elements > 8) ? (elements & ~7) : 8;
}

} // namespace cache_info

#endif // CACHE_INFO_HPP
