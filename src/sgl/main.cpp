#include "core_types.h"
#include "hal/platform_memory.h"
#include "hal/malloc_binned.h"
#include "hal/platform_math.h"
#include "generic/memory_pool.h"
#include "coremin.h"

Malloc			* gMalloc = nullptr;
MallocBinned	* gMallocBinned = nullptr;

namespace Test
{
	/// Tests
	/// @{
	FORCE_INLINE int32 memory();
	FORCE_INLINE int32 array();
	FORCE_INLINE int32 queue();
	FORCE_INLINE int32 map();
	FORCE_INLINE int32 list();
	/// @}
}

#include <map>

int main()
{
	Memory::createGMalloc();
	gMallocBinned = new MallocBinned;

	uint64 sum = 0;

	//srand(clock());

	BinaryTree<uint64, MallocAnsi> tree;
	for (uint32 i = 0; i < 1024 * 1024; ++i)
		tree.insertUnique(rand() % (1024 * 1024));

	/* std::map<uint64, uint64> tree;
	for (uint32 i = 0; i < 1024 * 1024; ++i)
		tree.insert(std::make_pair(rand() % (1024 * 1024), i)); */
	
	/* for (uint32 i = 0; i < 1024 * 128; ++i)
	{
		auto it = tree.find(i);
		if (it != tree.end()) ++sum;
	}

	printf("%llu\n", sum); */

	/* BinaryTree<uint64, MallocAnsi> other = tree;

	tree.empty();

	for (uint32 i = 0; i < 1024 * 128; ++i)
	{
		auto it = tree.find(i);
		if (it != tree.end()) --sum;
	}

	printf("%llu\n", sum); */

	return 0;

	return
		Test::memory() &
		Test::array() &
		Test::list() &
		Test::queue() &
		Test::map();
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
		buffer = gMallocBinned->malloc(1024), *reinterpret_cast<int64*>(buffer) = i;
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

	printf("------------------------------\n");

	Array<uint64>				aAnsi(2);
	Array<uint64, MallocBinned>	aBinned(2, gMallocBinned);
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

	/* start = clock();
	aAnsi.append(aAnsi);
	printf("AAnsi::append   | %ld ticks\n", clock() - start);

	printf("------------------------------\n"); */

	start = clock();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		aAnsi.removeAt(aAnsi.getCount());
	printf("AAnsi::remove   | %ld ticks\n", clock() - start);

	start = clock();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		aBinned.removeAt(aBinned.getCount() - 1);
	printf("ABinned::remove | %ld ticks\n", clock() - start);

	printf("------------------------------\n");

	/* aAnsi.reset();
	aBinned.reset(); */
	std::vector<uint64> stdvec; stdvec.reserve(2);

	start = clock();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		aAnsi.push(i);
	printf("AAnsi::push     | %ld ticks\n", clock() - start);

	start = clock();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		aBinned.push(i);
	printf("ABinned::push   | %ld ticks\n", clock() - start);

	start = clock();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		stdvec.push_back(i);
	printf("stdvec::push    | %ld ticks\n", clock() - start);

	printf("------------------------------\n");

	return 0;
}

int32 Test::queue()
{
	auto start = clock();
	void * buffer;

	start = clock();

	Queue<uint64> qAnsi;
	Queue<uint64, MallocBinned> qBinned(gMallocBinned);

	printf("------------------------------\n");

	start = clock();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		qAnsi.push(i);
	for (uint64 i = 0; i < 1024 * 128; ++i)
		qAnsi.pop();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		qAnsi.push(i);
	printf("QAnsi           | %ld ticks\n", clock() - start);

	start = clock();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		qBinned.push(i);
	for (uint64 i = 0; i < 1024 * 128; ++i)
		qBinned.pop();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		qBinned.push(i);
	printf("QBinned         | %ld ticks\n", clock() - start);

	printf("------------------------------\n");

	return 0;
}

#include <map>
#include <string>

int32 Test::map()
{
	auto start = clock();
	Map<uint64, uint64> map;
	std::map<uint64, uint64> stdmap;

	printf("------------------------------\n");
	
	start = clock();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		map.insert(i, i / 2);
	printf("TreeMap::insert | %ld ticks\n", clock() - start);

	start = clock();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		stdmap.insert(std::make_pair(i, i / 2));
	printf("std::map::insert| %ld ticks\n", clock() - start);
	
	start = clock();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		map[i] *= 2;
	printf("TreeMap::[]     | %ld ticks\n", clock() - start);

	start = clock();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		stdmap[i] *= 2;
	printf("std::map::[]    | %ld ticks\n", clock() - start);

	printf("------------------------------\n");

	return 0;
}

int32 Test::list()
{
	auto start = clock();
	void * buffer;
	uint64 client;

	start = clock();

	Queue<uint64> llAnsi;
	Queue<uint64, MallocBinned> llBinned(gMallocBinned);

	printf("------------------------------\n");

	start = clock();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		llAnsi.push(i);
	for (uint64 i = 0; i < 1024 * 128; ++i)
		llAnsi.pop(client);
	for (uint64 i = 0; i < 1024 * 128; ++i)
		llAnsi.push(i);
	printf("llAnsi           | %ld ticks\n", clock() - start);

	start = clock();
	for (uint64 i = 0; i < 1024 * 128; ++i)
		llBinned.push(i);
	for (uint64 i = 0; i < 1024 * 128; ++i)
		llBinned.pop(client);
	for (uint64 i = 0; i < 1024 * 128; ++i)
		llBinned.push(i);
	printf("llBinned         | %ld ticks\n", clock() - start);

	printf("------------------------------\n");

	return 0;
}