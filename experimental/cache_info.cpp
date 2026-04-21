#include "cache_info.hpp"
#include <iostream>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(__APPLE__) || defined(__MACH__)
#include <sys/sysctl.h>
#include <sys/types.h>
#elif defined(__linux__)
#include <fstream>
#include <sstream>
#endif

namespace cache_info {

CacheLayout detect() {
    CacheLayout layout;
    
#if defined(_WIN32) || defined(_WIN64)
    layout.source = "Windows API";
    DWORD buffer_size = 0;
    GetLogicalProcessorInformation(nullptr, &buffer_size);
    if (buffer_size > 0) {
        std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> buffer(buffer_size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));
        if (GetLogicalProcessorInformation(buffer.data(), &buffer_size)) {
            for (const auto& info : buffer) {
                if (info.Relationship == RelationCache) {
                    if (info.Cache.Level == 1) layout.l1_size = info.Cache.Size;
                    else if (info.Cache.Level == 2) layout.l2_size = info.Cache.Size;
                    else if (info.Cache.Level == 3) layout.l3_size = info.Cache.Size;
                    layout.line_size = info.Cache.LineSize;
                }
            }
        }
    }

#elif defined(__APPLE__) || defined(__MACH__)
    layout.source = "macOS sysctl";
    size_t size = sizeof(int64_t);
    int64_t val = 0;
    if (sysctlbyname("hw.l1dcachesize", &val, &size, NULL, 0) == 0) layout.l1_size = val;
    if (sysctlbyname("hw.l2cachesize", &val, &size, NULL, 0) == 0) layout.l2_size = val;
    if (sysctlbyname("hw.l3cachesize", &val, &size, NULL, 0) == 0) layout.l3_size = val;
    if (sysctlbyname("hw.cachelinesize", &val, &size, NULL, 0) == 0) layout.line_size = val;

#elif defined(__linux__)
    layout.source = "Linux sysfs";
    auto read_sysfs = [](const std::string& path) -> size_t {
        std::ifstream file(path);
        if (!file.is_open()) return 0;
        std::string line;
        std::getline(file, line);
        // Sizes in sysfs are often in KB (e.g., "32K"), but here we'll assume bytes for simplicity or parse
        if (line.empty()) return 0;
        size_t val = std::stoul(line);
        if (line.back() == 'K') val *= 1024;
        else if (line.back() == 'M') val *= 1024 * 1024;
        return val;
    };
    
    // Linux provides cache info per CPU core
    layout.l1_size = read_sysfs("/sys/devices/system/cpu/cpu0/cache/index0/size"); // L1 Data
    layout.l2_size = read_sysfs("/sys/devices/system/cpu/cpu0/cache/index2/size"); // L2
    layout.l3_size = read_sysfs("/sys/devices/system/cpu/cpu0/cache/index3/size"); // L3
    layout.line_size = read_sysfs("/sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size");

#endif

    // Fallback if detection failed or returned 0
    if (layout.l1_size == 0) {
        layout.l1_size = 32768; // 32KB default
        if (layout.source.empty()) layout.source = "Fallback (Default Values)";
        else layout.source += " (with Fallback)";
    }
    if (layout.l2_size == 0) layout.l2_size = 262144; // 256KB default
    if (layout.l3_size == 0) layout.l3_size = 8388608; // 8MB default

    return layout;
}

} // namespace cache_info
