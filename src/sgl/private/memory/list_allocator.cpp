#include "memory/list_allocator.h"

#define block_offset(block, offset) (reinterpret_cast<ubyte*>(block) + offset)
#define block_next(block) *reinterpret_cast<void**>(block)
#define block_size(block) *(reinterpret_cast<uint64*>(block) + 1)
#define block_mem(block) (reinterpret_cast<void**>(block) + 2)
#define block_end(block) (reinterpret_cast<ubyte*>(block) + block_size(block))
#define mem_block(mem) (reinterpret_cast<void**>(mem) - 2)
#define align_dword(n) n & 0x7 ? (n | 0x7) + 0x1 : n

void ListAllocator::init()
{
	// 16-Bytes header
	headerSize = 0x10;

	head = start;
	block_next(head) = nullptr;
	block_size(head) = size - headerSize;
}

void * ListAllocator::alloc(uint64 n)
{
	// Align to dword
	n = align_dword(n);

	// Find first free block
	void * it = head, * prev = nullptr;
	while (it && block_size(it) < n) prev = it, it = block_next(it);

	if (it)
	{
		// Split block
		void * block_end = block_offset(it, headerSize + n);
		
		if (block_size(it) - n > headerSize)
		{
			// Create new block
			block_next(block_end) = block_next(it);
			block_size(block_end) = block_size(it) - (n + 0x8);

			// Link new block
			if (prev) block_next(prev) = block_end; else head = block_end;
		}

		return block_mem(it);
	}

	// Out of memory
	return nullptr;
}

void ListAllocator::free(void * ptr)
{
	// We make the assumption was returned
	// by this very allocator

	// There's no head, make one
	if (!head)
		head = mem_block(ptr);
	else
	{
		// Get block start and end
		void	* block_start	= mem_block(ptr),
				* block_end		= block_end(block_start);

		// Iterator to find closest block
		void * it = head, * prev = nullptr;
		while (it && it < block_start) prev = it, it = block_next(it);

		if (!prev)
			head = block_start;
		else if (block_end(prev) == block_start)
		{
			// Merge with prev
			block_size(prev) += block_size(block_start) + 0x8;
			block_start = prev;
		}
		else
			// Link with prev
			block_next(prev) = block_start;

		if (block_end == it)
		{
			// Merge with next
			block_size(block_start) += block_size(it) + headerSize;
			block_next(block_start) = block_next(it);
		}
		else
			// Link with next
			block_next(block_start) = it;
	}
}

#undef block_offset
#undef block_next
#undef block_size
#undef block_mem
#undef block_end
#undef mem_block
#undef align_dword