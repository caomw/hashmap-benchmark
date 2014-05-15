#include "speedtest.h"

#include <vector>
#include <map>
#include <memory>

#include "consts.h"
#include "harnessfactory.h"
#include "timer.h"

struct SpeedResult
{
	SpeedResult()
		: fillRate(0)
		, countExistingRate(0), findExistingRate(0)
		, countMissingRate(0), findMissingRate(0)
		, removeRate(0), deleteRate(0)
	{}
	double fillRate;
	double countMissingRate;
	double findMissingRate;
	double countExistingRate;
	double findExistingRate;
	double removeRate;
	double deleteRate;
};

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
	HarnessFactory hf;

	size_t nMaps = hf.mapCount();
	std::vector<std::vector<SpeedResult>> results; // results[hashMapIdx][run]
	//initialize results array
	results.resize(nMaps);
	for (size_t i = 0; i < nMaps; ++i)
	{
		results[i].resize(kNumRunsPerTest);
	}
	for (size_t hmidx = 0; hmidx < nMaps; ++hmidx)
	{
		printf("Testing speed of %s\n", hf.mapTypeNameByIdx(hmidx).c_str());
		for (size_t r = 0; r < kNumRunsPerTest; ++r)
		{
			std::unique_ptr<Harness> hm(hf.createByIdx(hmidx));

			//test fill
			double tic = TimeStamp();
			hm->fill(0, itemCounts[r]);
			double dt = TimeStamp() - tic;
			size_t s = hm->size();
			results[hmidx][r].fillRate = s / dt;

			//test count() existing
			tic = TimeStamp();
			hm->count(0, itemCounts[r], true);
			dt = TimeStamp() - tic;
			results[hmidx][r].countExistingRate = s / dt;

			//test count() missing
			tic = TimeStamp();
			hm->count(0 + itemCounts[r], itemCounts[r] + itemCounts[r], false);
			dt = TimeStamp() - tic;
			results[hmidx][r].countMissingRate = s / dt;

			//test find() existing
			tic = TimeStamp();
			hm->find(0, itemCounts[r], true);
			dt = TimeStamp() - tic;
			results[hmidx][r].findExistingRate = s / dt;

			//test find() missing
			tic = TimeStamp();
			hm->find(0 + itemCounts[r], itemCounts[r] + itemCounts[r], false);
			dt = TimeStamp() - tic;
			results[hmidx][r].findMissingRate = s / dt;

			//test 
		}

		size_t prevItemCount = 0;
		for (int r = 0; r < itemCounts.size(); ++r)
		{
			printf("%d..", r);
			hm->fill(prevItemCount, itemCounts[r]);
			prevItemCount = itemCounts[r];
//			results[hmidx][r] =
	//			(double)(GetMemUsage() - mem0) / (*it)->size() - kItemSize;
		}
		hm->deleteContainer();
		printf("\n");
	}
	/*
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
*/
}