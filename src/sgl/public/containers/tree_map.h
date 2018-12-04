#ifndef SGL_TREE_MAP_H
#define SGL_TREE_MAP_H

#include "map_interface.h"
#include "binary_tree.h"

/**
 * @class TreeMap containers/tree_map.h
 * @brief A map built on top of a binary tree
 * 
 * A tree map has an optimal space-efficiency. Keys
 * are sorted and all basic operations (insertion, deletion,
 * search) are O(log(n))
 */
template<typename KeyT, typename ValT>
class TreeMap : public IMap<KeyT, ValT>
{
public:
	/// Pair type definition
	typedef typename TreeMap::PairT PairT;

protected:
	/// Binary tree type definition
	typedef BinaryTree<PairT> TreeT;

	/// @brief Binary tree used for entry storage
	TreeT tree;

public:
	/// @brief Default-constructor
	TreeMap() = default;

	/// @copydoc IMap::operator[]
	virtual FORCE_INLINE PairT & operator[](const KeyT & key)
	{
		auto iter = tree.find(key);
		if (iter != tree.end())
			// Return found value
			return *iter;
		else
		{
			// Insert new one
		}
	}

	/// @copydoc IMap::insert()
	virtual FORCE_INLINE void insert(const PairT & pair)
	{
		// Insert in tree
		tree += pair;
	}
};

#endif