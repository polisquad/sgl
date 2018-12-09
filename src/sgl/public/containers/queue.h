#ifndef SGL_QUEUE_H
#define SGL_QUEUE_H

#include "core_types.h"
#include "hal/platform_memory.h"

/**
 * @class Queue containers/queue.h
 */
template <typename T>
class Queue
{
protected:
	/// Linked list node
	struct Node
	{
		/// Link data
		T data;

		/// Next and prev node
		Node * next;

		/// Node constructor
		FORCE_INLINE Node(const T & elem, Node * _next = nullptr, Node * _prev = nullptr) :
			data(elem), next(_next) {}
	};

protected:
	/// @brief Used allocator
	Malloc * allocator;

	/// @brief Front of the queue
	Node * head;

	/// @brief Back of the queue
	Node * tail;

	/// @brief Number of clients in queue
	uint64 counter;

public:
	/// @brief Default-construct(or
	FORCE_INLINE Queue(Malloc * _allocator = gMalloc);
	
	/**
	 * @brief Push element to the back of the queue
	 * 
	 * @param [in] elem element to insert
	 */
	void push(const T & elem);

	/**
	 * @brief Pop element from the front
	 * 
	 * @return head of the queue,
	 * or @c nullptr
	 */
	T pop();

	/// @brief Get number of clients in queue
	FORCE_INLINE uint64 getNumClients() { return counter; };
};

template<typename T>
Queue<T>::Queue(Malloc * _allocator) :
	allocator(_allocator),
	head(nullptr),
	tail(nullptr),
	counter(0ULL) {}

template<typename T>
void Queue<T>::push(const T & elem)
{
	Node * node = reinterpret_cast<Node*>(allocator->malloc(sizeof(Node)));
	new (node) Node(elem);

	// Push to back
	if (UNLIKELY(tail == nullptr))
	{
		head = tail = node;
	}
	else
	{
		tail->next = node;
		tail = node;
	}

	// Increment counter
	++counter;
}

template<typename T>
T Queue<T>::pop()
{
	if (LIKELY(head != nullptr))
	{
		// Pop head
		Node * node = head;
		head = head->next;

		// Decrement counter
		--counter;

		// Dealloc node
		const T data = node->data;
		allocator->free(node);
		
		return data;
	}
}

#endif