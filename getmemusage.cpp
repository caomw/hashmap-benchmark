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

#include <mach/mach.h>

size_t GetMemUsage()
{
    struct mach_task_basic_info ti;
    memset(&ti, 0, sizeof(ti));
    mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;
    kern_return_t kr = task_info(mach_task_self(), MACH_TASK_BASIC_INFO,
                                 (task_info_t) &ti, &count);
	
    return kr == KERN_SUCCESS ? ti.virtual_size : 0;
}

#endif
