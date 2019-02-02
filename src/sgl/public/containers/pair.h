#ifndef SGL_PAIR_H
#define SGL_PAIR_H

#include "core_types.h"
#include "templates/const_ref.h"

/**
 * @class Pair containers/pair.h
 * 
 * A pair of values
 */
template<typename A, typename B>
class Pair
{
public:
	union
	{
		struct {
			/// First element
			A first;

			/// Second element
			B second;
		};

		struct {
			/// Pair key
			A key;

			/// Pair value
			B val;
		};
	};
	

public:
	/// Pair constructor
	FORCE_INLINE Pair(typename ConstRef<A>::Type _first = A(), typename ConstRef<B>::Type _second = B()) :
		first(_first),
		second(_second) {}

	/// Equality operators
	/// @{
	FORCE_INLINE bool operator==(const Pair<A, B> & other) const { return first == other.first & second == other.second; }
	FORCE_INLINE bool operator!=(const Pair<A, B> & other) const { return first != other.first | second != other.second; }
	FORCE_INLINE bool operator< (const Pair<A, B> & other) const { return first < other.first; }
	FORCE_INLINE bool operator> (const Pair<A, B> & other) const { return first > other.first; }
	FORCE_INLINE bool operator<=(const Pair<A, B> & other) const { return first <= other.first; }
	FORCE_INLINE bool operator>=(const Pair<A, B> & other) const { return first >= other.first; }
	/// @}
};

#endif