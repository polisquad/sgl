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
	virtual void init();

	/**
	 * @brief Allocate N bytes
	 * 
	 * @param [in] n number of bytes to allocate
	 * 
	 * @return pointer to the allocated chunk
	 */
	virtual void * alloc(uint32 n);
	
	/**
	 * @brief Free previously allocated memory
	 * 
	 * @param [in] ptr pointer to allocated memory
	 */
	virtual void free(void * ptr);
};

#endif