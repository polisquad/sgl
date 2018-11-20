#ifndef SGL_ALLOCATOR_H
#define SGL_ALLOCATOR_H

#include "core/platform.h"

/**
 * @struct Allocator memory/allocator.h
 * @brief Base class for all allocators
 */
struct Allocator
{
protected:
	/**
	 * @brief Memory buffer bounds
	 * @{
	 */
	void * start, * end;
	/** @} */

	/**
	 * @brief Buffer size
	 */
	uint64 size;

public:
	/**
	 * @brief Header size
	 */
	static const uint32 headerSize;

public:
	/**
	 * @brief Default-constructor
	 */
	Allocator();

	/**
	 * @brief Buffer-constructor
	 * 
	 * @param [in]	start	pointer to buffer start
	 * @param [in]	end		pointer to buffer end or buffer length
	 * @{
	 */
	Allocator(void * start, void * end);
	Allocator(void * start, uint32 size);
	/** @} */

	/**
	 * @brief Initialize allocator
	 */
	void init();

	/**
	 * @brief Allocate N bytes
	 * 
	 * @param [in] n number of bytes to allocate
	 * 
	 * @return pointer to the allocated chunk
	 */
	virtual void * alloc(uint32 n) = 0;
	
	/**
	 * @brief Free previously allocated memory
	 * 
	 * @param [in] ptr pointer to allocated memory
	 */
	virtual void free(void * ptr) = 0;

	/**
	 * @brief Static creators
	 * 
	 * @param [in] size allocator buffer size
	 * 
	 * @return created allocator
	 */
	template<class A = Allocator, typename ... TArgs>
	static A * create(uint32 bufferSize, TArgs && ... args);
};

template<class A, typename ... TArgs>
A * Allocator::create(uint32 bufferSize, TArgs && ... args)
{
	// Create buffer
	// Default size alignment to 16 bytes
	if (bufferSize & 0xf) bufferSize = (bufferSize | 0xf) + 0x1;
	void * buffer = malloc(bufferSize);

	// Create allocator
	A * allocator = new A(buffer, bufferSize);
	allocator->init(std::forward<TArgs>(args) ...);

	return allocator;
}

#endif