#include "api.h"
#include "cpu-features.h"

uint64_t AndroidUtils::getCpuFamily()
{
    return (uint64_t)android_getCpuFamily();
}

uint64_t AndroidUtils::getCpuFeatures()
{
    return (uint64_t)android_getCpuFeatures();
}