#ifndef SGL_MEMORY_POOL_H
#define SGL_MEMORY_POOL_H

#include "core_types.h"
#include "hal/platform_memory.h"

/**
 * @struct MemoryPool generic/memory_pool.h
 * @brief A pool of fixed size memory blocks
 * 
 * A memory pool is contains N blocks of fixed size.
 * A list of free blocks is mantained in a bookkeping
 * memory chunk allocated before the pool buffer.
 * 
 * The pool buffer must be aligned to the system page size
 * (which right now we assume to be a constant 4 KB, note
 * that this is true for the most of the systems, unless
 * the user explicitly changed it).
 * 
 * We don't build a buffer on our own, but we rather receive
 * an already allocated one and we assume that its size is
 * enough to create an aligned pool (i.e. 2 * blockSize *
 * numBlocks)
 */
struct MemoryPool final
{
protected:
	/// @brief Index list structure
	typedef struct Index
	{
		/// Pool block
		void * block;

		/// Next index
		Index * next;
	} * IndexList;

protected:
	/// @brief Pointer to the index list
	IndexList indices;

	/// @brief Pointer to the pool buffer
	void * pool;

	/// @brief Index of first free block
	Index * head;

	/// @brief Block size
	sizet blockSize;

	/// @brief Number of blocks in the pool
	sizet numBlocks;

	/// @brief Number of free blocks
	sizet numFreeBlocks;

public:
	/**
	 * @brief Default-constructor
	 * 
	 * @param [in]	_blockSize	size of a single block
	 * @param [in]	_numBlocks	number of requested blocks
	 * @param [in]	_buffer		The buffer on which to create the pool.
	 * 							We don't require this to aligned
	 */
	MemoryPool(sizet _blockSize, sizet _numBlocks, void * _buffer);

	/// Reset indices list
	/// This wipes out all memory
	void resetIndices();

	/**
	 * @brief Allocate a new block
	 * 
	 * @param [in] n Bytes to allocate
	 * 
	 * @return Allocated block
	 */
	void * allocate(sizet n);

	/// @brief Info functions
	/// @{
	FORCE_INLINE bool	isEmpty() const { return numFreeBlocks == numBlocks; }
	FORCE_INLINE bool	canAllocate(sizet n) const { return n <= blockSize; }
	FORCE_INLINE sizet	getUsable() const { return numFreeBlocks * blockSize; }
	/// @}
};

MemoryPool::MemoryPool(sizet _blockSize, sizet _numBlocks, void * _buffer) :
	indices(nullptr),
	pool(nullptr),
	head(nullptr),
	blockSize(_blockSize),
	numBlocks(_numBlocks),
	numFreeBlocks(_numBlocks)
{
	// Reserve space for indices
	indices = reinterpret_cast<IndexList>(_buffer);
	_buffer = indices + _numBlocks;

	// Align up pool buffer
	pool = Memory::align(_buffer, 0x1000);

	// Set indices
	resetIndices();
}

void MemoryPool::resetIndices()
{
	sizet block = reinterpret_cast<sizet>(pool);

	// Rebuild index list
	sizet i = 0;
	for (; i < numBlocks; ++i)
	{
		indices[i].block	= reinterpret_cast<void*>(block + (i * blockSize));
		indices[i].next		= indices + i + 1;
	}

	// Last block
	indices[i].next = nullptr;

	// Set free block
	head = indices;
}

void * MemoryPool::allocate(sizet n)
{
	// Check size
	assert(n <= blockSize);

	if (head)
	{
		void * out = head;

		// Pop index
		head = head->next;

		--numFreeBlocks;
		return out;
	}

	return nullptr;
}

#endif