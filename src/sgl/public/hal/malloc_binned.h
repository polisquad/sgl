#pragma once

#include "platform_memory.h"
#include "platform_math.h"

#define BINNED_POOL_SIZE 8 * 1024 * 1024	// Fixed pool size, 4 MB
#define BINNED_BLOCK_MIN_SIZE 32			// Min block size (block size for first bucket), 32 B
#define BINNED_BLOCK_MAX_SIZE 1024 * 1024	// Max block size (block size for last bucket), 1 MB
#define BINNED_NUM_BUCKETS 16				// Should change according to min and max bucket size
#define BINNED_POOL_ALIGNMENT 0x1000		// 4 KB, size of a page

/**
 * @class MallocBinned hal/malloc_binned.h
 * @brief Allocator backed by memory pools
 * 
 * A binned allocator leverages seeveral memory
 * pools organized in buckets of linked lists.
 * Each bucket is tagged with the block dimension
 * of the underlying memory pool.
 * 
 * When an allocation request is made, the request
 * is forwarded to the appropriate bucket and the
 * first bucket is queried for a free block. If the
 * pool is exahusted, a new pool is created on the
 * fly and push to the front of the list (for
 * future allocations).
 * 
 * Deallocating e block of memory requires a linear
 * search in each bucket. When the block is found it
 * is added to the list of free blocks in the pool.
 * 
 * Reallocation combines allocation and deallocation.
 * 
 * If the requested size exceeds the maximum block
 * size, the request is handled by an external
 * allocator (probably an instance of @ref MallocAnsi)
 */
class MallocBinned : public Malloc
{
protected:
	typedef struct BucketPool
	{
		/// Pointer to memory pool
		struct MemoryPool * pool;

		/// Next pool in bucket
		BucketPool * next;
	} * Bucket;

protected:
	/// @brief Buckets ordered by size
	Bucket buckets[BINNED_NUM_BUCKETS];

	/// @brief Buckets tails
	Bucket bucketsTails[BINNED_NUM_BUCKETS];

	/// @brief Last allocated memory pool
	MemoryPool * lastUsed;

public:
	/// @brief Default-constructor
	MallocBinned();

	/// @brief Destructor
	~MallocBinned();

	/// @group MallocInterface
	/// @{
	/// @copydoc Malloc::malloc()
	virtual void * malloc(uintP n, uint32 alignment = DEFAULT_ALIGNMENT) override;

	/// @copydoc Malloc::realloc()
	virtual void * realloc(void * original, uintP n, uint32 alignment = DEFAULT_ALIGNMENT) override;

	/// @copydoc Malloc::free()
	virtual void free(void * original) override;

	/// @copydoc Malloc::getAllocSize()
	virtual bool getAllocSize(void * original, uintP & n) override;
	/// @}

protected:
	/**
	 * @brief Create a new pool
	 * 
	 * @param [in]	i	bucket index
	 */
	BucketPool * createBucketPool(uint8 i);

	/// @brief Get associated bucket index
	FORCE_INLINE uint8 getBucketIndexFromSize(sizet n)
	{
		if (n <= BINNED_BLOCK_MIN_SIZE) return 0;

		const uint8 minIndex = PlatformMath::getNextPowerOf2Index(BINNED_BLOCK_MIN_SIZE);
		return PlatformMath::getNextPowerOf2Index(n) - minIndex;
	}
};

