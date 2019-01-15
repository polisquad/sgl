#pragma once

#include "core_types.h"
#include "hal/platform_memory.h"
#include "templates/enable_if.h"
#include "templates/is_container.h"
#include "templates/is_trivially_copyable.h"

/// Forward declarations
/// @{
template<typename> class Array;
template<typename> class Queue;
/// @}

namespace
{
	/// @brief Swap two elements
	/// @{
	template<typename T>
	FORCE_INLINE EnableIfT<IsTriviallyCopyableV(T), void> swap(T & a, T & b)
	{
		// Swap memory chunks
		Memory::memswap(&a, &b, sizeof(T));
	}
	template<typename T>
	FORCE_INLINE EnableIfT<!IsTriviallyCopyableV(T), void> swap(T & a, T & b)
	{
		// We resort to user specified class method
		a.swap(b);
	}
	/// @}
};

/**
 * @namespace Containers
 * @brief Utilities for containers
 */
namespace Containers
{
	template<typename RetT, typename T>
	FORCE_INLINE auto enumerate(RetT (*_func)(T t, uint64 i), uint64 _i = 0)
	{
		struct {
			/// @brief Enumerate variable
			uint64 i;

			/// @brief Functor
			typedef RetT (*Func)(T, uint64);
			Func func;

			/// @brief Run func
			FORCE_INLINE RetT operator()(T t)
			{
				// Call function
				return func(t, i++);
			}
		} out = {.i = _i, .func = _func};

		return out;
	}

	/**
	 * @brief Filter container with a predicate
	 * 
	 * The Container class is assumed to overload the
	 * @c operator+= method (to insert non-filtered
	 * elements). Filter is assumed to be a callable
	 * object (functor, lambda expression) or a class
	 * or struct that overloads the @c operator() method
	 * 
	 * @param [in]	container	container to filter
	 * @param [in]	filter		predicate
	 * 
	 * @return container of same type filtered
	 */
	template<class ContainerT, typename FilterFunc>
	ContainerT filter(const ContainerT & container, FilterFunc filter);

	/**
	 * @brief Map a function on a container
	 * 
	 * The function can be any callable type (lambdas,
	 * functors) or any struct or class that overloads
	 * the @c operator() method
	 * 
	 * @param [in]	container	container to map
	 * @param [in]	func		map function
	 * 
	 * @return container of same type with map applied
	 */
	template<class ContainerT, typename MapFunc>
	ContainerT map(const ContainerT & container, MapFunc func);

	/**
	 * @brief Partition container elements using a pivot value
	 * 
	 * This function operates in-place and requires the container
	 * to have a forward iterator
	 * 
	 * @param [in]	begin	container start
	 * @param [in]	end		container end
	 * @param [in]	pivot	pivot value
	 * 
	 * @return position of pivot value
	 */
	template<typename IterT, typename T>
	IterT partition(const IterT & begin, const IterT & end, const T & pivot);

	/**
	 * @brief Performs heap-sort sorting
	 * 
	 * @param [in]	begin	container's start
	 * @param [in]	end		container's end
	 */
	template<typename IterT>
	void sort(const IterT & begin, const IterT & end);

	/// @see sort()
	template<typename ContainerT>
	FORCE_INLINE void sort(ContainerT & container) { sort(container.begin(), container.end()); }
};

template<class ContainerT, typename FilterFunc>
ContainerT Containers::filter(const ContainerT & container, FilterFunc filter)
{
	ContainerT out;

	// Keep only elements that satisfy the filter
	for (const auto elem : container)
		if (filter(elem)) out += elem;

	return out;
};

template<class ContainerT, typename MapFunc>
ContainerT Containers::map(const ContainerT & container, MapFunc func)
{
	ContainerT out;

	// Apply function and insert in new container
	for (const auto elem : container)
		out += func(elem);

	return out;
}

template<typename IterT, typename T>
IterT Containers::partition(const IterT & begin, const IterT & end, const T & pivot)
{
	IterT i = begin, j = begin;
	IterT prev = begin;
	for (; i != end; ++i)
	{
		if (*i <= pivot)
		{
			prev = j;
			j++;

			// Swap elements
			swap(*i, *prev);
		}
	}

	return prev;
}

template<typename IterT>
void Containers::sort(const IterT & begin, const IterT & end)
{
	if (LIKELY(begin != end))
	{
		auto pivot = begin; ++pivot;
		if (pivot != end)
		{
			// Partition using second element as pivot
			auto pivot = partition(begin, end, *begin);

			// Swap pivot and begin
			swap(*begin, *pivot);

			// Next, recursive call on left and right branch
			sort(begin, pivot);
			sort(++pivot, end);
		}
	}
}

