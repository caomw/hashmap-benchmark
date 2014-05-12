#include "getmemusage.h"

#ifdef WIN32

#pragma comment(lib, "psapi.lib")

#include <Windows.h>
#include <Psapi.h>

size_t GetMemUsage()
{
	HANDLE Process = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS_EX p;
	BOOL b = GetProcessMemoryInfo(Process, (PPROCESS_MEMORY_COUNTERS)&p, sizeof(p));
	return b ? p.PrivateUsage : 0;
}

#else

size_t GetMemUsage()
{
	// unimplemented. used only for profiling
	return 0;
}

#endif
