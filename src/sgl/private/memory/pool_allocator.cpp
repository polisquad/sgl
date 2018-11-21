#include "memory/pool_allocator.h"

#define block_offset(block, offset)	reinterpret_cast<ubyte*>(block) + offset
#define block_next(block)			*reinterpret_cast<void**>(block)
#define block_mem(block)			(reinterpret_cast<void**>(block) + 1)
#define mem_block(mem)				(reinterpret_cast<void**>(mem) - 1)
#define align_dword(n)				n & 0x7 ? (n | 0x7) + 1 : n

void PoolAllocator::init(uint32 chunkSize)
{
	// Set chunks size
	this->chunkSize = align_dword(chunkSize);

	// Initialize linked list
	head = start;
	void	* last = block_offset(end, -(chunkSize + sizeof(void*))),
			* next = nullptr;

	while (head < last)
	{
		next = block_offset(head, chunkSize + 0x8);
		block_next(head) = next;
		head = next;
	}

	// Terminate linked list
	block_next(head) = nullptr;
	
	// Head is start
	head = start;
}

void * PoolAllocator::alloc(uint64 n)
{
	bool size_check = n <= chunkSize && !(bStrictAllocation && n < chunkSize);
	if (head && size_check)
	{
		// Pop head of free chunks
		void * chunk = block_mem(head);
		head = block_next(head);

		return chunk;
	}

	return nullptr;
}

void PoolAllocator::free(void * ptr)
{
	// We make the assumption that ptr
	// had been allocated by this allocator

	// Relink to linked list
	ptr = mem_block(ptr);
	block_next(ptr) = head;

	// Set as new head
	head = ptr;
}