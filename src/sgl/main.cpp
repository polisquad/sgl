#include "core_types.h"
#include "coremin.h"

// Just using it to measure threading performance
#include <omp.h>

Malloc * gMalloc = nullptr;

int main()
{
	MallocBinned * bin = new MallocBinned;
	Array<uint64> foo(1024 * 64);

	for (uint64 i = 0; i < 1024 * 64; ++i)
		foo.push(i);
	
	return 0;
}