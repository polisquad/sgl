#ifndef SGL_CONTAINERS_H
#define SGL_CONTAINERS_H

#include "core_types.h"
#include "templates/enable_if.h"

/// Forward declarations
/// @{
template<typename> class Array;
template<typename> class Queue;
/// @}

/**
 * @namespace Containers
 * @brief Utilities for containers
 */
namespace Containers
{
	template<typename R, typename T>
	FORCE_INLINE auto enumerate(R (*_func)(T t, uint64 i), uint64 _i = 0)
	{
		struct {
			/// @brief Enumerate variable
			uint64 i;

			/// @brief Functor
			typedef R (*Func)(T, uint64);
			Func func;

			/// @brief Run func
			FORCE_INLINE R operator()(T t)
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
	template<class Container, typename Filter>
	Container filter(const Container & container, Filter filter);

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
	template<class Container, typename Map>
	Container map(const Container & container, Map func);
};

template<class Container, typename Filter>
Container Containers::filter(const Container & container, Filter filter)
{
	Container out;

	// Keep only elements that satisfy the filter
	for (const auto elem : container)
		if (filter(elem)) out += elem;

	return out;
};

template<class Container, typename Map>
Container Containers::map(const Container & container, Map func)
{
	Container out;

	// Apply function and insert in new container
	for (const auto elem : container)
		out += func(elem);

	return out;
}

#endif