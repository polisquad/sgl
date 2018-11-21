#ifndef SGL_FRAME_ALLOCATOR_H
#define SGL_FRAME_ALLOCATOR_H

#include "allocator.h"

/**
 * @struct FrameAllocator memory/frame_allocator.h
 * @brief The simplest allocator, can only be freed completely
 */
struct FrameAllocator : public Allocator
{
protected:
	/**
	 * @brief Stack pointer
	 */
	void * sp;

public:
	/**
	 * @brief Inherit constructors
	 */
	using Allocator::Allocator;

	/**
	 * @copydoc Allocator::init()
	 */
	inline void init();

	/**
	 * @copydoc Allocator::alloc()
	 */
	virtual inline void * alloc(uint64 n);

	/**
	 * @copydoc Allocator::free()
	 */
	virtual inline void free(void * ptr);

	/**
	 * @copydoc Allocator::reset()
	 */
	virtual inline void reset();
};

void FrameAllocator::init()
{
	// Set stack pointer
	sp = start;
}

void * FrameAllocator::alloc(uint64 n)
{
	// Align n
	n = (n & 0x7) ? (n | 0x7) + 0x1 : n;

	// Increment sp if possible
	void * fp = sp;
	sp = reinterpret_cast<ubyte*>(sp) + n;

	// Return allocated chunk
	if (sp < end) return fp;

	// Reset sp to fp
	sp = fp;
	return nullptr;
}

void FrameAllocator::free(void * ptr)
{
	// Reset stack pointer
	if (hasBlock(ptr)) sp = start;
}

void FrameAllocator::reset()
{
	// Reset sp
	sp = start;
}

#endif