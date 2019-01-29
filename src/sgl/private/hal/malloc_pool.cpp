#include "hal/malloc_pool.h"

void * MallocPool::malloc(sizet n, uint32 alignment)
{
	// Pool empty
	if (head == nullptr | n > blockSize) return nullptr;

	void * out = getBlock(head);
	head = getNext(head);

	--numFreeBlocks;
	return out;
}

void * MallocPool::realloc(void * original, sizet n, uint32 alignment)
{
	// We cannot really realloc
	if (n > blockSize)
		return nullptr;
	else
		return original;
}

void MallocPool::free(void * original)
{
	// Relink in free list
	original = getDescriptor(original);
	setNext(original, (void**)head);
	
	++numFreeBlocks;
	head = original;
}

bool MallocPool::getAllocSize(void * original, sizet & n)
{
	if (hasBlock(original))
	{
		n = blockSize;
		return true;
	}
	
	return false;
}