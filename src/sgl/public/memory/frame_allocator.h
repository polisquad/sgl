#ifndef SGL_FRAME_ALLOCATOR_H
#define SGL_FRAME_ALLOCATOR_H

#include "core_types.h"

/**
 * @class FrameAllocator memory/frame_allocator.h
 * @brief A simple allocator that accumulates memory
 * in a stack fashion and release then entire buffer
 * at the end of the frame
 */
class FrameAllocator
{
protected:
	/// @brief Stack pointer
	uintP sp;

public:
	/// @brief Default-constructor
	FrameAllocator();

	/// @copydoc Allocator::alloc()
	void FORCE_INLINE * alloc(uintP size);

	/// @copydoc Allocator::free()
	void FORCE_INLINE free(void * original);
};

void * FrameAllocator::alloc(uintP size)
{
	const void * out = buffer + sp;
	sp += size;
	
	return sp < 0 ? out : nullptr;
}

void FrameAllocator::free(void * original)
{
	// Reset sp
	sp = 0;
}

#endif