#pragma once
#include <stdlib.h> //for abort
#include <stdint.h>

typedef int64_t Time;

#define ARRSIZE(x) (sizeof(x) / (sizeof(x[0])))

#define PRINT_MAX_CHARACTERS 4096

void logInit(const char* filename);
void logTerm();

// Print a debug message to the console, including a newline. Not printed to the app's log. 
void print(const char *format, ...);

// Print a message to the log and also to the console, including a newline.
void printLog(const char *format, ...);

#define ASSERT(cond) \
        if (!(cond)) { \
          print("HALT: Assert '%s' failed (in file %s on line %d)", #cond, __FILE__, __LINE__); \
          abort(); \
        }

#define ASSERTM(cond, ...) \
        if (!(cond)) { \
          print("HALT: Assert '%s' failed (in file %s on line %d):", #cond, __FILE__, __LINE__); \
          print(__VA_ARGS__); \
          abort(); \
        }

/*---------------------------------------------------*
 | Time. Needs to be inline for performance.         |
 *---------------------------------------------------*/

#ifdef _WIN32
#include <Windows.h> // for QueryPerformanceCounter and QueryPerformanceFrequency

static __inline int64_t osTimer() {
	LARGE_INTEGER ticks;
	QueryPerformanceCounter(&ticks);
	return (int64_t)ticks.QuadPart;
}
static __inline int64_t osTimerFrequency() {
	LARGE_INTEGER ticksPerSecond;
	QueryPerformanceFrequency(&ticksPerSecond);
	return (int64_t)ticksPerSecond.QuadPart;
}

#else
#include <time.h>     /* For clock_gettime, CLOCK_MONOTONIC */
#include <string.h>   /* For memcpy */
static __inline int64_t time_counter() {
	struct timespec ts1;
	if (clock_gettime(CLOCK_MONOTONIC, &ts1) != 0) {
		ASSERT("Can't clock :( where's my clock?");
		return -1L;
	}
	return ts1.tv_nsec;
}
static __inline int64_t time_frequency() {
	return 1000000000L;
}
#endif

