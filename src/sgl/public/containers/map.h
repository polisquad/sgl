#pragma once

#include "core_types.h"
#include "pair.h"
#include "binary_tree.h"
#include "hal/malloc_binned.h"

/**
 * @class Map containers/tree_map.h
 * 
 * A map built on top of a binary tree
 * 
 * A tree map has an optimal space-efficiency. Keys
 * are sorted and all basic operations (insertion,
 * deletion, search) are O(log(n))
 */
template<typename KeyT, typename ValT, typename AllocT = MallocBinned>
class Map
{
public:
	/// Pair type
	using PairT = Pair<KeyT, ValT>;

	/// Tree type
	using TreeT = BinaryTree<PairT, AllocT>;

	/// Node type
	using NodeT		= typename TreeT::Node;
	using NodeRef	= NodeT*;

	/// Iterators
	using Iterator		= typename TreeT::Iterator;
	using ConstIterator	= typename TreeT::ConstIterator;

protected:
	/// Binary tree used for node storage
	TreeT tree;

public:
	/// Default constructor
	FORCE_INLINE Map(AllocT * allocator = reinterpret_cast<AllocT*>(gMalloc)) :
		tree(allocator) {}

	/**
	 * Find value using key
	 * 
	 * @param [in] key search key
	 * @return map iterator
	 * @{
	 */
	FORCE_INLINE Iterator find(typename ConstRef<KeyT>::Type key)
	{
		return tree.find(PairT(key));
	}
	FORCE_INLINE ConstIterator find(typename ConstRef<KeyT>::Type key) const
	{
		return tree.find(PairT(key));
	}
	/// @}

	/// Returns end iterator
	/// @{
	FORCE_INLINE Iterator		end()		{ return tree.end(); }
	FORCE_INLINE ConstIterator	end() const	{ return tree.end(); }
	/// @}

	/**
	 * Returns ref to value associated with key
	 * 
	 * If key doesn't exist, create a new one
	 * 
	 * @param [in] key search key
	 * @return ref to associated value
	 */
	FORCE_INLINE ValT & operator[](typename ConstRef<KeyT>::Type key)
	{
		PairT pair	= PairT(key);
		Iterator it	= tree.find(pair);

		if (it != tree.end())
			return it->second;
		else
			// Create if not found
			return tree.insert(pair).second;
	}

	/**
	 * Insert a new unique pair
	 * 
	 * @param [in] pair <key, value> pair to insert
	 * @param [in] key key value
	 * @param [in] val pair value
	 * @return inserted pair or pair that prevented insertion
	 * @{
	 */
	FORCE_INLINE PairT & insert(const PairT & pair)
	{
		return tree.insertUnique(pair);
	}
	FORCE_INLINE PairT & insert(typename ConstRef<KeyT>::Type key, typename ConstRef<ValT>::Type val)
	{
		return insert(PairT(key, val));
	}
	/// @}

public:

};

