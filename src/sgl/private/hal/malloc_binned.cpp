#include "hal/malloc_binned.h"

MallocBinned::MallocBinned() :
	buckets{},
	root(nullptr)
{
	// Create pools
	for (uint32 i = 0; i < MALLOC_BINNED_NUM_BUCKETS; ++i)
		createPool(i);
}

void * MallocBinned::malloc(sizet n, uint32 alignment)
{
	if (n > MALLOC_BINNED_BLOCK_MAX_SIZE)
		// Use global allocator
		return gMalloc->malloc(n, alignment);
	
	// Find bucket index
	uint32 bucketIdx = getBucketIndex(n);

	{
		// Try first pool
		PoolLinkRef head = buckets[bucketIdx];
		if (head->data.numFreeBlocks)
			return head->data.malloc(n);

		// Find next free pool
		PoolLinkRef it = head->next;
		while (it && it->data.numFreeBlocks == 0) it = it->next;

		if (it)
		{
			// Bring forth
			it->unlink();
			it->linkNext(head);
			buckets[bucketIdx] = it;

			// Allocate and return
			return it->data.malloc(n);
		}
		else
		{
			MallocPool * pool;
			if (pool = createPool(bucketIdx))
				return pool->malloc(n);
		}
	}

	return nullptr;
}

void * MallocBinned::realloc(void * original, sizet n, uint32 alignment)
{
	// Nothing to reallocate, literally nothing
	if (original == nullptr) return malloc(n, alignment);

	void * out;

	// Find pool that alloc'd this block

	// Search key
	Pair<void*, MallocPool*> searchKey(original);

	PoolNodeRef it = root;
	while (it)
	{
		if (searchKey < it->data)
			it = it->left;
		else
		{
			MallocPool * pool = it->data.second;
			if (pool->hasBlock(original))
			{
				if (n < pool->blockSize)
					// No need to reallocate
					return original;
				else
				{
					// Allocate new and copy memory
					out = malloc(n, alignment);
					Memory::memcpy(out, original, n);

					// Free original
					pool->free(original);

					return out;
				}
			}
			else
				it = it->right;
		}
	}

	printf("backup\n");

	// Memory was alloc'd by the backup allocator
	if (n > MALLOC_BINNED_BLOCK_MAX_SIZE)
		// Let the backup allocator handle realloc
		return gMalloc->realloc(original, n, alignment);
	else
	{
		// Allocate new out of pool
		out = malloc(n);
		Memory::memcpy(out, original, n);

		// Free original with backup allocator
		gMalloc->free(original);

		return out;
	}
}

void MallocBinned::free(void * original)
{
	// Use tree to quickly find pool
	
	// Search key
	Pair<void*, MallocPool*> searchKey(original);

	PoolNodeRef it = root;
	while (it)
	{
		if (searchKey < it->data)
			it = it->left;
		else
		{
			MallocPool * pool = it->data.second;
			if (pool->hasBlock(original))
			{
				pool->free(original);
				return;
			}
			else
				it = it->right;
		}
	}

	// Block was not alloc'd by pools
	// Free using global allocator
	gMalloc->free(original);
}

bool MallocBinned::getAllocSize(void * original, sizet & n)
{
	// Use tree to quickly find pool
	
	// Search key
	Pair<void*, MallocPool*> searchKey(original);

	PoolNodeRef it = root;
	while (it)
	{
		if (searchKey < it->data)
			it = it->left;
		else
		{
			MallocPool * pool = it->data.second;
			if (pool->hasBlock(original))
			{
				n = pool->blockSize;
				return true;
			}
			else
				it = it->right;
		}
	}

	return false;
}