#include "speedtest.h"

#include <vector>

#include "consts.h"

void speedtest()
{
	std::vector<size_t> itemCounts;

	size_t itemCount_max = (size_t)((double)(kSpeedTest_rawDataMBytes_max)* 1024 * 1024 / kItemSize);
	double ex_min = log10((double)kSpeedTest_itemCount_min);
	double ex_max = log10((double)itemCount_max);
	double dex = (ex_max - ex_min) / kNumRunsPerTest;
	for (int i = 0; i < kNumRunsPerTest; ++i)
	{
		double ex = ex_min + i * dex;
		itemCounts.push_back((size_t)pow(10, ex));
	}
#if 0
	HarnessFactory hf;

	//space graphs
	std::vector<Harness*> hs(hf.createAll());
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
#endif
}