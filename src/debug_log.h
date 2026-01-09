#pragma once
#include <cstdio>

#ifdef DEBUG_LOG
#define DEBUG(...) do { fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); } while(0)
#else
#define DEBUG(...) do {} while(0)
#endif
