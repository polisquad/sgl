#ifndef SGL_AUTO_ALLOCATOR_H
#define SGL_AUTO_ALLOCATOR_H

#include "allocator.h"

/**
 * @struct AutoAllocator memory/auto_allocator.h
 * @brief A auto-resizable allocator
 */
template<class A>
struct AutoAllocator : public A
{
	static_assert(std::is_base_of<Allocator, A>::value, "A must derive from Allocator!");

protected:
	/**
	 * @brief Linked list of allocators
	 */
	struct AllocList
	{
		/**
		 * @brief Pointed allocator
		 */
		Allocator * allocator;

		/**
		 * @brief Next allocator
		 */
		AllocList * next;
	} * head;

public:
	/**
	 * @brief Inherit constructors
	 */
	using A::A;

	/**
	 * @copydoc Allocator::init()
	 * 
	 * @param [in] args arguments supplied
	 * to the inner allocator
	 */
	template<typename ... TArgs>
	inline void init(TArgs && ... args);

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

template<class A>
template<typename ... TArgs>
void AutoAllocator<A>::init(TArgs && ... args)
{
	// Create allocator on existing buffer
	A * allocator = new A(this->start, this->end);
	allocator->init(args ...);

	// Init allocators list
	head = new AllocList;
	head->allocator = allocator,
	head->next = nullptr;
}

template<class A>
void * AutoAllocator<A>::alloc(uint64 n)
{
	// Try to allocate with first allocator
	AllocList * it = head;
	while (it)
	{
		void * block = it->allocator->alloc(n);
		if (block) return block;

		// Next allocator
		it = it->next;
	}
	
	// Create a new allocator
	// with the same settings of the current
	// We're using default copy constructor
	A * allocator = new A(*reinterpret_cast<A*>(head->allocator));

	// Create a new buffer for the allocator
	void * buffer = nullptr;
	posix_memalign(&buffer, sizeof(void*), this->size);
	allocator->attachBuffer(buffer, this->size);
	allocator->reset();

	// Add to list as new head
	AllocList * prev = new AllocList;
	prev->allocator = allocator,
	prev->next = head;
	head = prev;

	// Finally, alloc with new allocator
	return allocator->alloc(n);
}

template<class A>
void AutoAllocator<A>::free(void * ptr)
{
	AllocList * it = head;
	while (it)
	{
		if (it->allocator->hasBlock(ptr))
		{
			it->allocator->free(ptr);
			break;
		}

		// Next allocator
		it = it->next;
	}
}

template<class A>
void AutoAllocator<A>::reset()
{
	// Reset all allocators
	AllocList * it = head;
	while (it)
	{
		it->allocator->reset();
		it = it->next;
	}
}

#endif