#ifndef INCLUDE_GUARD_harnessfactory_1399849088
#define INCLUDE_GUARD_harnessfactory_1399849088

#include <vector>

#include "harness.h"

//Create a benchmarking harness with a hash map of the specified type
//
//Key type will be as specified
//in consts.h (or as build parameters, see there)
//The mapped type will be the key type padded to the size
//of the mapped-type-size specified in consts.h or as a build parameter.
//
//It will create harnesses that do benchmarking tests
//with map size of itemCount.
class HarnessFactory
{
public:
	//Loki::AssocVector can be included for the mem test
	//as a best-case reference
	std::vector<Harness*> createAll(bool bAddLokiAssocVector); //ownership passed
};


#endif

