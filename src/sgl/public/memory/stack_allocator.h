#ifndef SGL_STACK_ALLOCATOR_H
#define SGL_STACK_ALLOCATOR_H

#include "allocator.h"

/**
 * @struct StackAllocator memory/stack_allocator.h
 * @brief Allocates memory in a stack fashion way
 */
struct StackAllocator : public Allocator
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
	 * @brief Initialize allocator
	 */
	void init();

	/**
	 * @copydoc Allocator::alloc
	 */
	virtual void * alloc(uint32 n);
	
	/**
	 * @copydoc Allocator::free
	 */
	virtual void free(void * ptr);
};

#endif