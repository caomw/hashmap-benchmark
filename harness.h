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

	//to use hash(key) instead of key, default: true
	//NOTE: affects only fill()!
	void setRandomize(bool b)
	{
		bRandomize = b;
	}

	virtual void fill(size_t beginKey, size_t endKey) = 0;
	virtual void count(size_t beginKey, size_t endKey, bool bExpectedResult) = 0; //bExpectedResult controls if we must or must not find the key
	virtual void find(size_t beginKey, size_t endKey, bool bExpectedResult) = 0; //bExpectedResult controls if we must or must not find the key
	virtual void remove(size_t beginKey, size_t endKey) = 0;
	virtual void deleteContainer() = 0;

	virtual size_t size() const = 0;

	//base type name, without the "class " and template parameters
	virtual std::string hashMapTypeName() const = 0;
protected:
	bool bRandomize;
};


#endif

