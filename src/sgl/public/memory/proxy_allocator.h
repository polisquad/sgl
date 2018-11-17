#ifndef SGL_OBJECT_ALLOCATOR_H
#define SGL_OBJECT_ALLOCATOR_H

#include "allocator.h"

/**
 * @struct ProxyAllocator memory/proxy_allocator.h
 * @brief A proxy allocator for a specific object
 */
template<class T>
struct ProxyAllocator
{
protected:
	/**
	 * @brief The real allocator
	 */
	Allocator * allocator;

public:
	/**
	 * @brief Standard allocator type definitions
	 * @{
	 */
	typedef T			value_type;
	typedef T*			pointer;
	typedef const T*	const_pointer;
	/** @} */

public:
	/**
	 * Default-constructor
	 */
	inline ProxyAllocator();

	/**
	 * @brief Allocator-constructor
	 * 
	 * @param [in] allocator in-allocator
	 */
	inline ProxyAllocator(Allocator * allocator);

	/**
	 * @brief Allocate memory for one object
	 * 
	 * @return address of memory chunk
	 */
	inline pointer alloc();

	/**
	 * @brief Free previously allocated object
	 * 
	 * @param [in] ptr pointer to object
	 */
	inline void free(pointer ptr);
};

template<class T>
ProxyAllocator<T>::ProxyAllocator() : allocator(nullptr) {}

template<class T>
ProxyAllocator<T>::ProxyAllocator(Allocator * allocator) : allocator(allocator) {}

template<class T>
typename ProxyAllocator<T>::pointer ProxyAllocator<T>::alloc()
{
	// Delegate to allocator
	if (allocator) return static_cast<ProxyAllocator<T>::pointer>(allocator->alloc(sizeof(ProxyAllocator::value_type)));
	
	return nullptr;
}

template<class T>
void ProxyAllocator<T>::free(ProxyAllocator::pointer ptr)
{
	if (allocator) allocator->free(ptr);
}

#endif