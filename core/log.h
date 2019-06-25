#pragma once

void log(const char *format, ...);
void logImGuiPrint();

#define ARRSIZE(x) (sizeof(x) / (sizeof(x[0])))

#include <stdlib.h> //for abort

#ifdef assert
#undef assert
#endif

#define dmsg(...) \
      { log(__VA_ARGS__); \
        log("\n"); \
      }

#define assert(cond) \
        if (!(cond)) { \
          log("HALT: Assert '%s' failed (in file %s on line %d)\n", #cond, __FILE__, __LINE__); \
          abort(); \
        }

#define assertm(cond, ...) \
        if (!(cond)) { \
          log("HALT: Assert '%s' failed (in file %s on line %d):\n", #cond, __FILE__, __LINE__); \
          log(__VA_ARGS__); \
          abort(); \
        }

#define ensure(cond) \
        if (!(cond)) { \
          log("HALT: Ensure '%s' failed (in file %s on line %d)\n", #cond, __FILE__, __LINE__); \
          abort(); \
        }
