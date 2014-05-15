#include "memtest.h"

#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <memory>

#include "consts.h"
#include "harnessfactory.h"
#include "getmemusage.h"
#include "embedded_html_files.h"
#include "util.h"

#include <unordered_map>

void memtest()
{
	std::vector<size_t> itemCounts;

	size_t kTestItemCountDiff = (size_t)((double)(kMemTest_rawDataMBytes_max - kMemTest_rawDataMBytes_min) * 1024 * 1024 / kItemSize);
	size_t dItems = kTestItemCountDiff / kNumRunsPerTest;
	size_t count_from = (size_t)(kMemTest_rawDataMBytes_min * 1024.0 * 1024.0 / kItemSize);
	for (int r = 0; r < kNumRunsPerTest; ++r)
		itemCounts.push_back((r + 1) * dItems + count_from);

	HarnessFactory hf;

	//space graphs
	size_t nMaps = hf.mapCount();
	std::map<size_t, std::vector<double>> results; // results[hashMapIdx][run]
	//initialize results array
	for (size_t i = 0; i < nMaps; ++i)
	{
		results[i].resize(kNumRunsPerTest);
	}
	for (size_t hmidx = 0; hmidx < nMaps; ++hmidx)
	{
		printf("Testing memory usage of %s\n", hf.mapTypeNameByIdx(hmidx).c_str());
		std::unique_ptr<Harness> hm(hf.createByIdx(hmidx));
		size_t mem0 = GetMemUsage();
		size_t prevItemCount = 0;
		for (int r = 0; r < itemCounts.size(); ++r)
		{
			printf("%d..", r);
			hm->fill(prevItemCount, itemCounts[r]);
			prevItemCount = itemCounts[r];
			results[hmidx][r] =
				(double)(GetMemUsage() - mem0) / hm->size() - kItemSize;
		}
		printf("\n");
	}
	std::ostringstream ss;
	ss << "[";
	// header
	ss << "[ 'Raw data MB'";
	size_t nc = nMaps; //num of cols
	size_t nr = kNumRunsPerTest; //num of rows
	for (size_t hmidx = 0; hmidx < nMaps; ++hmidx)
		ss << ", '" << hf.mapTypeNameByIdx(hmidx) << "'";
	ss << "]\n";
	// body
	for (size_t r = 0; r < nr; ++r)
	{
		ss << ",[" << itemCounts[r];
		for (size_t c = 0; c < nc; ++c)
		{
			ss << "," << results[c][r];
		}
		ss << "]\n";
	}
	ss << "]";

	//insert dataTable into line chart

	//get line chart template
	int l;
	const char* p = embedded_html_files("linechart.html", &l);
	std::string linechart(p, l);

	replace_string(linechart, "${ARRAY}", ss.str());
	std::ostringstream itemsize_string;
	itemsize_string << kItemSize;
	replace_string(linechart, "${ITEMSIZE}", itemsize_string.str());

	//write out
	std::ofstream ofs("hash_maps_memtest_linechart.html");
	ofs << linechart;
}