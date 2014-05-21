#ifndef INCLUDE_GUARD_harnessfactory_1399849088
#define INCLUDE_GUARD_harnessfactory_1399849088

#include <vector>
#include <string>

#include "harness.h"

//Create a benchmarking harness with a hash map of different types
//
//The key type of the hash map is set in config.h (or as build parameters, see there)
//The mapped type of the hash map is the key type padded to the size
//set in config.h or as a build parameter.
class HarnessFactory
{
public:
	enum MapId {
		M_FIRST,
		M_STD_MAP = M_FIRST,
		M_BOOST_MAP,
		M_STD_UNORDERED_MAP,
		M_BOOST_UNORDERED_MAP,
		M_GOOGLE_SPARSE_MAP,
		M_GOOGLE_DENSE_MAP,
		M_LOKI_ASSOCVECTOR,
		M_BOOST_FLAT_MAP,
		M_LAST = M_LOKI_ASSOCVECTOR,
		M_COUNT,
	};
	HarnessFactory();

	size_t mapCount() const; //valid indices will be [0..mapCount)
	std::string mapTypeNameByIdx(size_t idx) const;

	Harness* createById(MapId id); //ownership passed
	Harness* createByIdx(size_t idx); //ownership passed

private:
	struct Map
	{
		std::string typeName;
		MapId id;
	};
	std::vector<Map> maps;
};


#endif

