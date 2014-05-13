#ifndef INCLUDE_GUARD_harnessfactory_1399849088
#define INCLUDE_GUARD_harnessfactory_1399849088

#include <vector>

#include "harness.h"

//Create a benchmarking harness with a hash map of the specified type
//
//Key type will be as specified
//in macros.h (or as build parameters, see there)
//The mapped type will be the key type padded to the size
//of the mapped-type-size specified in macros.h or as a build parameter.
//
//It will create harnesses that do benchmarking tests
//with map size of itemCount.
class HarnessFactory
{
public:
	Harness* create_std_unordered_map(); //ownership passed
	Harness* create_std_map(); //ownership passed
	std::vector<Harness*> createAll(); //ownership passed
};


#endif

