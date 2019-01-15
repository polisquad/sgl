#pragma once

#include "core_types.h"

/**
 * @class Pair containers/pair.h
 * @brief A pair of values
 */
template<typename A, typename B>
class Pair
{
public:
	/// @brief First element
	A first;

	/// @brief Second element
	B second;

public:
	/// @brief Default-constructor
	Pair() = default;

	/// @brief Explicit copy-constuctor
	FORCE_INLINE Pair(const Pair<A, B> & p) :
		first(p.first),
		second(p.second) {}

	/// @brief Elements-constructor
	FORCE_INLINE Pair(const A & _first, const B & _second) :
		first(_first),
		second(_second) {}

	/// @brief Partial-constructor
	FORCE_INLINE Pair(const A & _first) : first(_first) {}

	/// @brief Explicit default assignment-operator
	Pair & operator=(const Pair<A, B> & p) = default;

	/// @brief Equality operators
	/// @{
	FORCE_INLINE bool operator==(const Pair<A, B> & p) const { return first == p.first & second == p.second; }
	FORCE_INLINE bool operator!=(const Pair<A, B> & p) const { return first != p.first | second != p.second; }
	/// @}
};

/**
 * @class KVPair containers/pair.h
 * @brief A pair with a key and a value
 */
template<typename KeyT, typename ValT>
class KVPair : public Pair<KeyT, ValT>
{
public:
	/// @brief Default-constructor
	KVPair() = default;

	/// Inherit constructors
	using Pair<KeyT, ValT>::Pair;

	/// @brief Return key
	FORCE_INLINE const KeyT & getKey() { return this->first; }

	/// @brief Return value
	FORCE_INLINE ValT & getValue() { return this->second; }

	/**
	 * @brief Enforce ordering between pairs using key
	 * 
	 * @param [in] p other key-value pair
	 * 
	 * @return result of comparison
	 * @{
	 */
	FORCE_INLINE bool operator<(const KVPair<KeyT, ValT> & p) const { return this->first < p.first; }
	FORCE_INLINE bool operator<=(const KVPair<KeyT, ValT> & p) const { return this->first <= p.first; }
	FORCE_INLINE bool operator>(const KVPair<KeyT, ValT> & p) const { return this->first > p.first; }
	FORCE_INLINE bool operator>=(const KVPair<KeyT, ValT> & p) const { return this->first >= p.first; }
	/// @}
};

