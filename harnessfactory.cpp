#include "harnessfactory.h"

#if HAVE_BOOST
#include <boost/unordered_map.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/container/map.hpp>
#endif

#include <map>
#include <unordered_map>

#include "google_dense_hash_map_adapter.h"
#include <sparsehash/sparse_hash_map>

#include "loki/AssocVector.h"

#include "macros.h"
#include "mappedtype.h"
#include "harnessimpl.h"

std::vector<Harness*> HarnessFactory::
	createAll()
{
	typedef KEY_TYPE KT;
	typedef MappedType<KEY_TYPE, MAPPED_TYPE_SIZE> MT;

	std::vector<Harness*> v;

	typedef std::unordered_map<KT, MT> StdUM;
	typedef std::map<KT, MT> StdM;

	v.push_back(new HarnessImpl<StdUM>);
	v.push_back(new HarnessImpl<StdM>);

#if HAVE_BOOST
	typedef boost::unordered_map<KT, MT> BoostUM;
	v.push_back(new HarnessImpl<BoostUM>);
#if !defined(_MSC_VER) || _MSC_VER < 1800
	typedef boost::container::map<KT, MT> BoostM;
	typedef boost::container::flat_map<KT, MT> BoostFM;
	v.push_back(new HarnessImpl<BoostFM>);
	v.push_back(new HarnessImpl<BoostM>);
#endif
#endif

	typedef google::sparse_hash_map<KT, MT> GoogleSHM;
	v.push_back(new HarnessImpl<google_dense_hash_map_adapter>);
	v.push_back(new HarnessImpl<GoogleSHM>);

	typedef Loki::AssocVector<KT, MT> LokiAV;

	v.push_back(new HarnessImpl<LokiAV>);
	v.back()->setRandomize(false);

	return v;
}
