#ifndef SGL_PAIR_H
#define SGL_PAIR_H

#include "core_types.h"

/**
 * @class Pair containers/pair.h
 * 
 * A pair of values
 */
template<typename A, typename B>
class Pair
{
public:
	/// First element
	A first;

	/// Second element
	B second;

public:
	/// Default-constructor
	Pair() = default;

	/// Explicit copy-constuctor
	FORCE_INLINE Pair(const Pair<A, B> & p) :
		first(p.first),
		second(p.second) {}

	/// Elements-constructor
	FORCE_INLINE Pair(const A & _first, const B & _second) :
		first(_first),
		second(_second) {}

	/// Partial-constructor
	FORCE_INLINE Pair(const A & _first) : first(_first) {}

	/// Explicit default assignment-operator
	Pair & operator=(const Pair<A, B> & p) = default;

	/// Equality operators
	/// @{
	FORCE_INLINE bool operator==(const Pair<A, B> & p) const { return first == p.first & second == p.second; }
	FORCE_INLINE bool operator!=(const Pair<A, B> & p) const { return first != p.first | second != p.second; }
	/// @}
};

/**
 * @class KVPair containers/pair.h
 * 
 * A pair with a key and a value.
 * Enables ordering of pairs by
 * comparing keys
 */
template<typename KeyT, typename ValT>
class KVPair : public Pair<KeyT, ValT>
{
public:
	/// Default-constructor
	KVPair() = default;

	/// Inherit constructors
	using Pair<KeyT, ValT>::Pair;

	/// Return key
	FORCE_INLINE const KeyT & getKey() const { return this->first; }

	/// Return value
	FORCE_INLINE ValT & getValue() { return this->second; }

	/**
	 * Enforce ordering between pairs using key
	 * 
	 * @param [in] p other key-value pair
	 * @return result of comparison
	 * @{
	 */
	FORCE_INLINE bool operator< (const KVPair<KeyT, ValT> & p) const	{ return this->first < p.first; }
	FORCE_INLINE bool operator> (const KVPair<KeyT, ValT> & p) const	{ return this->first > p.first; }
	FORCE_INLINE bool operator<=(const KVPair<KeyT, ValT> & p) const	{ return this->first <= p.first; }
	FORCE_INLINE bool operator>=(const KVPair<KeyT, ValT> & p) const	{ return this->first >= p.first; }
	/// @}
};

#endif