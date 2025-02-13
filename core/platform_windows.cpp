#include "platform.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h> // for OutputDebugString
#else
#include <stdio.h> // for printf
#endif

#pragma warning(disable:4996) // sprintf is unsafe

static FILE* s_log_file;

void logInit(const char* filename) {
	s_log_file = fopen(filename, "w");
}
void logTerm() {
	fflush(s_log_file);
	fclose(s_log_file);
}

void print(const char * format, ...) {
	va_list args;
	va_start(args, format);

	static char buff[PRINT_MAX_CHARACTERS + 2]; // +1 for newline, +1 for nullterm. 

#ifdef _WIN32
	int c = vsprintf_s(buff, PRINT_MAX_CHARACTERS, format, args);
	buff[c]   = '\n';
	buff[c+1] = 0;
	OutputDebugString(buff);
#else
	int c = vsnprintf(buff, , PRINT_MAX_CHARACTERS, format, args);
	buff[c]   = '\n';
	buff[c+1] = 0;
	printf("%s", buff);
#endif

	va_end(args);
}

void printLog(const char * format, ...) {
	va_list args;
	va_start(args, format);

	static char buff[PRINT_MAX_CHARACTERS + 2]; // +1 for newline, +1 for nullterm. 

#ifdef _WIN32
	int c = vsprintf_s(buff, PRINT_MAX_CHARACTERS, format, args);
	buff[c]   = '\n';
	buff[c+1] = 0;
	OutputDebugString(buff);
	if (s_log_file) {
		fwrite(buff, c+1, 1, s_log_file);
		fflush(s_log_file);
	}
#else
	int c = vsnprintf(buff, , PRINT_MAX_CHARACTERS, format, args);
	buff[c]   = '\n';
	buff[c+1] = 0;
	printf("%s", buff);
	if (s_log_file) {
		fwrite(buff, c+1, 1, s_log_file);
		fflush(s_log_file);
	}
#endif

	va_end(args);
}

#if 0 // maybe

void printError(const char* system, int code, const char * format, ...) {
	va_list args;
	va_start(args, format);

	static char buff[4096]; 
	char* str = buff;
	int64_t t = timeCounterSinceStart();
	int64_t f = osTimerFrequency();

	str += sprintf(str, "%lld.%lld [ ERROR ] [%s]", t / f, t%f, system);
	if (code)
		str += sprintf(str, " [code %d (0x%08x)]", code, code);
	str += sprintf(str, ": '");
	str += vsprintf(str, format, args);
	str += sprintf(str, "'\n");
#ifdef _WIN32
	OutputDebugString(buff);
#else
	printf("%s", buff);
#endif

	va_end(args);
	}
void printInfo(const char* system, const char * format, ...) {
	va_list args;
	va_start(args, format);

	static char buff[4096]; 
	char* str = buff;
	int64_t t = timeCounterSinceStart();
	int64_t f = osTimerFrequency();
	str += sprintf(str, "%lld.%lld [ INFO  ] [%s]: ", t/f, t%f, system);
	str += vsprintf(str, format, args);
	str += sprintf(str, "\n");
#ifdef _WIN32
	OutputDebugString(buff);
#else
	printf("%s", buff);
#endif

	va_end(args);
}

#endif