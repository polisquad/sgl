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

void Allocator::attachBuffer(void * start, void * end)
{
	this->start = start, this->end = end, this->size = (uint64)end - (uint64)start;
}

void Allocator::attachBuffer(void * start, uint64 size)
{
	this->start = start, this->end = reinterpret_cast<ubyte*>(start) + size, this->size = size;
}

void * Allocator::detachBuffer()
{
	void * buffer = start;
	start = end = nullptr, size = 0;
	return buffer;
}

bool Allocator::hasBlock(void * block)
{
	return block > start & block < end;
}