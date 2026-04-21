#include <iostream>
#include <iomanip>
#include "cache_info.hpp"

int main() {
    auto layout = cache_info::detect();
    
    std::cout << "--- CPU Cache Layout Detection ---" << std::endl;
    std::cout << "Detection Method: " << layout.source << std::endl;
    std::cout << "L1 Data Cache:   " << std::setw(6) << (layout.l1_size / 1024) << " KB" << std::endl;
    std::cout << "L2 Cache:        " << std::setw(6) << (layout.l2_size / 1024) << " KB" << std::endl;
    std::cout << "L3 Cache:        " << std::setw(6) << (layout.l3_size / (1024 * 1024)) << " MB" << std::endl;
    std::cout << "Cache Line Size: " << std::setw(6) << layout.line_size << " bytes" << std::endl;
    std::cout << "----------------------------------" << std::endl;
    
    size_t block_size = cache_info::suggest_block_size<double>(layout);
    std::cout << "Suggested block size for double: " << block_size << " elements" << std::endl;
    std::cout << "(Approx. " << (block_size * sizeof(double) / 1024.0) << " KB)" << std::endl;
    
    return 0;
}
