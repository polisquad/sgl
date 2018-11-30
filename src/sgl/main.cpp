#include "core_types.h"
#include "hal/platform_memory.h"
#include "hal/malloc_binned.h"
#include "hal/platform_math.h"
#include "generic/memory_pool.h"
#include "coremin.h"

Malloc * gMalloc = nullptr;

namespace Test
{
	/// Tests
	/// @{
	FORCE_INLINE int32 memory();
	FORCE_INLINE int32 array();
	FORCE_INLINE int32 queue();
	/// @}
}

int main()
{
	Memory::createGMalloc();
	return Test::array();
}

int32 Test::memory()
{
	auto start = clock();
	void * buffer;

	printf("------------------------------\n");

	start = clock();
	MallocBinned * lMalloc = new MallocBinned;
	printf("memory pool init | %ld ticks\n", clock() - start);

	printf("------------------------------\n");

	start = clock();
	for (uint64 i = 0; i < 1024 * 512; ++i)
		buffer = gMalloc->malloc(1024), *reinterpret_cast<int64*>(buffer) = i;
	printf("malloc ansi      | %ld ticks\n", clock() - start);

	start = clock();
	for (uint64 i = 0; i < 1024 * 512; ++i)
		buffer = lMalloc->malloc(1024), *reinterpret_cast<int64*>(buffer) = i;
	printf("malloc binned    | %ld ticks\n", clock() - start);

	start = clock();
	for (uint64 i = 0; i < 1024 * 512; ++i)
		buffer = malloc(1024), *reinterpret_cast<int64*>(buffer) = i;
	printf("malloc:          | %ld ticks\n", clock() - start);

	printf("------------------------------\n");

	return 0;
}

#include <vector>

int32 Test::array()
{
	void * out;
	auto start = clock();
	MallocBinned * lMalloc = new MallocBinned;

	printf("------------------------------\n");

	Array<uint64> aAnsi(2);
	Array<uint64> aBinned(2, lMalloc);
	std::vector<uint64>::iterator it;

	start = clock();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		aAnsi(i) = i;
	printf("AAnsi::OOBA     | %ld ticks\n", clock() - start);

	start = clock();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		aBinned(i) = i;
	printf("ABinned:OOBA    | %ld ticks\n", clock() - start);

	printf("------------------------------\n");
	start = clock();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		aAnsi.push(i);
	printf("AAnsi           | %ld ticks\n", clock() - start);

	start = clock();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		aBinned.push(i);
	printf("ABinned         | %ld ticks\n", clock() - start);

	printf("------------------------------\n");

	start = clock();
	aAnsi.append(aAnsi);
	printf("AAnsi::append   | %ld ticks\n", clock() - start);

	printf("------------------------------\n");

	start = clock();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		aAnsi.removeAt(aAnsi.getCount());
	printf("AAnsi::remove   | %ld ticks\n", clock() - start);

	start = clock();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		aBinned.removeAt(aBinned.getCount() - 1);
	printf("ABinned::remove | %ld ticks\n", clock() - start);

	printf("------------------------------\n");

	return 0;
}

int32 Test::queue()
{
	auto start = clock();
	void * buffer;

	start = clock();
	MallocBinned * lMalloc = new MallocBinned;

	Queue<uint64> qAnsi;
	Queue<uint64> qBinned(lMalloc);

	printf("------------------------------\n");

	start = clock();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		qAnsi.push(i);
	printf("QAnsi           | %ld ticks\n", clock() - start);

	start = clock();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		qBinned.push(i);
	printf("QBinned         | %ld ticks\n", clock() - start);

	printf("------------------------------\n");

	return 0;
}