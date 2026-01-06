#pragma once
#include <cstdio>

#ifdef DEBUG_LOG
#define LOGT(...) do { fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); } while(0)
#else
#define LOGT(...) do {} while(0)
#endif
