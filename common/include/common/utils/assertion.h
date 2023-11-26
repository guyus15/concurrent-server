#pragma once

#include <common/utils/logging.h>
#include <common/utils/platform.h>

#if defined(SCX_PLATFORM_WINDOWS)
#define SCX_DEBUG_BREAK() __debugbreak()
#elif defined(SCX_PLATFORM_LINUX)
#if __has_builtin(__builtin_trap)
#define SCX_DEBUG_BREAK() __builtin_trap()
#else
#define SCX_DEBUG_BREAK() abort()
#endif
#endif

#if defined(SCX_ASSERTION)
#define SCX_ASSERT(COND, MSG) do { \
    if (!(COND)) \
    { \
        SCX_CORE_CRITICAL("Assertion failed ({0}:{1}): {2}", __FILE__, __LINE__, (MSG)); \
        SCX_DEBUG_BREAK(); \
    } \
} \
while (0)
#else
#define SCX_ASSERT(COND, MSG)
#endif