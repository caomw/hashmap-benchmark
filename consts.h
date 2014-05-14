#ifndef INCLUDE_GUARD_consts_1399849207
#define INCLUDE_GUARD_consts_1399849207

#include <cstdlib>

#include "mappedtype.h"

#ifdef BUILD_PARAM_KEY_TYPE
#define KEY_TYPE BUILD_PARAM_KEY_TYPE
#else
#define KEY_TYPE ptrdiff_t
#endif

#ifdef BUILD_PARAM_MAPPED_TYPE_SIZE
#define MAPPED_TYPE_SIZE BUILD_PARAM_MAPPED_TYPE_SIZE
#else
#define MAPPED_TYPE_SIZE sizeof(KEY_TYPE)
#endif

//kItemSize could be simply sizeof(KEY_TYPE) + MAPPED_TYPE_SIZE
//but it's safer this way.
//(a compiler may add extra padding to MappedType)
const size_t kItemSize =
	sizeof(KEY_TYPE) + sizeof(MappedType<KEY_TYPE, MAPPED_TYPE_SIZE>);

//mem test will test hash maps with data size between these values
const size_t kMemTest_rawDataMBytes_min = 40;
const size_t kMemTest_rawDataMBytes_max = 80;

//speed test will test from this itemCount up to this data size
const size_t kSpeedTest_itemCount_min = 1000;
const size_t kSpeedTest_rawDataMBytes_max = 80;

//One test will run for different item counts
//(hash map sizes). The item count will be the x-axis.
//This value determines the resolition along x-axis.
const size_t kNumRunsPerTest = 33;

#endif

