#pragma once

#include "core_types.h"
#include "hal/platform_memory.h"
#include "hal/malloc_ansi.h"
#include "templates/const_ref.h"
#include "templates/functional.h"

/**
 * @struct BinaryNode<T>containers/binary_tree.h
 * 
 * A standalone node of a binary tree.
 * The template type must define at least
 * operator<() and operator>() for node
 * comparison.
 */
template<typename T, typename CompareT = Compare>
struct GCC_ALIGN(32) BinaryNode
{
	template<typename, typename, typename> friend class BinaryTree;
	
public:
	/// Parent node
	BinaryNode* parent;

	/// Left child node
	BinaryNode* left;

	/// Right child node
	BinaryNode* right;

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
		BinaryNode* _parent = nullptr,
		BinaryNode* _left = nullptr,
		BinaryNode* _right = nullptr
	)
		: parent(_parent)
		, left(_left)
		, right(_right)
		, data(_data)
		, color(_color) {}

	/// Returns true if parent matches color
	/// @{
	FORCE_INLINE bool isBlack()	{ return color == NodeColor::BLACK; }
	FORCE_INLINE bool isRed()	{ return color == NodeColor::RED; }
	/// @}

	/// Get root of this tree
	FORCE_INLINE BinaryNode * getRoot()
	{
		return parent ? parent->getRoot() : this;
	}

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
		// Compare search key and node key
		const int32 compare = CompareT().template operator()<decltype(search), decltype(data)>(search, data);

		if (compare < 0)
			return left ? left->find(search) : nullptr;
		else if (compare > 0)
			return right ? right->find(search) : nullptr;
		else
			return this;
	}

	/// Search begins from right/left node
	FORCE_INLINE BinaryNode * findNext(typename ConstRef<T>::Type search)
	{
		if (CompareT().template operator()<decltype(search), decltype(data)>(search, data) < 0)
			return left ? left->find(search) : nullptr;
		else
			return right ? right->find(search) : nullptr;
	}
	/// @}
	
protected:
	/// Set node as left child
	FORCE_INLINE BinaryNode * setLeftChild(BinaryNode* node)
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
	 * Insert a node in this subtree
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
	FORCE_INLINE BinaryNode * insert(BinaryNode * node)
	{
		if (CompareT().template operator()<decltype(node->data), decltype(data)>(node->data, data) < 0)
			return left ? left->insert(node) : setLeftChild(node)->repair();
		else
			return right ? right->insert(node) : setRightChild(node)->repair();
	}
	
	/// If node already exists, don't reinsert it
	FORCE_INLINE BinaryNode * insertUnique(BinaryNode * node)
	{
		// Compare inserting node data against node data
		int32 compare = CompareT().template operator()<decltype(node->data), decltype(data)>(node->data, data);
		
		if (compare < 0)
			return left ? left->insertUnique(node) : setLeftChild(node)->repair();
		else if (compare > 0)
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
			
			// Case 2: uncle exists and is red
			if (uncle && uncle->isRed())
			{
				uncle->color = parent->color = NodeColor::BLACK;
				grand->color = NodeColor::RED;

				// Repair grand
				grand->repair();
			}
			else
			{
				// Case 3: uncle is black or doesn't exist

				// Bring me on the outside
				if (grand->left == parent)
				{
					if (parent->right == this)
					{
						// Note, I'm being pushed as root of the subtree
						parent->rotateLeft(),
						parent->rotateRight();

						color			= NodeColor::BLACK;
						right->color	= NodeColor::RED;
					}
					else
					{
						grand->rotateRight();

						parent->color	= NodeColor::BLACK;
						grand->color	= NodeColor::RED;
					}
				}
				else
				{
					if (parent->left == this)
					{
						// Note, I'm being pushed as root of the subtree
						parent->rotateRight(),
						parent->rotateLeft();

						color		= NodeColor::BLACK;
						left->color	= NodeColor::RED;
					}
					else
					{
						grand->rotateLeft();

						parent->color	= NodeColor::BLACK;
						grand->color	= NodeColor::RED;
					}
				}
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
			right->parent = nullptr;
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
			left->parent = nullptr;
		else if (parent->left == this)
			parent->setLeftChild(left);
		else
			parent->setRightChild(left);

		// Set left-right as my left child
		BinaryNode* prevLeft = left;
		setLeftChild(left->right);
		
		// Set me as right child
		prevLeft->setRightChild(this);
	}
};

/// Node reference type
template<typename T, typename CompareT = Compare>
using BinaryNodeRef = BinaryNode<T, CompareT>*;

/**
 * @class Tree containers/tree.h
 * 
 * A templated red-black tree.
 * @see BinaryNode
 */
template<typename T, typename CompareT = Compare, typename AllocT = MallocAnsi>
class GCC_ALIGN(32) BinaryTree
{
	template<typename, typename, typename>				friend class BinaryTree;
	template<typename, typename, typename, typename>	friend class Map;

public:
	/// Node type
	using Node		= BinaryNode<T, CompareT>;
	using NodeRef	= BinaryNodeRef<T, CompareT>;

	/// Node iterator
	template<typename U>
	struct GCC_ALIGN(32) NodeIterator
	{
		template<typename, typename, typename> friend class BinaryTree;

	private:
		/// Current node
		BinaryNodeRef<U> node;

		/// Search value
		U search;

	private:
		/// Default constructor, private
		NodeIterator() :
			node(nullptr),
			search() {}
		
		/// Initialize iterator with search value and node
		NodeIterator(typename ConstRef<U>::Type _search, BinaryNodeRef<U> _node) :
			node(_node),
			search(_search)
		{
			// Find first node
			if (node) node = node->find(search);
		}

	public:
		/// Advances iterator
		FORCE_INLINE NodeIterator<U> & operator++()
		{
			if (node = node->right)
				node = node->find(search);
			
			return *this;
		}

		/// Backtrack iterator
		FORCE_INLINE NodeIterator<U> & operator--()
		{
			while (node = node->parent)
				if (CompareT().template operator()<decltype(node->data), decltype(search)>(node->data, search) == 0) return *this;
			
			return *this;
		}

		/// Iterator comparison
		/// @{
		FORCE_INLINE bool operator==(const NodeIterator<U> & other) const { return node == other.node; }
		FORCE_INLINE bool operator!=(const NodeIterator<U> & other) const { return node != other.node; }
		/// @}

		/// Access Node data
		/// @{
		FORCE_INLINE U & operator* () const { return node->data; };
		FORCE_INLINE U * operator->() const { return &(node->data); };
		/// @}
	};

	/// Define iterator types
	using Iterator		= NodeIterator<T>;
	using ConstIterator	= NodeIterator<const T>;

protected:
	/// Allocator used to allocate new nodes
	AllocT * allocator;
	bool bHasOwnAllocator;

	/// Root node
	NodeRef root;

	/// Num of nodes
	uint64 numNodes;

public:
	/// Default constructor
	FORCE_INLINE BinaryTree(AllocT * _allocator = reinterpret_cast<AllocT*>(gMalloc)) :
		allocator(_allocator),
		bHasOwnAllocator(_allocator == nullptr),
		root(nullptr),
		numNodes(0)
	{
		// Create own allocator
		if (bHasOwnAllocator)
			allocator = new AllocT;
		/* else
			printf("%p\n", allocator); */
	}

protected:
	/// Create a new node using the class allocator
	FORCE_INLINE NodeRef createNode(typename ConstRef<T>::Type data)
	{
		return new (reinterpret_cast<NodeRef>(allocator->malloc(sizeof(Node)))) Node(data);
	}

	/// Recursively replicate structure of another tree
	template<typename U>
	void replicateStructure(NodeRef replica, BinaryNodeRef<U> original)
	{
		// Create left
		if (original->left)
		{
			if (replica->left)
				// Don't recreate node
				moveOrCopy(replica->left->data, original->left->data);
			else
				replica->setLeftChild(createNode(original->left->data));
			
			replicateStructure(replica->left, original->left);
		}
		else if (replica->left)
			// Remove unused nodes
			empty(replica->left);

		// Create right
		if (original->right)
		{
			if (replica->right)
				// Don't recreate node
				moveOrCopy(replica->right->data, original->right->data);
			else
				replica->setRightChild(createNode(original->right->data));
			
			replicateStructure(replica->right, original->right);
		}
		else if (replica->right)
			// Remove unused nodes
			empty(replica->right);
	}

public:
	/// Copy constructor
	FORCE_INLINE BinaryTree(const BinaryTree<T, AllocT> & other) : BinaryTree(nullptr)
	{
		if (other.root)
			// Copy the tree structure as-is
			replicateStructure(root = createNode(other.root->data), other.root);
		
		numNodes = other.numNodes;
	}

	/// Copy constructor (different allocator)
	template<typename AllocU>
	FORCE_INLINE BinaryTree(const BinaryTree<T, AllocU> & other) : BinaryTree(nullptr)
	{
		if (other.root)
			// Copy the tree structure as-is
			replicateStructure(root = createNode(other.root->data), other.root);
		
		numNodes = other.numNodes;
	}

	/// Move constructor
	FORCE_INLINE BinaryTree(BinaryTree<T, AllocT> && other) :
		allocator(other.allocator),
		bHasOwnAllocator(other.bHasOwnAllocator),
		root(other.root),
		numNodes(other.numNodes)
	{
		other.bHasOwnAllocator = false;
		other.root = nullptr;
	}

	/// Copy assignment
	FORCE_INLINE BinaryTree<T, AllocT> & operator=(const BinaryTree<T, AllocT> & other)
	{
		// Copy the tree structure as-is
		if (other.root)
		{
			if (root)
			{
				moveOrCopy(root->data, other.root->data);
				replicateStructure(root, other.root);
			}
			else
				replicateStructure(root = createNode(other.root->data), other.root);
		}
		
		numNodes = other.numNodes;
	}

	/// Copy assignment (different allocator)
	template<typename AllocU>
	FORCE_INLINE BinaryTree<T, AllocT> & operator=(const BinaryTree<T, AllocU> & other)
	{
		// Copy the tree structure as-is
		if (other.root)
		{
			if (root)
			{
				moveOrCopy(root->data, other.root->data);
				replicateStructure(root, other.root);
			}
			else
				replicateStructure(root = createNode(other.root->data), other.root);
		}
		
		numNodes = other.numNodes;
	}

	/// Move assignment
	FORCE_INLINE BinaryTree<T, AllocT> & operator=(BinaryTree<T, AllocT> && other)
	{
		allocator			= other.allocator;
		bHasOwnAllocator	= other.bHasOwnAllocator;
		root				= other.root;
		numNodes			= other.numNodes;

		other.bHasOwnAllocator	= false;
		other.root				= nullptr;
	}

	/// Destructor
	FORCE_INLINE ~BinaryTree()
	{
		// Empty tree
		empty(root);

		// Delete own allocator
		if (bHasOwnAllocator)
			delete allocator;
	}

public:
	/// Get number of nodes
	FORCE_INLINE uint64 getSize() const { return numNodes; }

	/**
	 * Find node matching search
	 * 
	 * @param [in] search search operand
	 * @return node iterator
	 */
	FORCE_INLINE Iterator find(typename ConstRef<T>::Type search) const
	{
		return Iterator(search, root);
	}

	/// Returns an iterator that matched end of search
	FORCE_INLINE Iterator end() const { return Iterator(); }

	/**
	 * Insert a new node with the provided data
	 * 
	 * @param [in] data data to insert in node
	 */
	T & insert(typename ConstRef<T>::Type data)
	{
		if (LIKELY(root != nullptr))
		{
			NodeRef node = root->insert(createNode(data));
			++numNodes;
			
			root = root->getRoot();
			
			return node->data;
		}
		else
		{
			root = createNode(data);
			root->color = Node::NodeColor::BLACK;

			return root->data;
		}
	}

	/**
	 * Insert a new node with the provided data
	 * 
	 * @param [in] data data to insert in node
	 */
	T & insertUnique(typename ConstRef<T>::Type data)
	{
		if (LIKELY(root != nullptr))
		{
			NodeRef node		= createNode(data);
			NodeRef actualNode	= root->insertUnique(node);

			if (node == actualNode)
			{
				++numNodes;

				root = root->getRoot();
			}
			else
				allocator->free(node);

			return actualNode->data;
		}
		else
		{
			root = createNode(data);
			root->color = Node::NodeColor::BLACK;

			numNodes = 1;
			return root->data;
		}
	}

	/// Recursively removes all nodes of the tree
	/// @{
	FORCE_INLINE void empty() { return empty(root); }

protected:
	void empty(NodeRef node)
	{
		if (node)
		{
			NodeRef
				left	= node->left,
				right	= node->right;
			
			// Dealloc node
			allocator->free(node);

			// Depth first
			empty(left), empty(right);
		}
	}
	///@}
};