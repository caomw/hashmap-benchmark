#include <memory>
#include <map>
#include <sstream>
#include <fstream>

#include "macros.h"
#include "harnessfactory.h"
#include "dobenchmark.h"
#include "mappedtype.h"
#include "embedded_html_files.h"

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x

//data for one run of the test
struct RunData
{
	size_t itemCount;
	double rawDataMbytes;
};

int main(int argc, const char* argv[])
{
	size_t testRawDataMBytes_from = 200;
	size_t testRawDataMBytes_to = 400;

	size_t kItemSize = sizeof(KEY_TYPE)+sizeof(MappedType<KEY_TYPE, MAPPED_TYPE_SIZE>);
	printf("Key type: '%s', mapped type size: %d\n", STRINGIZE(KEY_TYPE), MAPPED_TYPE_SIZE);
	printf("Item size: %d\n", kItemSize);

	std::vector<RunData> runDataList;

	int nRuns = 33;
	size_t kTestItemCountDiff = (size_t)((double)(testRawDataMBytes_to - testRawDataMBytes_from) * 1024 * 1024 / kItemSize);
	size_t dItems = kTestItemCountDiff / nRuns;
	size_t count_from = (size_t)(testRawDataMBytes_from * 1024.0 * 1024.0 / kItemSize);
	for (int r = 0; r < nRuns; ++r)
	{
		RunData rd;
		rd.itemCount = (r + 1) * dItems + count_from;
		rd.rawDataMbytes = rd.itemCount * kItemSize / 1024.0 / 1024.0;
		runDataList.push_back(rd);
	}

	HarnessFactory hf;


	//space graphs
	{
		std::vector<Harness*> hs(hf.createAll());
		std::map<size_t, std::vector<double>> results; // results[hashMapIdx][run]
		//initialize results array
		for (size_t i = 0; i < hs.size(); ++i)
		{
			results[i].resize(nRuns);
		}
		for (auto it = hs.begin(); it != hs.end(); ++it)
		{
			size_t hmidx = it - hs.begin();
			printf("Testing memory usage of %s\n", (*it)->hashMapType().c_str());
			size_t mem0 = GetMemUsage();
			size_t prevItemCount = 0;
			for (int r = 0; r < runDataList.size(); ++r)
			{
				printf("%d..", r);
				(*it)->fill(prevItemCount, runDataList[r].itemCount);
				prevItemCount = runDataList[r].itemCount;
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
		size_t nr = nRuns; //num of rows
		for (auto it = hs.begin(); it != hs.end(); ++it)
			ss << ", '" << (*it)->hashMapType() << "'";
		ss << "]\n";
		// body
		for (size_t r = 0; r < nr; ++r)
		{
			ss << ",[" << runDataList[r].rawDataMbytes;
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
		int l;
		const char* p = embedded_html_files("linechart.html", &l);
		std::string linechart(p, l);
		size_t spos = linechart.find("$");
		linechart = linechart.substr(0, spos) + ss.str() + linechart.substr(spos + 1);
		std::ofstream ofs("linechart.html");
		ofs << linechart;
	}
	

/*	for (auto it = hs.begin(); it != hs.end(); ++it)
	{
		BenchmarkResult br = DoBenchmark(**it, &Harness::FillSequential);
		printf("Type name: %s\n", (*it)->hashMapType().c_str());
		printf("Result: %s\n\n", br.toString(count).c_str());
	}
	*/

}