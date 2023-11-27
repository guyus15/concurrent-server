#pragma once

// Determine the target machine's platform and set up appropriate definitions accordingly.
#if defined(__unix__) || defined(__linux__)
#define SCX_PLATFORM_LINUX
#elif defined(_WIN32) || defined(WIN32)
#define SCX_PLATFORM_WINDOWS
#endif

#ifndef __has_builtin
#define __has_builtin(x) false
#endif
