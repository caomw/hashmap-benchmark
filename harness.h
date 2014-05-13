#ifndef INCLUDE_GUARD_harness_1399849926
#define INCLUDE_GUARD_harness_1399849926

#include <string>

//Harness is the interface of an object that performs certain operations on a hash map
//which will then be benchmarked.
//A Harness can be created by the HarnessFactory
class Harness
{
public:
	Harness() : bRandomize(true) {}
	virtual ~Harness() {}

	void setRandomize(bool b) //to use hash(key) instead of key, default: true
	{
		bRandomize = b;
	}

	virtual void fill(size_t beginKey, size_t endKey) = 0;
	//virtual void LookupAllSequential() = 0;
	//virtual void RemoveAllSequential() = 0;
	//virtual void FillRandom() = 0;
	//virtual void LookupAllRandom() = 0;
	virtual void deleteContainer() = 0;

	virtual size_t size() const = 0;

	//base type name, without the template parameters
	virtual std::string hashMapType() const = 0;
protected:
	bool bRandomize;
};


#endif

