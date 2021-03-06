#include "Error.h"

#define __DebugPrintToLogFile__(str) fprintf(log, str);
#define DEBUG_BUFFER_SIZE 512
#define DEBUG_FNAME_SIZE 64

#define NO_MSG_assert(expression) (void)(                                                       \
            (!!(expression)) ||                                                              \
            (_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0) \
        )

#if defined(DEBUG) || defined(_DEBUG) // turn VS logging features

#define __DebugPrintToDebuggerArea__(str) OutputDebugString(str)

#else // turn off full debugging features

#define __DebugPrintToDebuggerArea__(str)

#endif // Debug logging functions

void DebugLogging::initDebugLogger()
{
	// determine the filename
	if (_strcmpi(debugFileName, "") == 0)
	{
		SYSTEMTIME systemTime;
		GetSystemTime(&systemTime);
#if defined(DEBUG) || defined(_DEBUG)
		NO_MSG_assert(sprintf_s(debugFileName, DEBUG_FNAME_SIZE, "DEBUG_LOG_(%d-%d-%d_%d-%d-%d).log",
				systemTime.wMonth, systemTime.wDay, systemTime.wYear, systemTime.wHour, systemTime.wSecond, systemTime.wSecond) >= 0);
#else
		sprintf_s(debugFileName, DEBUG_BUFFER_SIZE, "DEBUG_LOG_(%d-%d-%d_%d-%d-%d).log",
			systemTime.wMonth, systemTime.wDay, systemTime.wYear, systemTime.wHour, systemTime.wSecond, systemTime.wSecond);
#endif
	}

	// open the log
#if defined(DEBUG) || defined(_DEBUG)
	NO_MSG_assert(fopen_s(&log, debugFileName, "a") == 0);
#else
	fopen_s(&log, debugFileName, "a");
#endif
	isInitialized = true;
}

void DebugLogging::dbgPrintf(const char* format, ...)
{
	// initiate the log (if not already done so)
	if (!isInitialized) initDebugLogger();

	// debug log text buffer
	char debugBuffer[DEBUG_BUFFER_SIZE];
	int result = 0;

	// construct the output string
	va_list args;
	va_start(args, format);
#if defined(DEBUG) || defined(_DEBUG)
	NO_MSG_assert(vsprintf_s(debugBuffer, format, args) >= 0);
#else
	vsprintf_s(debugBuffer, format, args);
#endif
	va_end(args);

	// write it to the correct areas
	__DebugPrintToDebuggerArea__(debugBuffer);
	__DebugPrintToLogFile__(debugBuffer);
}

void DebugLogging::finishDebugLogger()
{
	// return standard output to its normal location
#if defined(DEBUG) || defined(_DEBUG)
	NO_MSG_assert(fclose(log) == 0);
#else
	fclose(log);
#endif 
	isInitialized = false;
}

// macro cleanup
#undef DEBUG_FILE_NAME
#undef DEBUG_BUFFER_SIZE
#undef __DebugPrintToDebuggerArea__
#undef __DebugPrintToLogFile__
