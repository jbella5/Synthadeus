////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   Error Catching Helpers                                                   //
//   Everett Moser                                                            //
//   9-8-15                                                                   //
//                                                                            //
//   Definitions for assert, DebugPrintf, and DEBUG_ONLY                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stdio.h>
#include <Windows.h>

#define DebugPrintf DebugLogging::dbgPrintf

/////////////////////
// Assert function //
/////////////////////

#if defined(DEBUG) || defined(_DEBUG) // turn on assert breaks

#include <assert.h>
#undef assert
#define assert(expression) \
	if (!expression) \
	{ \
		DebugPrintf("Failed assertion: " #expression "\n"); \
		_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)); \
	}

#else // turn off assert breaks

#define assert(exp) if (!(exp)) { DebugPrintf("Failed assertion: " #exp "\n"); }

#endif // Assert functions

///////////////////////////////
// Debug segmenting function //
///////////////////////////////
#if defined(DEBUG) || defined(_DEBUG) // turn on assert breaks

#define DEBUG_ONLY

#else // turn off assert breaks

#define DEBUG_ONLY if (false) 

#endif // Assert functions

/////////////////////////////
// Debug logging functions //
/////////////////////////////
#define DEBUG_FNAME_SIZE 32

namespace DebugLogging
{
	static char debugFileName[DEBUG_FNAME_SIZE] = "\0";
	static FILE* log = NULL;
	static bool isInitialized = false;

	// start and stop the logger
	void initDebugLogger();
	void finishDebugLogger();

	// auto log-creating debug printing function
	void dbgPrintf(const char* format, ...);
}

// macro cleanup
#undef DEBUG_FNAME_SIZE
