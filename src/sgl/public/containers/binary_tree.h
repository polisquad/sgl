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
	struct Node
	{
	public:
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

	public:
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

protected:
	/// @brief Root node
	Node * root;

	/// @brief Allocator used for nodes allocation
	Malloc * allocator;

public:
	/// @brief Default constructor
	FORCE_INLINE BinaryTree(Malloc * _allocator = gMalloc) :
		root(nullptr),
		allocator(_allocator) {}

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

		if (UNLIKELY(root == nullptr))
		{
			// Set root
			node->color = 0; // Root is always black
			setRoot(node);
		}
		else
		{
			// Insert node
			rb_insert(node);
		}
		return *this;
	}
	FORCE_INLINE BinaryTree<T> & push(const T & elem) { return operator+=(elem); }
	FORCE_INLINE BinaryTree<T> & add(const T & elem) { return operator+=(elem); }
	/// @}

#if SGL_BUILD_DEBUG
	void printDebug() const
	{
		printDebug_internal(root, 0);
	}

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
	FORCE_INLINE Node * setRoot(Node * _root)
	{
		ASSERT(_root != nullptr, "Root cannot be nullptr");
		Node * oldRoot = root;

		// Set new root with root properties
		root = _root;
		root->parent = nullptr;
		root->color = BLACK;

		// Return old root
		return oldRoot;
	}

	/// R&B insertion and rotation methods
	/// @{

	/// @brief Insert node
	FORCE_INLINE void rb_insert(Node * _node)
	{
		// Insert binary and repair
		binary_insert(_node);
		rb_repair(_node);
	}

	/// @brief Applies red and black properties after binary insert
	/// @note Recursive
	void rb_repair(Node * it);

	/// @brief Rotate left
	FORCE_INLINE void rb_rotateLeft(Node * parent);

	/// @brief Rotate right
	FORCE_INLINE void rb_rotateRight(Node * parent);

	/// @brief Insert node in a binary-tree fashion and set to red
	FORCE_INLINE void binary_insert(Node * _node);

	/// @}
};

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

template<typename T>
void BinaryTree<T>::rb_rotateLeft(Node * pivot)
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

template<typename T>
void BinaryTree<T>::rb_rotateRight(Node * pivot)
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

template<typename T>
void BinaryTree<T>::binary_insert(Node * _node)
{
	if (UNLIKELY(root == nullptr))
	{
		// Make root
		setRoot(_node);
	}
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
			else
			{
				bLeft = 0;
				it = it->right;
			}
		}

		_node->parent = prev;
		if (bLeft)
			prev->setLeft(_node);
		else
			prev->setRight(_node);
	}
}

#endif