#ifndef INCLUDE_GUARD_harnessimpl_1399849841
#define INCLUDE_GUARD_harnessimpl_1399849841

#include "harness.h"

#include <memory>
#include <stdexcept>
#include <typeinfo>

#include "config.h"
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
		for (KEY_TYPE i = beginKey; i < (KEY_TYPE)endKey; ++i)
			hm[transformKey(i)] = i;
	}
	virtual void count(size_t beginKey, size_t endKey, bool bExpectedResult)
	{
		HashMapType& hm = *hashMap;
		for (KEY_TYPE i = beginKey; i < (KEY_TYPE)endKey; ++i)
		{
			bool bFound = hm.count(transformKey(i)) > 0;
			if (bFound != bExpectedResult)
				throw std::runtime_error("contains(): bad result");
		}
	}
	virtual void find(size_t beginKey, size_t endKey, bool bExpectedResult)
	{
		HashMapType& hm = *hashMap;
		for (KEY_TYPE i = beginKey; i < (KEY_TYPE)endKey; ++i)
		{
			auto it = hm.find(transformKey(i));
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
			bool bFound = hm.at(transformKey(i)).value() == i;
			if (bFound != bExpectedResult)
				throw std::runtime_error("at(): bad result");
		}
	}*/
	virtual void remove(size_t beginKey, size_t endKey, bool bReverse)
	{
		if (beginKey > endKey)
			throw std::runtime_error("remove(): bad range");

		HashMapType& hm = *hashMap;
		if (!bReverse)
			for (size_t i = beginKey; i < endKey; ++i)
			{
				if (hm.erase(transformKey(i)) != 1)
					throw std::runtime_error("remove(): bad result");
			}
		else
		{
			size_t i = endKey;
			while (i != beginKey)
			{
				--i;
				if (hm.erase(transformKey(i)) != 1)
					throw std::runtime_error("remove(): bad result");
			}
		}
	}
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

