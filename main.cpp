#include <memory>

#include "macros.h"
#include "harnessfactory.h"
#include "dobenchmark.h"
#include "mappedtype.h"

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x

int main(int argc, const char* argv[])
{
	size_t count = 10000000;

	printf("Key type: '%s', mapped type size: %d, %d items / test\n", STRINGIZE(KEY_TYPE), MAPPED_TYPE_SIZE, count);
	printf("Item size: %d\n", sizeof(KEY_TYPE)+sizeof(MappedType<KEY_TYPE, MAPPED_TYPE_SIZE>));

	HarnessFactory hf(count);

	std::vector<Harness*> hs(hf.createAll());

	for (auto it = hs.begin(); it != hs.end(); ++it)
	{
		BenchmarkResult br = DoBenchmark(**it, &Harness::FillSequential);
		printf("Type name: %s\n", (*it)->hashMapType().c_str());
		printf("Result: %s\n\n", br.toString(count).c_str());
	}

	for (auto it = hs.begin(); it != hs.end(); ++it)
		delete *it;

}