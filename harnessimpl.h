#ifndef INCLUDE_GUARD_harnessimpl_1399849841
#define INCLUDE_GUARD_harnessimpl_1399849841

#include "harness.h"

#include <memory>
#include <stdexcept>
#include <typeinfo>

#include "consts.h"
#include "util.h"


template<typename HashMapType>
class HarnessImpl
: public Harness
{
public:
	HarnessImpl()
	: hashMap(new HashMapType)
	{
	}
	virtual void fill(size_t beginKey, size_t endKey)
	{
		HashMapType& hm = *hashMap;
		if (bRandomize)
			for (KEY_TYPE i = beginKey; i < (KEY_TYPE)endKey; ++i)
				hm[hash64(i)] = i;
		else
			for (KEY_TYPE i = beginKey; i < (KEY_TYPE)endKey; ++i)
				hm[i] = i;
	}
	virtual void count(size_t beginKey, size_t endKey, bool bExpectedResult)
	{
		HashMapType& hm = *hashMap;
		for (KEY_TYPE i = beginKey; i < (KEY_TYPE)endKey; ++i)
		{
			bool bFound = hm.count(hash64(i)) > 0;
			if (bFound != bExpectedResult)
				throw std::runtime_error("contains(): bad result");
		}
	}
	virtual void find(size_t beginKey, size_t endKey, bool bExpectedResult)
	{
		HashMapType& hm = *hashMap;
		for (KEY_TYPE i = beginKey; i < (KEY_TYPE)endKey; ++i)
		{
			auto it = hm.find(hash64(i));
			bool bFound = it != hm.end() && it->second.value() == i;
			if (bFound != bExpectedResult)
				throw std::runtime_error("find(): bad result");
		}
	}
/*	virtual void at(size_t beginKey, size_t endKey, bool bExpectedResult)
	{
		HashMapType& hm = *hashMap;
		for (KEY_TYPE i = beginKey; i < (KEY_TYPE)endKey; ++i)
		{
			bool bFound = hm.at(hash64(i)).value() == i;
			if (bFound != bExpectedResult)
				throw std::runtime_error("at(): bad result");
		}
	}*/
	virtual void remove(size_t beginKey, size_t endKey)
	{
		HashMapType& hm = *hashMap;
		for (KEY_TYPE i = beginKey; i < (KEY_TYPE)endKey; ++i)
		{
			if(hm.erase(hash64(i)) != 1)
				throw std::runtime_error("remove(): bad result");
		}
	}
	/*
	virtual void contains(size_t beginKey, size_t endKey, bool bExpectedResult)
	{
		HashMapType& hm = *hashMap;
		for (KEY_TYPE i = beginKey; i < (KEY_TYPE)endKey; ++i)
		{
			bool bFound = hm.contains(hash64(i)) > 0;
			if (bFound != bExpectedResult)
				throw std::runtime_error("contains(): bad result");
		}
	}
	virtual void LookupAllSequential(size_t beginKey, size_t endKey)
	{
		HashMapType& hm = *hashMap;
		for (KEY_TYPE i = 0; i < (KEY_TYPE)kMapSize; ++i)
			if (hm[i] != i)
				throw std::runtime_error("LookupAllSequential: bad value.");
	}
	virtual void RemoveAllSequential(size_t beginKey, size_t endKey)
	{
		HashMapType& hm = *hashMap;
		for (KEY_TYPE i = 0; i < (KEY_TYPE)kMapSize; ++i)
		if (hm.erase(i) != 1)
			throw std::runtime_error("RemoveAllSequential: key not found.");
	}*/
	//virtual void FillRandom() = 0;
	//virtual void LookupAllRandom() = 0;
	virtual void deleteContainer()
	{
		hashMap.reset();
	}
	virtual std::string hashMapTypeName() const
	{
		std::string s = typeid(HashMapType).name();
		size_t p = s.find_first_of('<');
		if (p != std::string::npos)
			s.erase(p, std::string::npos);
		if (s.find("class ") == 0 || s.find("struct ") == 0)
		{
			size_t p = s.find_first_of(' ');
			s.erase(0, p + 1);
		}
		return s;
	}
	virtual size_t size() const
	{
		return hashMap->size();
	}
private:
	std::unique_ptr<HashMapType> hashMap;
};


#endif

