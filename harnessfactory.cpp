#include "harnessfactory.h"

#include <map>
#include <unordered_map>

#include "macros.h"
#include "mappedtype.h"

#include "harnessimpl.h"


HarnessFactory::
	HarnessFactory(size_t s)
: itemCount(s)
{}

void HarnessFactory::
	setItemCount(size_t s)
{
	itemCount = s;
}

Harness* HarnessFactory::
	create_std_unordered_map()
{
	typedef std::unordered_map<KEY_TYPE, MappedType<KEY_TYPE, MAPPED_TYPE_SIZE>> HashMapType;
	return new HarnessImpl<HashMapType>(itemCount);
}

Harness* HarnessFactory::
create_std_map()
{
	typedef std::map<KEY_TYPE, MappedType<KEY_TYPE, MAPPED_TYPE_SIZE>> HashMapType;
	return new HarnessImpl<HashMapType>(itemCount);
}

std::vector<Harness*> HarnessFactory::
	createAll()
{
	std::vector<Harness*> v;
	v.push_back(create_std_unordered_map());
	v.push_back(create_std_map());
	return v;
}
