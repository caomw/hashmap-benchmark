#include "harnessfactory.h"

#if HAVE_BOOST
#include <boost/unordered_map.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/container/map.hpp>
#endif

#include <map>
#include <unordered_map>

#include "google_hash_map_adapters.h"

#include "loki/AssocVector.h"

#include "config.h"
#include "mappedtype.h"
#include "harnessimpl.h"

HarnessFactory::
	HarnessFactory()
{
	for (int id = M_FIRST; id <= M_LAST; ++id)
	{
		std::unique_ptr<Harness> p(createById((MapId)id));
		if (p)
		{
			Map m;
			m.id = (MapId)id;
			m.typeName = p->hashMapTypeName();
			maps.push_back(m);
		}
	}
}

Harness* HarnessFactory::
	createById(MapId id)
{
	typedef KEY_TYPE KT;
	typedef MappedType<KEY_TYPE, MAPPED_TYPE_SIZE> MT;

	switch (id)
	{
	case M_STD_UNORDERED_MAP:
		return new HarnessImpl<std::unordered_map<KT, MT>>;
	case M_STD_MAP:
		return new HarnessImpl<std::map<KT, MT>>;
	case M_BOOST_UNORDERED_MAP:
#if HAVE_BOOST
		return new HarnessImpl<boost::unordered_map<KT, MT>>;
#else
		return nullptr;
#endif
	case M_BOOST_MAP:
	{
//boost::container::map 1.55.0 doesn't work with vs2013
#if defined HAVE_BOOST && (!defined(_MSC_VER) || _MSC_VER < 1800)
		return new HarnessImpl<boost::container::map<KT, MT>>;
#else
		return nullptr;
#endif
	}
	case M_BOOST_FLAT_MAP:
#if defined HAVE_BOOST && (!defined(_MSC_VER) || _MSC_VER < 1800)
		return new HarnessImpl<boost::container::flat_map<KT, MT>>;
#else
		return nullptr;
#endif
	case M_GOOGLE_SPARSE_MAP:
		return new HarnessImpl<google_sparse_hash_map_adapter<KT, MT>>;
	case M_GOOGLE_DENSE_MAP:
		return new HarnessImpl<google_dense_hash_map_adapter<KT, MT>>;
	case M_LOKI_ASSOCVECTOR:
	{
		Harness *p = new HarnessImpl<Loki::AssocVector<KT, MT>>;
		p->setRandomize(false); //don't randomize to speed up insertion
		return p;
	}
	default:
		return nullptr;
	}
}

size_t HarnessFactory::
	mapCount() const
{
	return maps.size();
}

std::string HarnessFactory::
	mapTypeNameByIdx(size_t idx) const
{
	return maps[idx].typeName;
}

Harness* HarnessFactory::
	createByIdx(size_t idx)
{
	return createById(maps[idx].id);
}
