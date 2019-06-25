#include "log.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h> // for OutputDebugString
#else
#include <stdio.h> // for printf
#endif

void log(const char * format, ...) {
	va_list args;
	va_start(args, format);

	static char buff[4096]; 
	
#ifdef _WIN32
	vsprintf_s(buff, format, args);
	OutputDebugString(buff);
#else
	vsnprintf(buff, sizeof(buff), format, args);
	printf("%s", buff);
#endif

	va_end(args);
}

namespace {
struct Log
{
	char* text = NULL;
	char* cursor = NULL;
	long capacity = 0;
	void makeroom()
	{
		if (!text)
		{
			capacity = 4096;
			text = (char*)malloc(capacity);
			cursor = text;
		}
		if (length() * 2 > capacity)
		{
			capacity = capacity * 2;
			text = (char*)realloc(text, capacity);
			cursor = text + length();
		}
	}
	long length()
	{
		return cursor - text;
	}
};
};
