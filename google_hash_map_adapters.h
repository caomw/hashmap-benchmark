#ifndef INCLUDE_GUARD_google_hash_map_adapters_1399903289
#define INCLUDE_GUARD_google_hash_map_adapters_1399903289

#include <limits>

#include <sparsehash/dense_hash_map>
#include <sparsehash/sparse_hash_map>
#include "config.h"
#include "mappedtype.h"

template<typename K, typename M>
class google_dense_hash_map_adapter
	: public google::dense_hash_map<K, M>
{
public:
	google_dense_hash_map_adapter()
	{
		//needs to set special values, key must be signed
		static_assert(std::numeric_limits<K>::is_signed, "KEY_TYPE must be signed if you test google::dense_hash_map");
		set_empty_key(-1);
		set_deleted_key(-2);
	}
};

template<typename K, typename M>
class google_sparse_hash_map_adapter
	: public google::sparse_hash_map<K, M>
{
public:
	google_sparse_hash_map_adapter()
	{
		//needs to set special values, key must be signed
		static_assert(std::numeric_limits<K>::is_signed, "KEY_TYPE must be signed if you test google::sparse_hash_map");
		set_deleted_key(-2);
	}
};

#endif

