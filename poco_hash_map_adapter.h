#ifndef INCLUDE_GUARD_poco_hash_map_adapter_1399903289
#define INCLUDE_GUARD_poco_hash_map_adapter_1399903289

#include <limits>

#include "Poco/HashMap.h"

template<typename K, typename M>
class poco_hash_map_adapter
	: public Poco::HashMap<K, M>
{
public:
	size_t erase(const K& k)
	{
		Poco::HashMap<K, M>::erase(k);
		return 1; //this is cheating, Poco::HashMap::erase is void
	}
};

#endif

