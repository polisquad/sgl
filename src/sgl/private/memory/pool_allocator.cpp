#include "memory/pool_allocator.h"

void PoolAllocator::init(uint32 chunkSize)
{
	// Set chunks size
	this->chunkSize = chunkSize;

	// Initialize linked list
	head = start;
	void	* last = static_cast<ubyte*>(end) - (chunkSize + sizeof(void*)),
			* next = nullptr;

	while (head < last)
	{
		// Allocate chunk with next pointer
		next = head = static_cast<ubyte*>(head) + chunkSize;
		head = static_cast<ubyte*>(head) + sizeof(void*);

		// Set next pointer
		*static_cast<void**>(next) = head;
	}

	// End linked list
	*static_cast<void**>(next) = nullptr;
	head = start;
}

void * PoolAllocator::alloc(uint32 n)
{
	if (head && n <= chunkSize)
	{
		// Pop head of free chunks
		void * chunk = head;
		head = static_cast<ubyte*>(head) + chunkSize;
		head = *static_cast<void**>(head);

		return chunk;
	}

	return nullptr;
}

void PoolAllocator::free(void * ptr)
{
	// We make the assumption that ptr
	// had been allocated by this allocator

	// Relink to linked list
	void * next = static_cast<ubyte*>(ptr) + chunkSize;
	*static_cast<void**>(next) = head;

	// Set as new head
	head = ptr;
}