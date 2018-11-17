#include "memory/allocator.h"

Allocator::Allocator() : start(nullptr), end(nullptr), size((uint64)end - (uint64)start) {}

Allocator::Allocator(void * start, void * end) : start(start), end(end) {}

Allocator::Allocator(void * start, uint32 size) : start(start), end((ubyte*)start + size) {}