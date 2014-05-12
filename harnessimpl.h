#ifndef INCLUDE_GUARD_harnessimpl_1399849841
#define INCLUDE_GUARD_harnessimpl_1399849841

#include "harness.h"

#include <memory>
#include <stdexcept>
#include <typeinfo>

template<typename HashMapType>
class HarnessImpl
: public Harness
{
public:
	HarnessImpl(size_t mapSize)
	: hm(new HashMapType)
	, kMapSize(mapSize)
	{
	}
	virtual void FillSequential()
	{
		for(size_t i = 0; i < kMapSize; ++i)
			(*hm)[i] = i;
		if(hm->size() != kMapSize)
			throw std::runtime_error("FillSequential: bad map size.");
	}
	virtual void LookupAllSequential()
	{
		for(size_t i = 0; i < kMapSize; ++i)
		if ((*hm)[i] != i)
			throw std::runtime_error("LookupAllSequential: bad value.");
	}
	virtual void RemoveAllSequential()
	{
		for(size_t i = 0; i < kMapSize; ++i)
		if (hm->erase(i) != 1)
			throw std::runtime_error("RemoveAllSequential: key not found.");
	}
	//virtual void FillRandom() = 0;
	//virtual void LookupAllRandom() = 0;
	virtual void deleteContainer()
	{
		hm.reset();
	}
	virtual std::string hashMapType() const
	{
		std::string s = typeid(HashMapType).name();
		size_t p = s.find_first_of('<');
		if (p != std::string::npos)
			s.erase(p, std::string::npos);
		return s;
	}
private:
	std::unique_ptr<HashMapType> hm;
	const size_t kMapSize;
};


#endif

