#pragma once

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

	/// Typedefs binary tree iterators
	/// @{
	typedef typename BinaryTree<PairT>::Iterator		Iterator;
	typedef typename BinaryTree<PairT>::ConstIterator	ConstIterator;
	/// @}

protected:
	/// Binary tree type definition
	typedef BinaryTree<PairT> TreeT;

	/// @brief Binary tree used for entries storage
	TreeT tree;

public:
	/// @brief Default-constructor
	TreeMap() = default;

	/// @brief Specify tree allocator
	TreeMap(Malloc * _allocator) : tree(_allocator) {}

	/// @todo Define copy-constructor and assignment operator

	/// @copydoc IMap::operator[]
	virtual FORCE_INLINE ValT & operator[](const KeyT & key) override
	{
		auto iter = tree.find(PairT(key)); // Note that it can also automatically convert key to a @ref KVPair using implicit conversion constructor
		if (iter != tree.end())
			// Return found value
			return (*iter).second;
		else
			// Insert new one
			return (tree.insertUnique(PairT(key))).second;
	}

	/// @brief Iterators simply delegates responsability to tree
	/// @{
	FORCE_INLINE Iterator find(const KeyT & key)	{ return tree.find(PairT(key)); }
	FORCE_INLINE Iterator end()						{ return tree.end(); }

	FORCE_INLINE ConstIterator find(const KeyT & key) const	{ return tree.find(PairT(key)); }
	FORCE_INLINE ConstIterator end() const					{ return tree.end(); }
	/// @}

	/// @copydoc IMap::insert()
	using IMap<KeyT, ValT>::insert;
	virtual FORCE_INLINE PairT insert(const PairT & pair) override
	{
		// Insert in tree
		return tree.insertUnique(pair);
	}
};

