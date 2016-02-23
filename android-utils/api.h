#ifndef ANDROID_UTILS_API_H
#define ANDROID_UTILS_API_H

#include <stdint.h>

typedef enum CpuFamily_e {
    UNKNOWN = 0,
    ARM,
    X86
}
CpuFamily_t;

typedef enum CpuFeatures_e {
    ARMv7 = 1,
    VFPv3 = 2,
    NEON = 4
}
CpuFeatures_t;

class AndroidUtils
{
public:
    AndroidUtils() {};
    virtual ~AndroidUtils() {};

    static uint64_t getCpuFamily();
    static uint64_t getCpuFeatures();
};

#endif /* ANDROID_UTILS_API_H */
