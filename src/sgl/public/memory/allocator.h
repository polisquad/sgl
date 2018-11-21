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
	inline void attachBuffer(void * start, void * end);
	/** @param [in] size buffer length */
	inline void attachBuffer(void * start, uint64 size);
	/** @} */

	/**
	 * @brief Detach buffer
	 * 
	 * @return detached buffer
	 */
	inline void * detachBuffer();

	/**
	 * @brief Return @c true if has block
	 * 
	 * @param [in] block pointer to block
	 * 
	 * @return @c true if block was (probably) allocated by this allocator
	 * @c false otherwise
	 */
	inline bool hasBlock(void * block);

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
	 * @brief Reset allocator
	 */
	virtual void reset() = 0;

	/**
	 * @brief Static creators
	 * 
	 * @param [in] size allocator buffer size
	 * 
	 * @return created allocator
	 */
	template<class A = Allocator, typename ... TArgs>
	static A * create(uint64 bufferSize, TArgs && ... args);
};

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

template<class A, typename ... TArgs>
A * Allocator::create(uint64 bufferSize, TArgs && ... args)
{
	static_assert(std::is_base_of<Allocator, A>::value, "A must derive from Allocator!");

	// Align buffer size
	bufferSize = (bufferSize | (sizeof(void*) - 0x1)) + 0x1;
	void * buffer;

	// Use an aligned malloc
	int32 status = posix_memalign(&buffer, sizeof(void*), bufferSize);
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