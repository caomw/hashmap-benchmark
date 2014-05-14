#include "memtest.h"

#include <vector>
#include <map>
#include <sstream>
#include <fstream>

#include "consts.h"
#include "harnessfactory.h"
#include "getmemusage.h"
#include "embedded_html_files.h"
#include "util.h"

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
	std::vector<Harness*> hs(hf.createAll(true));
	std::map<size_t, std::vector<double>> results; // results[hashMapIdx][run]
	//initialize results array
	for (size_t i = 0; i < hs.size(); ++i)
	{
		results[i].resize(kNumRunsPerTest);
	}
	for (auto it = hs.begin(); it != hs.end(); ++it)
	{
		size_t hmidx = it - hs.begin();
		printf("Testing memory usage of %s\n", (*it)->hashMapType().c_str());
		size_t mem0 = GetMemUsage();
		size_t prevItemCount = 0;
		for (int r = 0; r < itemCounts.size(); ++r)
		{
			printf("%d..", r);
			(*it)->fill(prevItemCount, itemCounts[r]);
			prevItemCount = itemCounts[r];
			results[hmidx][r] =
				(double)(GetMemUsage() - mem0) / (*it)->size() - kItemSize;
		}
		(*it)->deleteContainer();
		printf("\n");
	}
	std::ostringstream ss;
	ss << "[";
	// header
	ss << "[ 'Raw data MB'";
	size_t nc = hs.size(); //num of cols
	size_t nr = kNumRunsPerTest; //num of rows
	for (auto it = hs.begin(); it != hs.end(); ++it)
		ss << ", '" << (*it)->hashMapType() << "'";
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

	for (auto it = hs.begin(); it != hs.end(); ++it)
		delete *it;

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