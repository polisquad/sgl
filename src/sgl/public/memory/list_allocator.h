#ifndef SGL_LIST_ALLOCATOR_H
#define SGL_LIST_ALLOCATOR_H

#include "allocator.h"

/**
 * @struct ListAllocator memory/list_allocator.h
 * @brief A general purpose allocator that has an
 * allocation and deallocation complexity of O(n)
 */
struct ListAllocator : public Allocator
{
protected:
	/**
	 * @brief Head of the free list
	 */
	void * head;

public:
	/**
	 * @brief Inherit constructors
	 */
	using Allocator::Allocator;

	/**
	 * @copydoc Allocator::init()
	 */
	void init();

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

void ListAllocator::reset()
{
	#define block_next(block) *reinterpret_cast<void**>(block)
	#define block_size(block) *(reinterpret_cast<uint64*>(block) + 1)

	// Set head to start
	head = start;
	block_next(head) = nullptr;
	block_size(head) = size - headerSize;

	#undef block_next
	#undef block_size
}

#endif