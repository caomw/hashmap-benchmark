#ifndef INCLUDE_GUARD_google_dense_hash_map_adapter_1399903289
#define INCLUDE_GUARD_google_dense_hash_map_adapter_1399903289

#include <sparsehash/dense_hash_map>
#include "consts.h"
#include "mappedtype.h"

class google_dense_hash_map_adapter
	: public google::dense_hash_map<KEY_TYPE, MappedType<KEY_TYPE, MAPPED_TYPE_SIZE>>
{
public:
	google_dense_hash_map_adapter()
	{
		set_empty_key(-1);
		set_deleted_key(-2);
	}
};

#endif

