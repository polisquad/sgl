#pragma once

#include "core_types.h"
#include "containers.h"
#include "templates/const_ref.h"
#include "templates/is_trivially_copyable.h"
#include "templates/functional.h"

namespace Container
{
	/**
	 * Sorting algorithms
	 */
	enum SortingAlg
	{
		INSERTION = 0,
		MERGESORT,
		QUICKSORT
	};

	/**
	 * @class SortingClass containers/sorting.h
	 * 
	 * Unfortunately C++ doesn't support function
	 * templates partial specialization, thus we
	 * have to 'hack' our way around with a wrapper
	 * class
	 */
	template<SortingAlg = QUICKSORT>
	struct SortingClass
	{
		template<typename CompareT = Compare, typename It>
		FORCE_INLINE static void sort(It begin, It end);
	};

	/**
	 * Perform sorting in place on iterator range
	 * 
	 * @param [in] begin,end begin and end iterators
	 */
	template<typename CompareT = Compare, SortingAlg alg = QUICKSORT, typename It>
	FORCE_INLINE void sort(It begin, It end)
	{
		SortingClass<alg>::template sort<CompareT, It>(begin, end);
	}
} // Container

template<>
template<typename CompareT, typename It>
void Container::SortingClass<Container::QUICKSORT>::sort(It begin, It end)
{
	// Partition array around pivot value
	It first = begin; if (first == end | ++begin == end) return;

	// Partition code was limiting me
	It pivot = first;
	for (It i = begin; i != end; ++i)
		if (CompareT().template operator()<decltype(*i), decltype(*first)>(*i, *first) < 0) swap(*i, *(pivot = begin)), ++begin;
	
	// Swap pivot
	swap(*first, *pivot);

	// Divide and conquer
	sort<CompareT, It>(first, pivot);
	sort<CompareT, It>(begin, end);
}