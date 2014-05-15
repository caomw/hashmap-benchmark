#include "dobenchmark.h"

#include <sstream>

BenchmarkResult::
	BenchmarkResult()
	: elapsedSec(0.0)
	, allocatedBytes(0)
{
}

std::string BenchmarkResult::
	toString(size_t count) const
{
	std::ostringstream ss;
	double dcount = (double)count;

	if (elapsedSec == 0.0)
		ss << "Elapsed: 0 sec";
	else
		ss << dcount / elapsedSec / 1000.0 << "k items / sec";

	ss << ", ";

	if (allocatedBytes == 0)
		ss << "0 bytes allocated";
	else if (allocatedBytes < 0)
		ss << -allocatedBytes / dcount << " bytes freed / item";
	else
		ss << allocatedBytes / dcount << " bytes allocated / item";
	return ss.str();
}

