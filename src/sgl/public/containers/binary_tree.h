#ifndef SGL_BINARY_TREE_H
#define SGL_BINARY_TREE_H

#include "core_types.h"
#include "hal/platform_memory.h"

/**
 * @class Tree containers/tree.h
 * @brief A templated red&black tree
 */
template<typename T>
class BinaryTree
{
protected:
	/// @brief Node color
	enum
	{
		BLACK	= 0,
		RED		= 1
	};

protected:
	/**
	 * @brief A single node of a tree
	 */
	struct GCC_ALIGN(0x10) Node
	{
		friend BinaryTree;

	private:
		/// @brief Data of the node
		T data;

		/// @brief Parent node (nullptr for root)
		Node * parent;

		/// @brief Child nodes
		/// @{
		Node * left, * right;
		/// @}

		/// @brief Color of the node
		/// 1: red, 0: black
		uint8 color : 1;

	private:
		/// @brief Default-constructor
		FORCE_INLINE Node(const T & _data, Node * _parent = nullptr, Node * _left = nullptr, Node * _right = nullptr) :
			data(_data),
			parent(_parent),
			left(_left),
			right(_right),
			color(RED) {}
		
		/// Quick getters
		/// @{
		FORCE_INLINE bool isRoot() const { return parent == nullptr; }
		FORCE_INLINE bool isLeaf() const { return left == nullptr & right == nullptr; }
		FORCE_INLINE bool isRed() const { return color != BLACK; }
		FORCE_INLINE bool isBlack() const { return color == BLACK; }
		FORCE_INLINE Node * getUncle() const
		{
			if (parent && parent->parent)
			{
				const Node * grand = parent->parent;
				return grand->left == parent ? grand->right : grand->left;
			}

			return nullptr;
		}
		/// @}

		/// Quick setters
		/// @{
		FORCE_INLINE void setLeft(Node * _left)
		{
			left = _left;
			if (left) left->parent = this;
		}
		FORCE_INLINE void setRight(Node * _right)
		{
			right = _right;
			if (right) right->parent = this;
		}
		/// @}
	};

public:
	/**
	 * @class Iterator
	 * @brief BinaryTree iterator
	 */
	template<typename _T = T>
	class BinaryTreeIterator
	{
		friend BinaryTree;

	private:
		/// @brief Iterated tree
		BinaryTree * tree;

		/// @brief Used key
		T key;

		/// @brief Current node
		Node * node;

	public:

		/// @brief Iterator methods
		/// @{
		FORCE_INLINE BinaryTreeIterator<_T> & operator++() { node = tree->find_internal(key, node); return *this; }

		FORCE_INLINE bool operator==(const BinaryTreeIterator<_T> & iter) const { return node == iter.node; } // Checking node should be sufficient
		FORCE_INLINE bool operator!=(const BinaryTreeIterator<_T> & iter) const { return node != iter.node; }

		FORCE_INLINE _T & operator*() const { return node->data; }
		FORCE_INLINE _T * operator->() const { return &node->data; }
		/// @}

	private:
		/// @brief Private default-constructors
		/// @{
		FORCE_INLINE BinaryTreeIterator(BinaryTree * _tree, const T & _key, Node * start = nullptr) :
			tree(_tree),
			key(_key)
		{
			// First search
			node = tree->find_internal(key, start);
		}

		/// @brief Default-iterator, private, just null everything
		FORCE_INLINE BinaryTreeIterator() :
			tree(nullptr),
			node(nullptr) {}
		/// @}
	};
	typedef BinaryTreeIterator<      T> Iterator;
	typedef BinaryTreeIterator<const T> ConstIterator;

protected:
	/// @brief Root node
	Node * root;

	/// @brief Allocator used for nodes allocation
	Malloc * allocator;

	/// @brief Total number of nodes
	uint64 numNodes;

	/// @brief Allocated size
	sizet allocatedSize;

public:
	/// @brief Default constructor
	FORCE_INLINE BinaryTree(Malloc * _allocator = gMalloc) :
		root(nullptr),
		allocator(_allocator),
		numNodes(0),
		allocatedSize(0) {}

	/// @brief Iterators
	/// @{
	FORCE_INLINE Iterator find(const T & key)	{ return Iterator(this, key, root); }
	FORCE_INLINE Iterator end()					{ return Iterator(); }
	
	FORCE_INLINE ConstIterator find(const T & key) const	{ return ConstIterator(this, key, root); }
	FORCE_INLINE ConstIterator end() const					{ return ConstIterator(); }
	/// @}

	/**
	 * @brief Push an element in the tree
	 * 
	 * @param [in] elem element to push
	 * 
	 * @return self
	 * @{
	 */
	FORCE_INLINE BinaryTree<T> & operator+=(const T & elem)
	{
		// Create new node
		Node * node = reinterpret_cast<Node*>(allocator->malloc(sizeof(Node)));
		new (node) Node(elem); // Created red by default

	#if SGL_BUILD_DEBUG
		allocatedSize += sizeof(Node);
		++numNodes;
	#endif

		if (UNLIKELY(root == nullptr))
			// Set root
			setRoot(node);
		else
			// Insert node
			rb_insert(node);
		
		return *this;
	}
	FORCE_INLINE BinaryTree<T> & push(const T & elem) { return operator+=(elem); }
	FORCE_INLINE BinaryTree<T> & add(const T & elem) { return operator+=(elem); }
	/// @}

	/**
	 * @brief Like @ref operator+=() but returns inserted element
	 * 
	 * @return inserted element
	 */
	FORCE_INLINE T & insert(const T & elem)
	{
		Node * node = reinterpret_cast<Node*>(allocator->malloc(sizeof(Node)));
		new (node) Node(elem); // Red by default
	
	#if SGL_BUILD_DEBUG
		allocatedSize += sizeof(Node);
		++numNodes;
	#endif

		if (UNLIKELY(root == nullptr))
			// Set root
			setRoot(node);
		else
			// Insert node
			rb_insert(node);
		
		// Return inserted data
		return node->data;
	}

	/**
	 * @brief Same as @ref insert() but don't insert if already exists
	 * 
	 * @return inserted element
	 */
	FORCE_INLINE T & insertUnique(const T & elem)
	{
		Node * node = reinterpret_cast<Node*>(allocator->malloc(sizeof(Node), alignof(Node)));
		new (node) Node(elem); // Red by default

		if (UNLIKELY(root == nullptr))
		{
			// Set root
			setRoot(node);
			return node->data;
		}
		else
		{
			// Insert node
			Node * inserted = rb_insertUnique(node);
			if (inserted != node)
				allocator->free(node); /// @todo Should we really call destructor?
		#if SGL_BUILD_DEBUG
			else
			{
				allocatedSize += sizeof(Node);
				++numNodes;
			}
		#endif

			return inserted->data;
		}
	}

	/// @brief Like @ref insertUnique() but replaces the value of any already existing element
	FORCE_INLINE T & emplace(const T & elem)
	{
		Node * node = reinterpret_cast<Node*>(allocator->malloc(sizeof(Node), alignof(Node)));
		new (node) Node(elem); // Red by default

		if (UNLIKELY(root == nullptr))
		{
			// Set root
			setRoot(node);
			return node->data;
		}
		else
		{
			// Try to insert unique node
			Node * inserted = rb_insertUnique(node);
			if (inserted != node)
			{
				// Replace data
				inserted->data = node->data;
				allocator->free(node);
			}
		#if SGL_BUILD_DEBUG
			else
			{
				allocatedSize += sizeof(Node);
				++numNodes;
			}
		#endif

			return inserted->data;
		}
	}

#if SGL_BUILD_DEBUG
	void printDebug() const
	{
		printDebug_internal(root, 0);
	}

private:
	void printDebug_internal(Node * it, uint32 depth = 0) const
	{

		// Print this node information
		for (uint32 i = 0; i < depth; ++i) printf("|  ");
		if (it == nullptr)
		{
			printf("B: nil\n");
			return;
		}
		else
		{
			printf("%c: %llu\n", it->isBlack() ? 'B' : 'R', it->data);

			// Recursion
			printDebug_internal(it->right, depth + 1);
			printDebug_internal(it->left, depth + 1);
		}
	}
#endif

protected:
	/**
	 * @brief Set new root
	 * 
	 * @param [in] _root new root
	 * 
	 * @return ref to previous root
	 */
	FORCE_INLINE void setRoot(Node * _root)
	{
		ASSERT(_root != nullptr, "Root cannot be nullptr");

		// Set new root with root properties
		root = _root;
		root->parent = nullptr;
		root->color = BLACK;
	}

	/// @brief Find node given a key and a starting node
	Node * find_internal(const T & key, Node * start = nullptr) const;

	/// R&B insertion and rotation methods
	/// @{

	/**
	 * @brief Insert new node
	 * 
	 * Performs a binary insert and then repairs R&B tree
	 * 
	 * @param [in] _node node to insert
	 * 
	 * @return inserted node
	 */
	FORCE_INLINE Node * rb_insert(Node * _node)
	{
		// Insert binary and repair
		binaryInsert(_node);
		rb_repair(_node);

		return _node;
	}

	/// @brief Insert unique node
	FORCE_INLINE Node * rb_insertUnique(Node * _node)
	{
		Node * exists = binaryInsertUnique(_node);
		if (exists == nullptr)
		{
			// If node was inserted, repair tree
			rb_repair(_node);
			return _node;
		}

		return exists;
	}

	/// @brief Applies red and black properties after binary insert
	/// @note Recursive
	void rb_repair(Node * it);

	/// @brief Rotate left
	FORCE_INLINE void rb_rotateLeft(Node * pivot)
	{
		Node * super = pivot->parent;
		Node * it = pivot->right;
		
		if (super != nullptr)
		{
			if (super->left == pivot)
				super->setLeft(it);
			else
				super->setRight(it);
		}
		else
			// Is root, change root
			setRoot(it);

		// Finish rotation
		pivot->setRight(it->left);
		it->setLeft(pivot);
	}

	/// @brief Rotate right
	/// 
	/// This functions are inlined because called in
	/// limited context (i.e. insert and delete, they
	/// private btw)
	FORCE_INLINE void rb_rotateRight(Node * pivot)
	{
		Node * super = pivot->parent;
		Node * it = pivot->left;
		
		if (super != nullptr)
		{
			if (super->left == pivot)
				super->setLeft(it);
			else
				super->setRight(it);
		}
		else
			// Is root, change root
			setRoot(it);

		// Finish rotation
		pivot->setLeft(it->right);
		it->setRight(pivot);
	}

	/// @}

	/// @brief Insert node in a binary-tree fashion and set to red
	void binaryInsert(Node * _node);

	/// @brief Insert unique node in a binary-tree fashion
	/// @return Existing node or @c nullptr if _node was inserted
	Node * binaryInsertUnique(Node * _node);
};

template<typename T>
typename BinaryTree<T>::Node * BinaryTree<T>::find_internal(const T & key, Node * start) const
{
	Node * it = start ? start : root;
	while (it)
	{
		if (it->data < key)
			it = it->right;
		else if (it->data > key)
			it = it->left;
		else
			// Nice, we don't even need the equality operator
			return it;
	}

	return nullptr;
}

template<typename T>
void BinaryTree<T>::rb_repair(Node * it)
{
	// Case 0: I'm the root!
	if (it == root)
	{
		// And i'm always black!
		it->color = BLACK;
		return;
	}

	Node * parent = it->parent;
	Node * uncle = it->getUncle();
	Node * grand = parent->parent;

	if (parent->isBlack())
		// Case 1: parent is black, nothing to repair
		return;
	else
	{
		if (uncle && uncle->isRed())
		{
			// Switch parent and uncle to black
			// and grandparent to red
			parent->color = uncle->color = BLACK;
			grand->color = RED;

			// Repair grand
			rb_repair(grand);
		}
		else
		{
			// Ugh, rotations!
			if (grand->left == parent & parent->right == it)
			{
				rb_rotateLeft(parent);
				
				// Swap it and parent
				Node * t = it;
				it = parent, parent = t;
			}
			else if (grand->right == parent & parent->left == it)
			{
				rb_rotateRight(parent);

				// Swap it and parent
				Node * t = it;
				it = parent, parent = t;
			}

			// Now parent should be on the outside
			if (parent->left == it)
				rb_rotateRight(grand);
			else
				rb_rotateLeft(grand);

			// Change colors
			parent->color = BLACK;
			grand->color = RED;
		}
	}
}

/**
 * Comparison uses the operator< and operator>
 * define by the type T.
 * 
 * We don't require T to override operator==
 * because we can assert equality by excluding
 * the two former cases.
 * 
 * This comes handy when using key-value pairs,
 * for we can reserve operator== for full equality
 * (i.e. both key and value), as defined in @ref Pair.
 */
template<typename T>
void BinaryTree<T>::binaryInsert(Node * _node)
{
	if (UNLIKELY(root == nullptr))
		// Make root
		setRoot(_node);
	else
	{
		Node * it = root, * prev = nullptr;
		ubyte bLeft = 0;

		// Walk tree
		while (it)
		{
			// Set previous here
			prev = it;

			// Left
			if (_node->data < it->data)
			{
				it = it->left;
				bLeft = 1;
			}
			// Right
			else
			{
				it = it->right;
				bLeft = 0;
			}
		}

		// Insert node as new leaf
		if (bLeft)
			prev->setLeft(_node);
		else
			prev->setRight(_node);
	}
}

template<typename T>
typename BinaryTree<T>::Node * BinaryTree<T>::binaryInsertUnique(Node * _node)
{
	if (UNLIKELY(root == nullptr))
		setRoot(_node);
	else
	{
		Node * it = root, * prev = nullptr;
		ubyte bLeft = 0;

		// Walk tree
		while (it)
		{
			// Set here since we update 'it' conditionally
			prev = it;

			// Left
			if (_node->data < it->data)
			{
				it = it->left;
				bLeft = 1;
			}
			// Right
			else if (_node->data > it->data)
			{
				it = it->right;
				bLeft = 0;
			}
			// Already exists
			else
				return it;
		}

		// Insert node as new leaf
		if (bLeft)
			prev->setLeft(_node);
		else
			prev->setRight(_node);
	}

	return nullptr;
}

#endif