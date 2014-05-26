#include "speedtest.h"

#include <cassert>
#include <vector>
#include <map>
#include <memory>
#include <sstream>
#include <fstream>
#include <cmath>

#include "config.h"
#include "harnessfactory.h"
#include "timer.h"
#include "embedded_html_files.h"
#include "util.h"

struct SpeedResult
{
	enum Rates {
		R_FILL, R_FIND_MISSING, R_FIND_EXISTING, R_DELETE, R_REMOVE, R_MAX
	};

	static const char* names[R_MAX];
	SpeedResult()
	{
		for (int i = 0; i < R_MAX; ++i)
			rates[i] = 0;
	}
	double rates[R_MAX];
};

const char* rateTitles[SpeedResult::R_MAX] =
{
	"Fill rate (map<>::insert())",
	"Find-missing-elements rate (map<>::find())",
	"Find-existing-elements rate (map<>::find())",
	"Delete pre-filled map rate (map<>::~map<>)"
	"Erase single element rate (map<>::erase())",
};

const char* SpeedResult::
names[SpeedResult::R_MAX] =
{
	"fill",
	"find-missing", "find-existing",
	"delete", "remove"
};

void speedtest(SpeedTestMethod spm)
{
	std::vector<size_t> itemCounts;

	bool bSingleChart = spm == SP_SINGLE_ELEMENT_COUNT;
	
	size_t itemCount_max = (size_t)((double)(kSpeedTest_rawDataMBytes_max)* 1024 * 1024 / kItemSize);
	if (bSingleChart)
	{
		itemCounts.push_back(itemCount_max);
	}
	else
	{
		size_t itemCount_min = (size_t)((double)(kSpeedTest_rawDataMBytes_min)* 1024 * 1024 / kItemSize);
		size_t itemCount_max = (size_t)((double)(kSpeedTest_rawDataMBytes_max)* 1024 * 1024 / kItemSize);
		double ex_min = log10((double)itemCount_min);
		double ex_max = log10((double)itemCount_max);
		double dex = (ex_max - ex_min) / kNumRunsPerTest;
		for (int i = 0; i < kNumRunsPerTest; ++i)
		{
			double ex = ex_min + i * dex;
			itemCounts.push_back((size_t)pow(10, ex));
		}
	}

	HarnessFactory hf;

	size_t nMaps = hf.mapCount();
	
	//initialize results array
	std::vector< std::vector<SpeedResult> > results; // results[hashMapIdx][run]
	results.resize(nMaps, std::vector<SpeedResult>(itemCounts.size()));

	for (size_t hmidx = 0; hmidx < nMaps; ++hmidx)
	{
		printf("Testing speed of %s\n", hf.mapTypeNameByIdx(hmidx).c_str());

		std::unique_ptr<Harness> hm;
		size_t hm_size = 0;
		for (size_t r = 0; r < itemCounts.size(); ++r)
		{
			printf("# of elems: %u ", (unsigned)itemCounts[r]);

			for (int t = 0; t < SpeedResult::R_MAX; ++t)
			{
				printf(".");
				if (t == SpeedResult::R_FILL)
				{
					//create new, empty map before FILL test
					//other tests use the pre-filled maps filled in the FILL test
					hm.reset(hf.createByIdx(hmidx));
				}
				else if (t == SpeedResult::R_REMOVE)
				{
					//create new, pre-filled map before REMOVE test
					hm.reset(hf.createByIdx(hmidx));
					hm->fill(0, itemCounts[r]);
					hm_size = hm->size();
				}

				double tic = TimeStamp();
				switch (t)
				{
				case SpeedResult::R_FILL:
					hm->fill(0, itemCounts[r]);
					hm_size = hm->size();
					break;
				case SpeedResult::R_FIND_MISSING:
					hm->find(0 + itemCounts[r], itemCounts[r] + itemCounts[r], false);
					break;
				case SpeedResult::R_FIND_EXISTING:
					hm->find(0, itemCounts[r], true);
					break;
				case SpeedResult::R_DELETE:
					hm->deleteContainer();
					break;
				case SpeedResult::R_REMOVE:
					hm->remove(0, itemCounts[r], true); //going backwards otherwise flat maps like Loki::AssocVector would run prohibitively long
					break;
				default: assert(0);
				}

				double dt = TimeStamp() - tic;
				double rate = hm_size / dt;
				results[hmidx][r].rates[t] = rate;
			}
			printf("\n");
		}
	}

	if (bSingleChart)
	{
		std::ostringstream ss;
		ss << "[";
		// header
		ss << "[ 'Raw data MB'";
		size_t nc = nMaps; //num of cols
		for (int i = 0; i < nMaps; ++i)
			ss << ", '" << hf.mapTypeNameByIdx(i) << "'";
		ss << "]\n";
		// body
		for (size_t ridx = 0; ridx < SpeedResult::R_MAX; ++ridx)
		{
			ss << ",['" << SpeedResult::names[ridx] << "'";
			for (size_t c = 0; c < nc; ++c)
			{
				ss << "," << results[c][0].rates[ridx];
			}
			ss << "]\n";
		}
		ss << "]";

		//insert dataTable into line chart
		int l;
		const char* p = embedded_html_files("barchart.html", &l);
		std::string linechart(p, l);

		replace_string(linechart, "${ARRAY}", ss.str());
		replace_string(linechart, "${TITLE}", "Speed test of hash maps, element count: " + to_string(itemCounts[0]) + ", element size = " + to_string(kItemSize) + " bytes (key+value)");
		replace_string(linechart, "${HAXIS_TITLE}", "Operations per second [Hz]");
		replace_string(linechart, "${VAXIS_TITLE}", "Operation");

		//write out
        save_result("hash_maps_speedtest.html", linechart.c_str());
	}
	else
	{
		for (int ridx = 0; ridx < SpeedResult::R_MAX; ++ridx)
		{
			std::ostringstream ss;
			ss << "[";
			// header
			ss << "[ 'Raw data MB'";
			size_t nc = nMaps; //num of cols
			for (int i = 0; i < nMaps; ++i)
				ss << ", '" << hf.mapTypeNameByIdx(i) << "'";
			ss << "]\n";
			// body
			for (size_t r = 0; r < itemCounts.size(); ++r)
			{
				ss << ",[" << itemCounts[r];
				for (size_t c = 0; c < nc; ++c)
				{
					ss << "," << results[c][r].rates[ridx];
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
			replace_string(linechart, "${TITLE}", std::string(rateTitles[ridx]) + ", element size = " + to_string(kItemSize) + " bytes (key+value)");
			replace_string(linechart, "${HAXIS_TITLE}", "Operations per second [Hz]");
			replace_string(linechart, "${VAXIS_TITLE}", "Operation");
			replace_string(linechart, "${HAXIS_LOGSCALE}", "true");

			//write out
            save_result(
                        (std::string("hash_maps_speedtest_") + SpeedResult::names[ridx] + ".html").c_str(),
                        linechart.c_str());
		}
	}
}