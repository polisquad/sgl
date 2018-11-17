#include "memory/stack_allocator.h"

void StackAllocator::init()
{
	// Set sp to start of stack
	sp = start;
}

void * StackAllocator::alloc(uint32 n)
{
	// Align to 16-bytes
	if (n & 0xf) n = (n | 0xf) + 0x1;

	// Save frame pointer
	void * fp = sp;
	sp = static_cast<ubyte*>(sp) + n;

	if (static_cast<ubyte*>(sp) + sizeof(uint32) < end)
	{
		// Store allocated size
		*static_cast<uint32*>(sp) = n;
		sp = static_cast<ubyte*>(sp) + sizeof(uint32);

		// Return pointer to chunk
		return fp;
	}

	return nullptr;
}

void StackAllocator::free(void * ptr)
{
	if (sp != start)
	{
		sp = static_cast<ubyte*>(sp) - sizeof(uint32);
		const uint32 size = *static_cast<uint32*>(sp);
		sp = static_cast<ubyte*>(sp) - size;
	}
}