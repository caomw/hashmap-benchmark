#include "bitset_quick_find_false.h"
#include <set>
#include <cassert>
#include <random>

size_t randint(size_t lo, size_t hi)
{
	std::default_random_engine dre;
	std::uniform_int_distribution<size_t> uniform_dist(lo, hi);
	return uniform_dist(dre);
}

int main()
{
	bitset_quick_find_false<false> b;

	size_t n = 1000;


	std::set<size_t> allocated_idcs, free_idcs;
	size_t max_alloc_idx = ~(size_t)0;

	for (size_t i = 0; i < n; ++i)
	{
		if ((rand() & 3) == 0)
		{
			if (allocated_idcs.empty())
				continue;
			size_t maxidx = *allocated_idcs.rbegin();
			//free
			size_t randidx = randint(0, maxidx);
			auto it = allocated_idcs.lower_bound(randidx);
			if (it == allocated_idcs.end())
				continue;
			randidx = *it;
			b.reset(randidx);
			allocated_idcs.erase(it);
			free_idcs.insert(randidx);
		}
		else
		{
			size_t expected_first_false;
			if (free_idcs.empty())
				expected_first_false = 0;
			else
				expected_first_false = *free_idcs.begin();

			//alloc
			size_t x = b.find_first_false();
			assert(x == expected_first_false);
			allocated_idcs.insert(x);
			free_idcs.erase(x);
			b.set(x);
		}
	}

	return 0;
}