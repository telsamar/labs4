
#include <vector>

struct HardwareData
{
    using CacheMemoryType = std::vector<size_t>;

    CacheMemoryType cacheMemoryKB;
};