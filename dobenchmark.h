#ifndef INCLUDE_GUARD_dobenchmark_1399851869
#define INCLUDE_GUARD_dobenchmark_1399851869

#include <ctime>
#include <string>

#include "getmemusage.h"

//Calls a member function (void mf(void)) of an object
//Measures time and memory usage

struct BenchmarkResult
{
	BenchmarkResult();

	double elapsedSec;
	ptrdiff_t allocatedBytes; //negative if freed

	//return formatted result, amortize the time and memory
	//among count items
	std::string toString(size_t count = 1) const;
};

double TimeStamp();

template<typename T>
BenchmarkResult DoBenchmark(T& t, void (T::*mf)())
{
	BenchmarkResult br;
	br.allocatedBytes = GetMemUsage();
	double t0 = TimeStamp();
	(t.*mf)();
	br.elapsedSec = TimeStamp() - t0;
	br.allocatedBytes = GetMemUsage() - br.allocatedBytes;
	return br;
}

#endif

