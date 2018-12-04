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
	/// @brief Access value with key
	virtual PairT & operator[](const KeyT & key) = 0;

	/**
	 * @brief Insert pair in map
	 * 
	 * @param [in] pair key-value pair to insert
	 */
	virtual void insert(const PairT & pair) = 0;

	/// @brief Utility function to create a pair without templates
	static FORCE_INLINE PairT makePair(const KeyT & key, const ValT & val) { return PairT(key, val); }
};

#endif