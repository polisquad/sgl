#ifndef SGL_POOL_ALLOCATOR_H
#define SGL_POOL_ALLOCATOR_H

#include "allocator.h"

/**
 * @struct PoolAllocator memory/pool_allocator.h
 * @brief An allocator that uses a linked list to
 * provide constant-time allocation and deallocation
 * in a pool of fixed size chunks (blocks)
 */
struct PoolAllocator : public Allocator
{
protected:
	/**
	 * @brief Size of a chunk
	 */
	uint32 chunkSize;

	/**
	 * @brief Head of the free chunks linked list
	 */
	void * head;

public:
	/**
	 * @brief Inherit constructors
	 */
	using Allocator::Allocator;

	/**
	 * @copydoc Allocator::init()
	 * 
	 * @param [in] chunkSize size of a memory chunk
	 */
	void init(uint32 chunkSize = 64);

	/**
	 * @copydoc Allocator::alloc()
	 */
	virtual void * alloc(uint64 n);
	
	/**
	 * @copydoc Allocator::free()
	 */
	virtual void free(void * ptr);

	/**
	 * @copydoc Allocator::reset()
	 */
	virtual inline void reset();
};

void PoolAllocator::reset()
{
	// Rebuild linked list
	init(chunkSize);
}

#endif