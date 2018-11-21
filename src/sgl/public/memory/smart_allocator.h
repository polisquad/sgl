#ifndef SGL_SMART_ALLOCATOR_H
#define SGL_SMART_ALLOCATOR_H

#include "pool_allocator.h"
#include "list_allocator.h"

/**
 * @struct SmartAllocator memory/smart_allocator.h
 * @brief A pool allocator backed up by a list allocator
 * (or a tree allocator if I'll ever have the guts to implement it)
 */
struct SmartAllocator : public Allocator
{
protected:
	/**
	 * @brief The pool allocator
	 */
	PoolAllocator * pool;

	/**
	 * @brief The list allocator
	 */
	ListAllocator * list;

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
	inline void init(uint32 chunkSize = 64);

	/**
	 * @copydoc Allocator::alloc()
	 */
	virtual inline void * alloc(uint64 n);
	
	/**
	 * @copydoc Allocator::free()
	 */
	virtual inline void free(void * ptr);

	/**
	 * @copydoc Allocator::reset()
	 */
	virtual inline void reset();
};

void SmartAllocator::init(uint32 chunkSize)
{
	// Create pool allocator
	pool = new PoolAllocator(start, end);
	pool->bStrictAllocation = true;
	pool->init(chunkSize);

	// Create backup list allocator
	// Which uses half buffer size
	void * listBuffer = nullptr;
	posix_memalign(&listBuffer, sizeof(void*), size / 2);
	list = new ListAllocator(listBuffer, size / 2);
}

void * SmartAllocator::alloc(uint64 n)
{
	// Allocate in pool
	void * block = pool->alloc(n);
	if (block) return block;

	// Otherwise allocate in list
	block = list->alloc(n);
	return block;
}

void SmartAllocator::free(void * ptr)
{
	if (pool->hasBlock(ptr))
		pool->free(ptr);
	else if (list->hasBlock(ptr))
		list->free(ptr);
}

void SmartAllocator::reset()
{
	// Reset pool and list
	pool->reset(), list->reset();
}

#endif