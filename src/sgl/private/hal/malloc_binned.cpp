#include "hal/malloc_binned.h"
#include "hal/platform_stdlib.h"
#include "generic/memory_pool.h"

MallocBinned::MallocBinned()
{
	// Create a big chunk to contain first buckets
	/** @todo Use @ref Memory::malloc() */
	const sizet bufferSize
		= BINNED_NUM_BUCKETS * sizeof(BucketPool) * 2
		+ BINNED_NUM_BUCKETS * sizeof(MemoryPool)
		+ BINNED_NUM_BUCKETS * (BINNED_POOL_SIZE + BINNED_POOL_ALIGNMENT);
	sizet buffer = reinterpret_cast<sizet>(gMalloc->malloc(bufferSize));

	// Init all buckets with a pool
	for (uint8 i = 0; i < BINNED_NUM_BUCKETS; ++i)
	{
		BucketPool * bucketHeader	= reinterpret_cast<BucketPool*>(buffer);
		BucketPool * bucketFooter	= reinterpret_cast<BucketPool*>(bucketHeader + 1);
		MemoryPool * memoryPool		= reinterpret_cast<MemoryPool*>(bucketFooter + 1);
		void * poolBuffer			= memoryPool + 1;

		// Calculate block size and num blocks
		const sizet blockSize = (0x1 << i) * BINNED_BLOCK_MIN_SIZE;
		const sizet numBlocks = BINNED_POOL_SIZE / blockSize;

		// Sanity check
		assert(blockSize * numBlocks == BINNED_POOL_SIZE);

		// Create pool
		new (memoryPool) MemoryPool(blockSize, numBlocks, poolBuffer, BINNED_POOL_ALIGNMENT);
		
		// Init buckets
		buckets[i] = bucketHeader, bucketsTails[i] = bucketFooter;
		bucketHeader->pool = bucketFooter->pool = memoryPool;
		bucketHeader->next = bucketFooter->next = nullptr;

		// Advance buffer
		buffer += BINNED_POOL_SIZE + BINNED_POOL_ALIGNMENT;
	}
}

void * MallocBinned::malloc(uintP n, uint32 alignment)
{
	// Get bucket index
	const uint8 bucketIdx = getBucketIndexFromSize(n);
	if (UNLIKELY(bucketIdx >= BINNED_NUM_BUCKETS))
		/** @todo Use external malloc */
		return gMalloc->malloc(n, alignment);

	// Get free pool
	void * out = nullptr;
	BucketPool * it = buckets[bucketIdx], * prev = nullptr;

	// First pool should be free
	{
		MemoryPool * memoryPool = it->pool;
		if (memoryPool->getUsable() >= n)
		{
			out = memoryPool->allocate(n);

			// Bring back
			if (UNLIKELY(memoryPool->getUsable() == 0 & it->next != nullptr))
			{
				// Pop from head
				buckets[bucketIdx] = it->next;
				it->next = nullptr;

				// Push to back
				bucketsTails[bucketIdx]->next = it;
				bucketsTails[bucketIdx] = it;
			}
		}
		else
		{
			// Create a new pool
			it = createBucketPool(bucketIdx);
			assert(it);

			memoryPool = it->pool;
			out = memoryPool->allocate(n);
		}
	}

	// Set used bucket
	if (LIKELY(out != nullptr)) lastUsed = it->pool;

	return out;
}

void * MallocBinned::realloc(void * original, uintP n, uint32 alignment)
{
	// If no original block, just malloc it
	if (UNLIKELY(original == nullptr)) return malloc(alignment);

	// Get bucket index
	const uint8 bucketIdx = getBucketIndexFromSize(n);
	
	if (LIKELY(bucketIdx < BINNED_NUM_BUCKETS))
	{
		void * out = nullptr;
		BucketPool * it = buckets[bucketIdx], * prev = nullptr;
		while (LIKELY(it != nullptr))
		{
			MemoryPool * memoryPool = it->pool;
			if (memoryPool->hasBlock(original))
			{
				// No need to reallocate
				return original;
			}
			else if (memoryPool->getUsable() >= n)
			{
				// Allocate from this pool
				out = memoryPool->allocate(n);
				PlatformMemory::memcpy(out, original, n);

				// Bring back
				if (UNLIKELY(memoryPool->getUsable() == 0))
				{
					// Pop
					if (prev)
						prev->next = it->next;
					else
						buckets[bucketIdx] = it->next;

					bucketsTails[bucketIdx]->next = it;
					bucketsTails[bucketIdx] = it;
					it->next = nullptr;
				}

				break;
			}

			prev = it;
			it = it->next;
		}

		// If we did not find any available block
		// create a new memory pool
		if (UNLIKELY(!it))
		{
			// Create new bucket pool
			it = createBucketPool(bucketIdx);
			assert(it);

			MemoryPool * memoryPool = it->pool;
			out = memoryPool->allocate(n);
		}

		// Free original block
		free(original);

		// Set used bucket
		if (LIKELY(out != nullptr)) lastUsed = it->pool;

		return out;
	}
	else
	{
		// Allocate, copy and then free
		void * out = gMalloc->malloc(n, alignment);
		PlatformMemory::memcpy(out, original, n);

		// Free original block
		free(original);

		return out;
	}	

	return nullptr;
}

void MallocBinned::free(void * original)
{
	ASSERT(original != nullptr, "Cannot free nullptr");

	// Check last used pool
	if (UNLIKELY(lastUsed && lastUsed->hasBlock(original)))
	{
		// Free block
		lastUsed->free(original);
		return;
	}

	// Slow? maybe
	for (uint8 i = 0; i < BINNED_NUM_BUCKETS; ++i)
	{
		BucketPool * it = buckets[i], * prev = nullptr;	
		while (it)
		{
			MemoryPool * pool = it->pool;
			if (UNLIKELY(pool->hasBlock(original)))
			{
				pool->free(original);

				// Bring to front if more usable memory
				if (pool->getUsable() > buckets[i]->pool->getUsable())
				{
					prev->next = it->next;
					it->next = buckets[i];
					buckets[i] = it;
				}

				return;
			}

			prev = it;
			it = it->next;
		}
	}

	// No free? Then external free
	gMalloc->free(original);
}

bool MallocBinned::getAllocSize(void * original, uintP & n) { return false; }

MallocBinned::BucketPool * MallocBinned::createBucketPool(uint8 i)
{
	// Pool is created and appended to the
	// head of the i-th bucket pool list

	assert(i < BINNED_NUM_BUCKETS);

	const sizet bufferSize
		= sizeof(BucketPool) * 2
		+ sizeof(MemoryPool)
		+ BINNED_POOL_SIZE + BINNED_POOL_ALIGNMENT;
	sizet buffer = reinterpret_cast<sizet>(gMalloc->malloc(bufferSize));
	
	BucketPool * bucketHeader	= reinterpret_cast<BucketPool*>(buffer);
	MemoryPool * memoryPool		= reinterpret_cast<MemoryPool*>(bucketHeader + 1);
	void * poolBuffer			= memoryPool + 1;

	// Create memory pool
	const sizet blockSize = (0x1 << i) * BINNED_BLOCK_MIN_SIZE;
	const sizet numBlocks = BINNED_POOL_SIZE / blockSize;
	assert(blockSize * numBlocks == BINNED_POOL_SIZE);
	new (memoryPool) MemoryPool(blockSize, numBlocks, poolBuffer, BINNED_POOL_ALIGNMENT);

	// Link bucket
	bucketHeader->pool = memoryPool;
	bucketHeader->next = buckets[i];
	buckets[i] = bucketHeader;

	return bucketHeader;
}