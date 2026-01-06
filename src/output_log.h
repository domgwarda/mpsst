#pragma once
#include <cstdio>

#ifdef NO_OUTPUT
  #define OUTPUT_printf(...) do {} while(0)
  #define OUTPUT_stream(x) do {} while(0)
#else
  #define OUTPUT_printf(...) do { std::printf(__VA_ARGS__); } while(0)
  #define OUTPUT_stream(x) do { x; } while(0)
#endif