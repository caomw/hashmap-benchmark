#include <cstdio>

#include "config.h"
#include "memtest.h"
#include "speedtest.h"

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x

int main(int argc, const char* argv[])
{
	printf("Key type: '%s', mapped type size: %d\n", STRINGIZE(KEY_TYPE), MAPPED_TYPE_SIZE);
	printf("Item size: %d\n", kItemSize);

	memtest();
	speedtest(SP_SINGLE_ELEMENT_COUNT);
}