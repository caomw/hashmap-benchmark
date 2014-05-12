#ifndef INCLUDE_GUARD_harness_1399849926
#define INCLUDE_GUARD_harness_1399849926

#include <string>

//Harness is the interface of an object that performs certain operations on a hash map
//which will then be benchmarked.
//A Harness can be created by the HarnessFactory
class Harness
{
public:
	virtual void FillSequential() = 0;
	virtual void LookupAllSequential() = 0;
	virtual void RemoveAllSequential() = 0;
	//virtual void FillRandom() = 0;
	//virtual void LookupAllRandom() = 0;
	virtual void deleteContainer() = 0;

	//base type name, without the template parameters
	virtual std::string hashMapType() const = 0;

	virtual ~Harness() {}
};


#endif

