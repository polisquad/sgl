#ifndef SGL_MALLOC_BINNED_H
#define SGL_MALLOC_BINNED_H

#include "platform_memory.h"
#include "platform_math.h"

#define BINNED_POOL_SIZE 4096 * 1024		// Fixed pool size, 4 MB
#define BINNED_BLOCK_MIN_SIZE 64			// Min block size (block size for first bucket), 64 B
#define BINNED_BLOCK_MAX_SIZE 1024 * 1024	// Max block size (block size for last bucket), 1 MB
#define BINNED_NUM_BUCKETS 15				// Should change according to min and max bucket size
#define BINNED_POOL_ALIGNMENT 0x1000		// 4 KB, size of a page

/**
 * @class MallocBinned hal/malloc_binned.h
 * @brief Default allocation class
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

public:
	/// @brief Default-constructor
	MallocBinned();

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
	/// @brief Get associated bucket index
	FORCE_INLINE uint8 getBucketIndexFromSize(sizet n)
	{
		return PlatformMath::getNextPowerOf2Index(n / BINNED_BLOCK_MIN_SIZE);
	}
};

#endif