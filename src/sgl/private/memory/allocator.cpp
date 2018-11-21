#include "memory/allocator.h"

Allocator::Allocator() :
	start(nullptr),
	end(nullptr),
	size(0),
	headerSize(sizeof(void*)) {}

Allocator::Allocator(void * start, void * end) :
	start(start),
	end(end),
	size((uint64*)end - (uint64*)start),
	headerSize(sizeof(void*)) {}

Allocator::Allocator(void * start, uint64 size) :
	start(start),
	end((ubyte*)start + size),
	size(size),
	headerSize(sizeof(void*)) {}

Allocator::~Allocator()
{
	// Dealloc attached buffer
	if (start) ::free(start);
}