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
	uint64 numClients;

public:
	/// @brief Default-construct(or
	FORCE_INLINE Queue(Malloc * _allocator = gMalloc) :
		allocator(_allocator),
		head(nullptr),
		tail(nullptr),
		numClients(0ULL) {}
	
	/**
	 * @brief Push element to the back of the queue
	 * 
	 * @param [in] elem element to insert
	 */
	void push(const T & elem);

	/**
	 * @brief Pop element from the front
	 * 
	 * @param [out] _data data contained in the head
	 * 
	 * @return true if queue not empty
	 */
	bool pop(T & _data);

	/// @brief Get number of clients in queue
	/// @{
	FORCE_INLINE uint64 getCount() { return numClients; }
	FORCE_INLINE uint64 getNumClients() { return numClients; };
	/// @}
};

template<typename T>
void Queue<T>::push(const T & elem)
{
	Node * node = reinterpret_cast<Node*>(allocator->malloc(sizeof(Node)));
	new (node) Node(elem);

	// Push to back
	if (UNLIKELY(tail == nullptr))
		head = tail = node;
	else
	{
		tail->next = node;
		tail = node;
	}

	// Increment counter
	++numClients;
}

template<typename T>
bool Queue<T>::pop(T & _data)
{
	if (LIKELY(head != nullptr))
	{
		// Pop head
		Node * node = head;
		head = head->next;

		// Decrement counter
		--numClients;
		
		// Dealloc node
		_data = node->data;
		allocator->free(node);
		
		return true;
	}

	return false;
}

#endif