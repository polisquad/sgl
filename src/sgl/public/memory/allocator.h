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
	uint32 headerSize;

public:
	/**
	 * @brief Default-constructor
	 */
	Allocator();

	/**
	 * @brief Buffer-constructor
	 * 
	 * @param [in] start pointer to buffer start
	 * @{
	 */
	/** @param [in] end pointer to buffer end */
	Allocator(void * start, void * end);
	/** @param [in] size buffer length */
	Allocator(void * start, uint64 size);
	/** @} */

	/**
	 * @brief Destruct allocator and dealloate attached buffer
	 */
	virtual ~Allocator();

	/**
	 * @brief Attach buffer
	 * 
	 * @param [in] start pointer to buffer start
	 * @{
	 */
	/** @param [in] end pointer to buffer end */
	void attachBuffer(void * start, void * end);
	/** @param [in] size buffer length */
	void attachBuffer(void * start, uint64 size);
	/** @} */

	/**
	 * @brief Detach buffer
	 * 
	 * @return detached buffer
	 */
	void * detachBuffer();

	/**
	 * @brief Return @c true if has block
	 * 
	 * @param [in] block pointer to block
	 * 
	 * @return @c true if block was (probably) allocated by this allocator
	 * @c false otherwise
	 */
	bool hasBlock(void * block);

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
	virtual void * alloc(uint64 n) = 0;
	
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
	static A * create(uint32 bufferSize, uint32 alignment, TArgs && ... args);
};

template<class A, typename ... TArgs>
A * Allocator::create(uint32 bufferSize, uint32 alignment, TArgs && ... args)
{
	// A min alignment of sizeof(void*) is required
	alignment = alignment < sizeof(void*) ? sizeof(void*) : alignment;

	// Align buffer size
	bufferSize = (bufferSize | (alignment - 0x1)) + 0x1;
	void * buffer;

	// Use an aligned malloc
	int32 status = posix_memalign(&buffer, alignment, bufferSize);
	if (status == 0x0)
	{
		// Create allocator
		A * allocator = new A(buffer, bufferSize);

		// Init allocator with supplied arguments
		allocator->init(std::forward<TArgs>(args) ...);

		return allocator;
	}

	return nullptr;
}

#endif