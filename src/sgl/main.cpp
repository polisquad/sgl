#include "core_types.h"
#include "hal/platform_memory.h"
#include "hal/malloc_binned.h"
#include "hal/platform_math.h"
#include "generic/memory_pool.h"
#include "coremin.h"
#include "hal/runnable_thread.h"
#include "hal/runnable_pthread.h"
#include "hal/runnable.h"
#include "hal/threading.h"
#include "hal/event_pthread.h"
#include "async/future.h"

// Just using it to measure threading performance
#include <omp.h>

Malloc * gMalloc = nullptr;

namespace Test
{
	/// Tests
	/// @{
	FORCE_INLINE int32 memory();
	FORCE_INLINE int32 array();
	FORCE_INLINE int32 queue();
	FORCE_INLINE int32 map();
	/// @}
}

class CalcSomething : public Runnable
{
protected:
	uint32 workerId;

public:
	CalcSomething(uint32 _workerId) : workerId(_workerId) {}

	virtual uint32 run() override
	{
		static PThreadEvent futureState;
		futureState.create();
		if (workerId == 0)
		{
			printf("I'm going to sleep ...\n");
			sleep(2.f);
			printf("deploying\n");
			futureState.trigger(true);
		}
		else
		{
			printf("I'm waiting ...\n");
			if (futureState.wait(10000))
				//futureState.trigger(),
				printf("Here we go, I'm number %u\n", workerId);
			else
				printf("I'm tired of waiting you moron\n");
		}
	}
};

int main()
{
	Memory::createGMalloc();
	srand(clock());

	Array<RunnableThread*> threads;
	for (uint8 k = 0; k < 8; ++k)
		threads.push(RunnableThread::create(new CalcSomething(k), "MyThread"));

	for (uint8 k = 0; k < 8; ++k)
		delete threads[k];

	//return Test::array();
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

	aAnsi.reset();
	aBinned.reset();
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
	MallocBinned * lMalloc = new MallocBinned;

	Queue<uint64> qAnsi;
	Queue<uint64> qBinned(lMalloc);

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
	Map<uint64, uint64> map(new MallocBinned);
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