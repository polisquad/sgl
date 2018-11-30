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

	/**
	 * @class View containers/array.h
	 * @brief A sliced view of an array
	 */
	class View
	{
		friend Array;

	protected:
		/// @brief Array buffer
		T * buffer;

		/// @brief Elements count
		uint64 count;

	public:
		/// @brief Get elements count
		FORCE_INLINE uint64 getCount() const { return count; }

		/// @brief Reference access
		FORCE_INLINE T & operator[](uint64 i) { return buffer[i]; }

		/// @brief Slice view
		FORCE_INLINE View operator()(uint64 start, uint64 end)
		{
			ASSERT(start < end, "Start cannot be greater than end");
			return View(buffer + start, end - start);
		}

		/// @brief Iterators
		/// @{
		FORCE_INLINE T * begin() { return buffer; }
		FORCE_INLINE const T * begin() const { return buffer; }
		
		FORCE_INLINE T * end() { return buffer + count; }
		FORCE_INLINE const T * end() const { return buffer + count; }
		/// @}

		/**
		 * Removed methods
		 * @{
		 */

		/// @brief Assignment-operator
		View & operator=(const View & view) = delete;

		/// @}
	protected:
		/// Constructors are not accessible
		/// From outside Array class
		
		/// @brief Default-constructor
		View() : buffer(nullptr), count(0) {}

		/// @brief Copy-constructor
		View(const View & view) : buffer(view.buffer), count(view.count) {}

		/// @brief Array-constructor
		View(T * _buffer, uint64 _count) : buffer(_buffer), count(_count) {}
	};

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
	/**
	 * @brief Default-constructor
	 * 
	 * Creates a new buffer with space for
	 * at least 2 elements (would not even
	 * be an array with 1 element, would it?)
	 * 
	 * @param [in] size initial buffer size (default: 2)
	 */
	FORCE_INLINE Array(sizet _size = 2, Malloc * _allocator = gMalloc) :
		allocator(_allocator),
		buffer(nullptr),
		size(PlatformMath::max(_size, sizet(2))),
		count(0)
	{
		// Allocate initial buffer
		buffer = reinterpret_cast<T*>(malloc(size * sizeof(T)));
	}

	/**
	 * @brief Destructor
	 * 
	 * Deallocates any existing buffer
	 */
	FORCE_INLINE ~Array()
	{
		// Deallocate buffer
		if (buffer) allocator->free(buffer);
	}

	/// @brief Get elements count
	FORCE_INLINE uint64 getCount() const { return count; }

	/// @brief Get array size
	FORCE_INLINE uint64 getSize() const { return size; }

	/// @brief Return true if array is empty
	FORCE_INLINE bool isEmpty() const { return count > 0; }

	/// @brief Return true if has valid buffer
	FORCE_INLINE bool hasBuffer() const { return buffer != nullptr; }

	/// @brief Return true if has valid buffer and size
	FORCE_INLINE bool isValid() const { return buffer != nullptr & size > 0; }

	/// @brief Random access operator
	FORCE_INLINE T & operator[](uint64 i) { return buffer[i]; }

	/**
	 * @brief Returns reference to i-th element or creates it
	 * 
	 * Like @ref operator[]() but if i > count
	 * the element is created to fullfill the request.
	 * The buffer may be resized to accomodate the
	 * requested element
	 */
	FORCE_INLINE T & operator()(uint64 i)
	{
		// Increment buffer size as much as necessary
		sizet _size = size;
		while (_size < i) _size *= 2;
		resize(_size);

		// Update count
		count = PlatformMath::max(count, i + 1);

		return buffer[i];
	}

	/**
	 * @brief Returns a slice of the array
	 * 
	 * The slice is returned as a custom view
	 * on the array. As such you may not alter
	 * the structure of the array from the view,
	 * but you can still alter its elements
	 * 
	 * @param [in] start,end start and end indices of the view
	 * 
	 * @return sliced view of the array
	 */
	FORCE_INLINE View operator()(uint64 start, uint64 end) const
	{
		ASSERT(start < end, "Start should be greater than end");
		return View(buffer + start, end - start);
	}

	/// @brief Iterators
	/// @{
	FORCE_INLINE Iterator begin() { return Iterator(buffer); }
	FORCE_INLINE ConstIterator begin() const { return ConstIterator(buffer); }

	FORCE_INLINE Iterator end() { return Iterator(buffer + count); }
	FORCE_INLINE ConstIterator end() const { return ConstIterator(buffer + count); }
	/// @}

	/**
	 * @brief Push element to the back
	 * 
	 * @param [in] elem element to insert
	 * @{
	 */
	/// @return modified index
	FORCE_INLINE uint64 push(const T & elem)
	{
		const uint64 i = count;
		if (size < ++count) resize(size * 2);

		// Construct element
		new (buffer + i) T(elem);

		// Return inserted index
		return i;
	}

	/// @return 
	FORCE_INLINE Array<T> & operator += (const T & elem)
	{
		push(elem);
		return *this;
	}
	/// @}
	
	/**
	 * @brief Push multiple element to the end
	 * 
	 * @param [in]	elems	buffer of elements
	 * @param [in]	n		number of elements
	 * 
	 * @return index of first inserted
	 * @{
	 */
	uint64 push(const T elems[], uint64 n);

	/**
	 * @brief Insert element in position
	 * 
	 * @param [in]	elem	element to insert
	 * @param [in]	i		element position
	 * 
	 * @return modified index
	 */
	uint64 insert(const T & elem, uint64 i = 0);

	/**
	 * @brief Insert a single element at specified position
	 * 
	 * @param [in]	elems	elements to insert
	 * @param [in]	n		number of elements
	 * @param [in]	i		insert index
	 * 
	 * @return modified index
	 */
	uint64 insert(const T elems[], uint64 n, uint64 i);

	/**
	 * @brief Append another array
	 * 
	 * @param [in] arr other array
	 * 
	 * @return self
	 * @{
	 */
	Array<T> & append(const Array<T> & arr);
	
	FORCE_INLINE Array<T> & operator+=(const Array<T> & arr)
	{
		append(arr);
		return *this;
	}
	/// @}

	/// @brief Remove all elements from the array
	FORCE_INLINE void empty() { count = 0; }

	/// @copybrief empty()
	/// Additionally, free and recreate buffer with size = 2
	FORCE_INLINE void reset()
	{
		count = 0;
		
		// Recreate buffer
		allocator->free(buffer);
		buffer = reinterpret_cast<T*>(allocator->malloc((size = 2) * sizeof(T)));
	}

	/**
	 * @brief Remove n elements at position i
	 * 
	 * @param [in]	i	starting index
	 * @param [in]	n	count of elements to remove
	 * 
	 * @return count of removed elements
	 */
	uint64 removeAt(uint64 i, uint64 n = 1);
	
	/**
	 * @brief Remove elements using a filter
	 * 
	 * The filter function receives as parameters
	 * the current element and the current index.
	 * It returns @c true to keep the element or
	 * @c false to discard it
	 * 
	 * @param [in]	filter	function that performs the filter
	 * 
	 * @return count of filtered elements
	 */
	template<typename Filter> uint64 filter(Filter filter);

	/**
	 * @brief Attach a new buffer to the array
	 * 
	 * @param [in] _buffer buffer to attach
	 * @{
	 */
	FORCE_INLINE void attachBuffer(T * _buffer) { buffer = _buffer; }

	/// @param [in] size size of the buffer
	FORCE_INLINE void attachBuffer(T * _buffer, sizet _size) { buffer = _buffer, size = _size; }
	/** @} */

	/// @brief Detach existing buffer
	FORCE_INLINE void detachBuffer() { buffer = nullptr; }

	/**
	 * @brief Clone array
	 * 
	 * @return cloned array
	 * @{
	 */
	FORCE_INLINE Array<T> clone()
	{
		// Clone buffer
		T * _buffer = reinterpret_cast<T*>(allocator->malloc(size * sizeof(T)));

		// Construct elements
		for (uint64 i = 0; i < count; ++i)
			new (_buffer + i) T(buffer[i]);
		
		return Array<T>(_buffer, size, count, allocator);
	}

	/// This version performs a memcpy over the buffer
	/// Objects that requires a copy constructor to be
	/// called should call the safe version
	FORCE_INLINE Array<T> cloneUnsafe()
	{
		// Clone buffer
		T * _buffer = reinterpret_cast<T*>(allocator->malloc(size * sizeof(T)));

		// Copy buffer
		PlatformMemory::memcpy(_buffer, buffer, size * sizeof(T));

		return Array<T>(_buffer, size, count, allocator);
	}
	/// @}

	/**
	 * @brief Create a sliced copy
	 * 
	 * @param [in] i,j slice range [i, j)
	 * 
	 * @return sliced copy
	 * @{
	 */
	Array<T> slice(uint64 start, uint64 end = 0)
	{
		// Allocate buffer
		const uint64 _count = end - start;
		sizet _size = 2;
		while (_size < _count) _size *= 2;
		T * _buffer = reinterpret_cast<T*>(allocator->malloc(_size * sizeof(T)));

		// Construct objects
		for (uint64 i = 0; i < _count; ++i)
			new (_buffer + i) T(buffer[start + i]);
		
		return Array<T>(_buffer, _size, _count, allocator);
	}

	/// This version uses memcpy instead of
	/// copy constructing each object
	Array<T> sliceUnsafe(uint64 start, uint64 end = 0)
	{
		// Allocate buffer
		const uint64 _count = end - start;
		sizet _size = 2;
		while (_size < _count) _size *= 2;
		T * _buffer = reinterpret_cast<T*>(allocator->malloc(_size * sizeof(T)));

		// Copy buffer
		PlatformMemory::memcpy(_buffer, buffer + start, _count * sizeof(T));
		
		return Array<T>(_buffer, _size, _count, allocator);
	}

protected:
	/**
	 * @brief Buffer-constructor
	 * 
	 * Constructs an array with a given buffer
	 * 
	 * @param [in]	_buffer		input buffer
	 * @param [in]	_size		size of the given buffer
	 * @param [in]	_allocator	in allocator
	 */
	FORCE_INLINE Array(T * _buffer, uint64 _size, uint64 _count = 0, Malloc * _allocator = gMalloc) :
		allocator(_allocator),
		buffer(_buffer),
		size(_size),
		count(_count) {}

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
uint64 Array<T>::push(const T elems[], uint64 n)
{
	const uint64 i = count;

	// Resize if necessary
	count += n;
	sizet _size = size;
	while (count > _size) _size *= 2;
	resize(_size);

	// Construct objects
	for (uint64 j = i, k = 0; k < n; ++j, ++k)
		new (buffer + j) T(elems[k]);
	
	// Return modified index
	return i;
}

template<typename T>
uint64 Array<T>::insert(const T & elem, uint64 i)
{
	if (i < count)
	{
		// Resize if necessary
		if (++count > size) resize(size * 2);
		
		// Move all trailing elements
		memmove(buffer + i + 1, buffer + i, (count - i) * sizeof(T));
	}
	else
	{
		count = i + 1;

		// Resize if necessary
		sizet _size = size;
		while (count > _size) _size *= 2;
		resize(_size);
	}

	// Construct element
	new (buffer + i) T(elem);

	// Return modified index
	return i;
}

template<typename T>
uint64 Array<T>::insert(const T elems[], uint64 n, uint64 i)
{
	if (i < count)
	{
		count += n;

		// Resize if necessary
		sizet _size = size;
		while (count > _size) _size *= 2;
		resize(_size);
		
		// Move all trailing elements
		memmove(buffer + i + n, buffer + i, (count - (i + n)) * sizeof(T));
	}
	else
	{
		count = i + n;

		// Resize if necessary
		sizet _size = size;
		while (count > _size) _size *= 2;
		resize(_size);
	}

	// Construct elements
	for (uint64 j = i, k = 0; k < n; ++j, ++k)
		new (buffer + j) T(elems[k]);

	// Return modified index
	return i;
}

template<typename T>
Array<T> & Array<T>::append(const Array<T> & arr)
{
	const uint64 i = count;
	count += arr.count;

	// Resize if necessary
	sizet _size = size;
	while (_size < count) _size *= 2;
	resize(_size);

	// Copy elements
	PlatformMemory::memcpy(buffer + i, arr.buffer, arr.count * sizeof(T));
	return *this;
}

template<typename T>
uint64 Array<T>::removeAt(uint64 i, uint64 n)
{
	if (i < count)
	{
		// Calculate removed
		const uint64 removed = n < count - i ? n : count - i;

		// Move trailing elements
		memmove(buffer + i, buffer + i + n, (count - (i + removed)) * sizeof(T));

		// Update count
		count -= removed;
		return removed;
	}

	return 0;
}

template<typename T>
template<typename Filter>
uint64 Array<T>::filter(Filter filter)
{
	uint64 j = 0;

	// Filter elements
	for (uint64 i = 0; i < count; ++i)
		if (filter(buffer[i])) buffer[j++] = buffer[i];
	
	// Update count
	const uint64 removed = count - j;
	count = j;
	return removed;
}

#endif