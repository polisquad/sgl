#ifndef SGL_MAP_INTERFACE_H
#define SGL_MAP_INTERFACE_H

#include "pair.h"

/**
 * @class IMap containers/map_interface
 */
template<typename KeyT, typename ValT>
class IMap
{
public:
	/// The pair type
	typedef KVPair<KeyT, ValT> PairT;

public:
	/**
	 * @brief Access value with key
	 * 
	 * If an entry associated with the given key
	 * is found, the @ref KVPair is returned.
	 * If no entry is found, a new one is created
	 * on the fly and a reference to the pair is
	 * returned
	 * 
	 * @param [in] key search key
	 * 
	 * @return Found pair, or new one
	 */
	virtual ValT & operator[](const KeyT & key) = 0;

	/**
	 * @brief Insert pair in map
	 * 
	 * @param [in] pair key-value pair to insert
	 */
	virtual PairT insert(const PairT & pair) = 0;

	/// @brief Like @ref insert(const PairT&)
	FORCE_INLINE PairT insert(const KeyT & key, const ValT & val) { return insert(PairT(key, val)); }

	/// @brief Utility function to create a pair without templates
	static FORCE_INLINE PairT makePair(const KeyT & key, const ValT & val) { return PairT(key, val); }
};

#endif