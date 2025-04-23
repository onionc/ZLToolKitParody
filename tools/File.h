#pragma once




#if defined(_WIN32)

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

#elif defined(__linux__)
#include <limits.h>
#endif