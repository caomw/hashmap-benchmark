#include "timer.h"

#ifdef HAVE_BOOST
#include <boost/chrono/system_clocks.hpp>
#endif

double TimeStamp()
{
#if defined(HAVE_BOOST)
	typedef boost::chrono::high_resolution_clock Clock;
	Clock::time_point t = Clock::now();
	return t.time_since_epoch().count() * ((double)Clock::period::num / Clock::period::den);
#else
	return clock() / (double)CLOCKS_PER_SEC;
#endif
}