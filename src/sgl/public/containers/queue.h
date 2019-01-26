#ifndef SGL_QUEUE_H
#define SGL_QUEUE_H

#include "core_types.h"
#include "hal/platform_memory.h"
#include "hal/malloc_ansi.h"
#include "templates/const_ref.h"
#include "templates/is_trivially_copyable.h"
#include "templates/enable_if.h"
#include "templates/same_type.h"

/**
 * @class Queue containers/queue.h
 * 
 * A Queue class implemented as a
 * singly-linked list
 */
template <typename T, typename AllocT = MallocAnsi>
class GCC_ALIGN(32) Queue
{
	// Sometimes C++ doesn't really make sense ...
	template<typename U, typename AllocU>
	friend class Queue;

protected:
	/// A client node
	struct Client
	{
		/// Data carried by the client
		T data;

		/// Next client in queue
		Client * next;

		/// Default constructor
		FORCE_INLINE Client(typename ConstRef<T>::Type & _data, Client * _next = nullptr) :
			data(_data),
			next(nullptr) {}
	};

	using ClientRef = Client*;

protected:
	/// Allocator in use
	AllocT * allocator;
	bool bHasOwnAllocator;

	/// First client
	ClientRef first;

	/// Last client
	ClientRef last;

	/// Num of clients
	uint64 numClients;

public:
	/// Default constructor
	FORCE_INLINE Queue(AllocT * _allocator = reinterpret_cast<AllocT*>(gMalloc)) :
		allocator(_allocator),
		bHasOwnAllocator(!_allocator),
		first(nullptr),
		last(nullptr),
		numClients(0)
	{
		// Create a new allocator for the queue
		if (bHasOwnAllocator)
			allocator = new AllocT;
	}

protected:
	/// Construct client node
	FORCE_INLINE ClientRef createClient(typename ConstRef<T>::Type data)
	{
		return new (reinterpret_cast<ClientRef>(allocator->malloc(sizeof(Client)))) Client(data);
	}

public:
	/// Copy constructor
	FORCE_INLINE Queue(const Queue<T, AllocT> & other) : Queue(nullptr)
	{
		if (other.first)
		{
			// Set first
			first = last = createClient(other.first->data);

			// Very costly, we have to reinsert all the clients
			ClientRef it = other.first->next;
			while (it)
			{
				last = last->next = createClient(it->data);
				it = it->next;
			}
		}

		// Set num clients
		numClients = other.numClients;
	}

	/// Copy constructor with different allocator type
	template<typename AllocU>
	FORCE_INLINE Queue(const Queue<T, AllocU> & other) : Queue(nullptr)
	{
		if (other.first)
		{
			// Set first
			first = last = createClient(other.first->data);

			// Very costly, we have to reinsert all the clients
			typename Queue<T, AllocU>::ClientRef it = other.first->next;
			while (it)
			{
				last = last->next = createClient(it->data);
				it = it->next;
			}
		}

		// Set num clients
		numClients = other.numClients;
	}

	/// Move constructor
	FORCE_INLINE Queue(Queue<T, AllocT> && other) :
		allocator(other.allocator),
		bHasOwnAllocator(other.bHasOwnAllocator),
		first(other.first),
		last(other.last),
		numClients(other.numClients)
	{
		other.bHasOwnAllocator = false;
		other.first = other.last = nullptr;
	}

	/// Copy assignment
	FORCE_INLINE Queue<T, AllocT> & operator=(const Queue<T, AllocT> & other)
	{
		// @todo empty self first

		if (other.first)
		{
			// Set first
			first = last = createClient(other.first->data);

			// Very costly, we have to reinsert all the clients
			ClientRef it = other.first->next;
			while (it)
			{
				last = last->next = createClient(it->data);
				it = it->next;
			}
		}

		// Set num clients
		numClients = other.numClients;
	}

	/// Copy assignment with different allocator type
	template<typename AllocU>
	FORCE_INLINE Queue<T, AllocT> & operator=(const Queue<T, AllocU> & other)
	{
		// @todo empty self first

		if (other.first)
		{
			// Set first
			first = last = createClient(other.first->data);

			// Very costly, we have to reinsert all the clients
			typename Queue<T, AllocU>::ClientRef it = other.first->next;
			while (it)
			{
				last = last->next = createClient(it->data);
				it = it->next;
			}
		}

		// Set num clients
		numClients = other.numClients;
	}

	/// Move assignment
	FORCE_INLINE Queue<T, AllocT> & operator=(Queue<T, AllocT> && other)
	{
		// @todo empty self first

		allocator			= other.allocator;
		bHasOwnAllocator	= other.bHasOwnAllocator;
		first				= other.first;
		last				= other.last;
		numClients			= other.numClients;

		other.bHasOwnAllocator = false;
		other.first = other.last = nullptr;
	}
	
	/// Returns number of clients in queue
	FORCE_INLINE uint64 getLength() const { return numClients; }

	/**
	 * Insert a new client in queue
	 * 
	 * @param [in] data client data
	 * @return ref to inserted data
	 */
	FORCE_INLINE T & push(typename ConstRef<T>::Type data)
	{
		// Insert at the end of the queue
		if (last == nullptr)
			first = last = createClient(data);
		else
			last = last->next = createClient(data);
		
		++numClients;

		return last->data;
	}

	/**
	 * Pop first client in queue
	 * 
	 * @param [out] data value carried by client
	 * @return true if queue was not empty
	 * @{
	 */
	FORCE_INLINE bool pop()
	{
		if (first)
		{
			// Dealloc node
			ClientRef next = first->next;
			allocator->free(first);

			// Link next
			if (next)
				first = next;
			else
				first = last = nullptr;

			return true;
		}

		return false;
	}
	FORCE_INLINE bool pop(T & data)
	{
		if (first)
		{
			// Get data out
			moveOrCopy(data, first->data);
			return true;
		}

		return false;
	}
	/// @}
};

#endif