#ifndef SGL_ARRAY_H
#define SGL_ARRAY_H

#include "core_types.h"
#include "hal/platform_stdlib.h"
#include "hal/platform_math.h"
#include "hal/platform_memory.h"

/**
 * @class Array containers/array.h
 * @brief A dynamic array
 */
template<typename T>
class Array
{
public:
	/// Iterators are implemented as simple pointers
	typedef T*			Iterator;
	typedef const T*	ConstIterator;

protected:
	/// @brief Allocator in use
	Malloc * allocator;

	/// @brief Element buffer
	T * buffer;

	/// @brief Current size of buffer
	sizet size;

	/// @brief Actual number of elements
	uint64 count;

public:
	/// @brief Default-constructor
	FORCE_INLINE Array(sizet _count = 2, Malloc * _allocator = gMalloc);

	/// @brief Random access operator
	FORCE_INLINE T & operator[](uint64 i) { ASSERT(i < count, "Random access out of bounds!"); return buffer[i]; }

	/**
	 * @brief Same as @ref operator[] but
	 * extends buffer to requested indices
	 * if out of bounds encoutered
	 * 
	 * @param [in] i access position
	 * 
	 * @return reference to the element
	 */
	FORCE_INLINE T & operator()(uint64 i)
	{
		// Increment buffer size as much as necessary
		while (i >= size) resize(size * 2);
		count = PlatformMath::max(count, i + 1);

		return *(buffer + i);
	}

	/// @brief Get elements count
	FORCE_INLINE uint64 getCount() const { return count; }

	/// @brief Get array size
	FORCE_INLINE uint64 getSize() const { return size; }

	/// @brief Return true if array is empty
	FORCE_INLINE bool isEmpty() const { return count > 0; }

	/// @brief Return iterator from the first element
	/// @{
	FORCE_INLINE Iterator begin() { return Iterator(buffer); }
	FORCE_INLINE Iterator begin() const { return ConstIterator(buffer); }
	/// @}

	/// @brief Return iterator from the last element
	/// @{
	FORCE_INLINE Iterator end() { return Iterator(buffer + count); }
	FORCE_INLINE Iterator end() const { return ConstIterator(buffer + count); }
	/// @}

	/**
	 * @brief Push element to the back
	 * 
	 * @param [in] elem element to insert
	 * 
	 * @return reference to inserted element
	 */
	T & push(const T & elem);

	/**
	 * @brief Insert element in position
	 * 
	 * @param [in]	elem	element to insert
	 * @param [in]	i		element position
	 * 
	 * @return reference to inserted element
	 */
	T & insert(const T & elem, uint64 i = 0);

	/**
	 * @brief Remove element at position
	 * 
	 * @param [in] i element position
	 */
	void removeAt(uint64 i);

	/**
	 * @brief Append another array
	 * 
	 * @param [in] arr other array
	 * 
	 * @return self
	 */
	Array<T> & append(const Array<T> & arr);

	/**
	 * @brief Reset array buffer
	 * @note Effectively removes all elements
	 */
	FORCE_INLINE void reset()
	{
		// Deallocate buffer
		allocator->free(buffer);

		// Reallocate a small buffer
		count = 0, size = 2, buffer = reinterpret_cast<T*>(allocator->malloc(2));
	}

	/**
	 * @brief Swap elements at indices
	 * 
	 * @brief i,j elements to swap
	 */
	FORCE_INLINE void swap(uint64 i, uint64 j)
	{
		// Swap elements
		T temp		= buffer[i];
		buffer[i]	= buffer[j];
		buffer[j]	= temp;
	}

protected:
	/// @brief Resize buffer
	FORCE_INLINE bool resize(sizet n)
	{
		// Resize only if requested size is bigger
		if (n > size)
		{
			buffer = reinterpret_cast<T*>(allocator->realloc(buffer, n * sizeof(T)));
			assert(buffer);

			size = n;
			return true;
		}

		return false;
	}
};

template<typename T>
Array<T>::Array(sizet _count, Malloc * _allocator) :
	allocator(_allocator),
	buffer(nullptr),
	size(PlatformMath::max(_count * 2, (sizet)2)),
	count(0)
{
	// Create initial buffer
	buffer = reinterpret_cast<T*>(allocator->malloc(size * sizeof(T)));
}

template<typename T>
T & Array<T>::push(const T & elem)
{
	// Resize if necessary
	const uint64 i = count;
	if (++count > size) resize(size * 2);
	new (buffer + i) T(elem);

	return *(buffer + i);
}

template<typename T>
T & Array<T>::insert(const T & elem, uint64 i)
{
	// Resize if necessary
	if (++count > size) resize(size * 2);

	// Move forward
	if (i < count - 1)
		PlatformMemory::memmove(buffer + i + 1, buffer + i, (count - i) * sizeof(T));

	// Create object
	new (buffer + i) T(elem);
	return *(buffer + i);
}

template<typename T>
void Array<T>::removeAt(uint64 i)
{
	ASSERT(count - i > 0, "Cannot remove element out of bounds");
	// No resize involved here
	// Just a memory move
	PlatformMemory::memmove(buffer + i, buffer + i + 1, count - i);
	--count;
}

template<typename T>
Array<T> & Array<T>::append(const Array<T> & arr)
{
	// Resize buffer if necessary
	if (count + arr.count > size) resize(size + arr.size);

	// Then copy elements inside this
	PlatformMemory::memcpy(buffer + count, arr.buffer, arr.count * sizeof(T));
	count += arr.count;

	return *this;
}

#endif