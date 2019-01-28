#ifndef SGL_BINARY_TREE_H
#define SGL_BINARY_TREE_H

#include "core_types.h"
#include "hal/platform_memory.h"
#include "templates/const_ref.h"

/**
 * @struct BinaryNode containers/binary_tree.h
 * 
 * A standalone node of a binary tree.
 * The template type must define at least
 * operator<() and operator>() for node
 * comparison.
 */
template<typename T>
struct GCC_ALIGN(32) BinaryNode
{
public:
	/// Parent node
	BinaryNode * parent;

	/// Left child node
	BinaryNode * left;

	/// Right child node
	BinaryNode * right;

	/// Node data
	T data;

	/// Color of the node
	enum NodeColor : uint8
	{
		BLACK	= 0,
		RED		= 1
	} color;

public:
	/// Default constructor
	FORCE_INLINE BinaryNode(
		typename ConstRef<T>::Type _data,
		NodeColor _color = NodeColor::RED,
		BinaryNode * _parent = nullptr,
		BinaryNode * _left = nullptr,
		BinaryNode * _right = nullptr
	)
		: parent(_parent)
		, left(_left)
		, right(_right)
		, data(_data)
		, color(_color) {}

	/// Access node data
	/// @{
	FORCE_INLINE 	   T & operator*()			{ return data; }
	FORCE_INLINE const T & operator*() const	{ return data; }

	FORCE_INLINE	   T & getData()		{ return data; }
	FORCE_INLINE const T & getData() const	{ return data; }
	/// @}

	/// Returns true if parent matches color
	/// @{
	FORCE_INLINE bool isBlack()	{ return color == NodeColor::BLACK; }
	FORCE_INLINE bool isRed()	{ return color == NodeColor::RED; }
	/// @}

	/**
	 * Finds node that matches data
	 * 
	 * @param [in] search search data
	 * @return node if found, null otherwise
	 * @{
	 */
	/// Search begins from this node
	FORCE_INLINE BinaryNode * find(typename ConstRef<T>::Type search)
	{
		if (search < data)
			return left ? left->find(search) : nullptr;
		else if (search > data)
			return right ? right->find(search) : nullptr;
		else
			return this;
	}

	/// Search begins from right/left node
	FORCE_INLINE BinaryNode * findNext(typename ConstRef<T>::Type search)
	{
		if (search < data)
			return left ? left->find(search) : nullptr;
		else
			return right ? right->find(search) : nullptr;
	}
	/// @}
	
protected:
	/// Set node as left child
	FORCE_INLINE BinaryNode * setLeftChild(BinaryNode * node)
	{
		// @todo handle child replacement

		if (node) node->parent = this;
		return (left = node);
	}

	/// Set node as right child
	FORCE_INLINE BinaryNode * setRightChild(BinaryNode * node)
	{
		// @todo handle child replacement

		if (node) node->parent = this;
		return (right = node);
	}

public:
	/**
	 * Insert a node in the tree structure
	 * 
	 * The tree structure spawning from this node
	 * is traversed until a suitable leaf is found.
	 * The tree structure may be changed to satisfy
	 * the R&B tree properties
	 * 
	 * @param [in] node binary node operand
	 * @return inserted node
	 * @{
	 */
	FORCE_INLINE BinaryNode<T> * insert(BinaryNode * node)
	{
		if (node->data < data)
			return left ? left->insert(node) : setLeftChild(node)->repair();
		else
			return right ? right->insert(node) : setRightChild(node)->repair();
	}
	
	/// If node already exists, don't reinsert it
	FORCE_INLINE BinaryNode<T> * insertUnique(BinaryNode * node)
	{
		if (node->data < data)
			return left ? left->insertUnique(node) : setLeftChild(node)->repair();
		else if (node->data > data)
			return right ? right->insertUnique(node) : setRightChild(node)->repair();
		else
			return this;
	}
	/// @}

	/// Repair tree structure starting from this node
	/// @return self
	BinaryNode * repair()
	{
		// Case 0: I'm (g)root
		if (parent == nullptr)
		{
			color = NodeColor::BLACK;
			return this;
		}

		// Case 1: parent is black
		if (parent->isBlack())
		{
			color = NodeColor::RED;
			return this;
		}
		else
		{
			// Get relatives
			BinaryNode
				* grand = parent->parent,
				* uncle = grand ? (grand->left == parent ? grand->right : grand->left) : nullptr;
			
			// Case 2: uncle is red
			if (uncle && uncle->isRed())
			{
				uncle->color = parent->color = NodeColor::BLACK;
				grand->color = NodeColor::RED;

				// Repair grand
				grand->repair();
			}
			else
			{
				// Case 3: ROTATIONS ARGHHH!!

				// Bring me on the outside
				if (grand->left == parent & parent->right == this)
					parent->rotateLeft();
				else if (grand->right == parent & parent->left == this)
					parent->rotateRight();

				// Perform actual rotation
				if (parent->left == this)
					parent->rotateRight();
				else
					parent->rotateLeft();

				// update color
				color			= NodeColor::BLACK;
				left->color		= NodeColor::RED;
				right->color	= NodeColor::RED;
			}
		}

		return this;
	}

protected:
	/// Rotate left with this node as pivot
	FORCE_INLINE void rotateLeft()
	{
		// Replace me with right child
		if (parent == nullptr)
			; // Do nothing
		else if (parent->left == this)
			parent->setLeftChild(right);
		else
			parent->setRightChild(right);

		// Set right-left as my right child
		BinaryNode * prevRight = right;
		setRightChild(right->left);
		
		// Set me as left child
		prevRight->setLeftChild(this);
	}

	/// Rotate right with this node as pivot
	FORCE_INLINE void rotateRight()
	{
		// Replace me with left child
		if (parent == nullptr)
			; // Do nothing
		else if (parent->left == this)
			parent->setLeftChild(left);
		else
			parent->setRightChild(left);

		// Set left-right as my left child
		BinaryNode * prevLeft = left;
		setLeftChild(left->right);
		
		// Set me as right child
		prevLeft->setRightChild(this);
	}
};

/**
 * @class Tree containers/tree.h
 * @brief A templated red&black tree
 */
template<typename T>
class BinaryTree
{
protected:
	/// @brief Node color
	enum : uint8
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