#ifndef INCLUDE_GUARD_consts_1399849207
#define INCLUDE_GUARD_consts_1399849207

#include <cstddef>

#include "mappedtype.h"

#define KEY_TYPE ptrdiff_t //should be signed, see google_dense_hash_map_adapter.h
#define MAPPED_TYPE_SIZE sizeof(KEY_TYPE)

//mem test will test hash maps with data size in this range
const size_t kMemTest_rawDataMBytes_min = 100;
const size_t kMemTest_rawDataMBytes_max = 200;

//multiple-element-count speed test will test in this range
//single-element-count speed test will test at max
const size_t kSpeedTest_rawDataMBytes_min = 20;
const size_t kSpeedTest_rawDataMBytes_max = 200;

//One test may run for different item counts
//(hash map sizes). The item count will be the x-axis.
//This value determines the resolition along x-axis.
const size_t kNumRunsPerTest = 12;

// ---- //

//kItemSize could be simply sizeof(KEY_TYPE) + MAPPED_TYPE_SIZE
//but it's safer this way.
//(a compiler may add extra padding to MappedType)
const size_t kItemSize =
sizeof(KEY_TYPE)+sizeof(MappedType<KEY_TYPE, MAPPED_TYPE_SIZE>);

#endif

