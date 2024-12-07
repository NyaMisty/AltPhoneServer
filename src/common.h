#pragma once

#include <stdint.h>

typedef int HWND;
typedef int HINSTANCE;

#define DEBUG_PRINT(...) do { \
        fprintf(stderr, "[%s:%d] ", __FUNCTION__, __LINE__); \
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\n"); \
    } while (0)

#include <stdlib.h>
#include <unistd.h>

#define closesocket close
typedef struct timeval TIMEVAL;
typedef int BOOL;

#ifdef __cplusplus
#include <algorithm>
#include <vector>
using std::min;
#endif