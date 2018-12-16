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
		static PThreadEvent event;
		event.create();
		if (workerId == 0)
		{
			tell("calculating rotation speed ...");
			const uint32 steps = 20;
			const float32 perc = 1.f / steps;
			for (uint32 i = 0; i < steps; ++i)
			{
				sleep(perc);
				printf("%.1f%% done\n", 100.f * i * perc);
			}
			tell("The rotation speed is 19239128412941289419 rad/s. It's true");
			event.trigger(true);
		}
		else
		{
			tell("Hey, tell me the rotation speed!");
			event.wait();
			workerId != 5 ? tell("Thanks!") : tell("Bullshit!");
		}
	}

private:
	template<typename ... ArgsT>
	FORCE_INLINE void tell(const String & format, ArgsT ... args)
	{
		printf(*(String("%d) ") + format + "\n"), workerId, args ...);
	}

	FORCE_INLINE void tell(const String & format)
	{
		printf(*(String("%d) ") + format + "\n"), workerId);
	}
};

template<typename RetT>
class AsyncTask : public Runnable
{
	/// @brief Compute function
	Function<RetT(void)> computeFunc;

	/// @brief Promise
	Promise<RetT> promise;

public:
	AsyncTask(Promise<RetT> _promise, Function<RetT(void)> && _computeFunc) : promise(_promise), computeFunc(_computeFunc) {}

	virtual uint32 run() override
	{
		// Execute function
		promise.set(computeFunc());
	}
};

template<typename RetT>
Promise<RetT> async(Function<RetT(void)> && computeFunc)
{
	Promise<RetT> promise;
	auto Thread = RunnableThread::create(new AsyncTask<RetT>(promise, (Function<RetT(void)>&&)computeFunc), "AsyncTaskRunnable");
	return promise;
}

#include <omp.h>

int main()
{
	Memory::createGMalloc();
	srand(clock());

	Array<uint64> array1;
	Array<uint64> array2;
	Array<uint64> array3;
	Array<uint64> array4;

	const uint64 size = 1024 * 64;
	for (uint64 i = 0; i < size; ++i)
	{
		array1 += rand() % 1024;
		array2 += rand() % 1024;
		array3 += rand() % 1024;
		array4 += rand() % 1024;
	}
	
	double start = omp_get_wtime();
#if 1
	auto a = async<uint32>([&array1]() mutable {

		Containers::sort(array1);
		return 0;
	});
	auto b = async<uint32>([&array2]() mutable {

		Containers::sort(array2);
		return 0;
	});
	auto c = async<uint32>([&array3]() mutable {

		Containers::sort(array3);
		return 0;
	});
	auto d = async<uint32>([&array4]() mutable {

		Containers::sort(array4);
		return 0;
	});

	a.get(), b.get(), c.get(), d.get();
#else
	Containers::sort(array1), Containers::sort(array2), Containers::sort(array3), Containers::sort(array4);
#endif
	printf("elasped: %f sec\n", omp_get_wtime() - start);
	
	// Check
	for (uint32 i = 1; i < array1.getCount(); ++i)
	{
		if (array1[i - 1] > array1[i]) return 1;
		if (array2[i - 1] > array2[i]) return 2;
		if (array3[i - 1] > array3[i]) return 3;
		if (array4[i - 1] > array4[i]) return 4;
	}

	/* Array<RunnableThread*> threads;
	for (uint8 k = 0; k < 8; ++k)
		threads.push(RunnableThread::create(new CalcSomething(k), "MyThread"));

	for (uint8 k = 0; k < 8; ++k)
		delete threads[k]; */

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