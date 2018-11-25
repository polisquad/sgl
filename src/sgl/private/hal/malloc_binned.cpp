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
	void * buffer = gMalloc->malloc(bufferSize);

	// Inital part of buffer reserved for buckets headers
	BucketPool * bucketHeaders = reinterpret_cast<BucketPool*>(buffer);
	// Then buckets tails
	BucketPool * bucketFooters = bucketHeaders + BINNED_NUM_BUCKETS;
	// Then pool structures
	MemoryPool * pools = reinterpret_cast<MemoryPool*>(bucketFooters + BINNED_NUM_BUCKETS);
	// Then, allocate actual pool buffers
	sizet poolBuffers = reinterpret_cast<sizet>(pools + BINNED_NUM_BUCKETS);

	// Init all buckets with a pool
	for (uint8 i = 0; i < BINNED_NUM_BUCKETS; ++i)
	{
		// Calculate block size and num blocks
		const sizet blockSize = (0x1 << i) * BINNED_BLOCK_MIN_SIZE;
		const sizet numBlocks = BINNED_POOL_SIZE / blockSize;

		// Sanity check
		assert(blockSize * numBlocks == BINNED_POOL_SIZE);

		// Create pool
		new (pools + i) MemoryPool(blockSize, numBlocks, reinterpret_cast<void*>(poolBuffers), BINNED_POOL_ALIGNMENT);
		poolBuffers += BINNED_POOL_SIZE + BINNED_POOL_ALIGNMENT;
		
		// Init buckets
		buckets[i] = bucketHeaders + i, bucketsTails[i] = bucketFooters;
		(bucketHeaders + i)->pool = (bucketFooters + i)->pool = reinterpret_cast<MemoryPool*>(pools + i);
		(bucketHeaders + i)->next = (bucketFooters + i)->next = nullptr;
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
	while (LIKELY(it != nullptr))
	{
		// Query pool for usable memory
		MemoryPool * pool = it->pool;
		if (pool->getUsable() >= n)
		{
			out = pool->allocate(n);
			
			// Bring back
			if (UNLIKELY(pool->getUsable() == 0))
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

			return out;
		}

		// Iterate
		prev = it;
		it = it->next;
	}

	return nullptr;
}

void * MallocBinned::realloc(void * original, uintP n, uint32 alignment)
{
	// Get bucket index
	const uint8 bucketIdx = getBucketIndexFromSize(n);
	if (bucketIdx > BINNED_NUM_BUCKETS)
		/** @todo Use external realloc */
		gMalloc->realloc(original, n, alignment);
	
	// Check if still in same bucket
	BucketPool * targetBucketPool = nullptr, * targetPrev = nullptr;
	sizet maxUsable = 0;
	{
		BucketPool * it = buckets[bucketIdx], * prev = nullptr;
		while (it)
		{
			MemoryPool * pool = it->pool;
			if (pool->hasBlock(original))
				// Don't need reallocation
				return original;
			
			// Find best pool in the meantime
			sizet usable = pool->getUsable();
			if (UNLIKELY(usable > maxUsable))
			{
				targetBucketPool = it;
				targetPrev = prev;
				maxUsable = pool->getUsable();
			}

			prev = it;
			it = it->next;
		}
	}

	// Really we should just allocate a new bucket, I'll leave it for later
	if (UNLIKELY(maxUsable == 0)) return nullptr;

	// Search in smaller buckets
	for (uint8 i = bucketIdx - 1; i >= 0; --i)
	{
		BucketPool * it = buckets[i];
		while (it)
		{
			MemoryPool * pool = it->pool;
			if (UNLIKELY(pool->hasBlock(original)))
			{
				// And allocate using target bucket
				MemoryPool * targetPool = targetBucketPool->pool;
				void * out = targetPool->allocate(n);

				// Copy memory
				PlatformMemory::memcpy(out, original, n);
				
				// Free original
				pool->free(original);

				// Bring back if not usable
				if (UNLIKELY(targetPool->getUsable() == 0))
				{
					// Pop
					if (UNLIKELY(targetPrev != nullptr))
						targetPrev->next = it->next;
					else
						buckets[bucketIdx] = it->next;
					
					bucketsTails[bucketIdx]->next = targetBucketPool;
					bucketsTails[bucketIdx] = targetBucketPool;
					targetBucketPool->next = nullptr;
				}

				return out;
			}

			it = it->next;
		}
	}

	return nullptr;
}

void MallocBinned::free(void * original)
{
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
}

bool MallocBinned::getAllocSize(void * original, uintP & n) { return false; }