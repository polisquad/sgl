#ifndef SGL_LINKED_LIST_H
#define SGL_LINKED_LIST_H

#include "core_types.h"
#include "hal/platform_memory.h"

/**
 * @class LinkedList containers/linked_list.h
 * @brief Implementation of a traditional linked list container
 */
template<typename T>
class LinkedList
{
public:
	/// @brief A single link of the list
	struct Link
	{
	public:
		/// @brief Data held by the link
		T data;

		/// @brief Next link
		Link * next;

		/// @brief Previous link
		Link * prev;

	public:
		/// @brief Default-constructor, initializes data
		FORCE_INLINE Link(const T & _data) :
			data(_data),
			prev(nullptr),
			next(nullptr) {}

		FORCE_INLINE ~Link()
		{
			// Unlink me
			unlink();
		}

		/// @brief Link next
		FORCE_INLINE void linkNext(Link * _next)
		{
			next = _next;
			if (LIKELY(next != nullptr))
			{
				// Your prev is my prev!
				prev = next->prev;

				// Link with next
				next->prev = this;

				if (prev != nullptr)
				{
					// Link with prev
					prev->next = this;
				}
			}
		}

		/// @brief Link prev
		FORCE_INLINE void linkPrev(Link * _prev)
		{
			prev = _prev;
			if (LIKELY(prev != nullptr))
			{
				// Your next is my next!
				next = prev->next;

				// Link with prev
				prev->next = this;

				if (next != nullptr)
				{
					// Link with next
					next->prev = this;
				}
			}
		}

		/// @brief Unlik this link
		FORCE_INLINE void unlink()
		{
			if (next != nullptr) next->prev = this->prev;
			if (prev != nullptr) prev->next = this->next;
			prev = next = nullptr;
		}
	};
	typedef Link* LinkRef;

	/// @brief LinkedList iterator
	template<typename _T = T>
	class LinkedListIterator
	{
		friend LinkedList;

	private:
		/// @brief Current link
		LinkRef curr;

	public:
		/// @brief Iterator methods
		/// @{
		FORCE_INLINE LinkedListIterator<_T> & operator++() { if (curr != nullptr) curr = curr->next; }
		FORCE_INLINE LinkedListIterator<_T> & operator--() { if (curr != nullptr) curr = curr->prev; }

		FORCE_INLINE bool operator==(const LinkedListIterator & iter) const { return curr == iter.curr; }
		FORCE_INLINE bool operator!=(const LinkedListIterator & iter) const { return curr != iter.curr; }

		FORCE_INLINE _T & operator*() const { return curr->data; }
		FORCE_INLINE _T * operator->() const { return &curr->data; }
		/// @}

	private:
		/// @brief Default-constructor, private usage
		LinkedListIterator(LinkRef _link = nullptr) : curr(_link) {}
	};
	typedef LinkedListIterator<      T> Iterator;
	typedef LinkedListIterator<const T> ConstIterator;

protected:
	/// @biref Allocator used to allocate new links
	Malloc * allocator;

	/// @brief Head of the list
	LinkRef head;

	/// @brief Tail of the list
	LinkRef tail;

	/// @brief List length
	uint64 count;

public:
	/// @brief Default-constructor, empty list
	LinkedList(Malloc * _allocator = gMalloc) :
		allocator(_allocator),
		head(nullptr),
		tail(nullptr),
		count(0) {}

	/// @brief Return list length
	/// @{
	FORCE_INLINE uint64 getCount() const { return count; }
	FORCE_INLINE uint64 getLength() const { return count; }
	/// @}

	/// @brief Random access operator, O(i) time
	T & operator[](uint64 i);

	/// @brief Iterators
	/// @{
	FORCE_INLINE Iterator begin()	{ return Iterator(head); }
	FORCE_INLINE Iterator end()		{ return Iterator(nullptr); }

	FORCE_INLINE ConstIterator begin() const	{ return ConstIterator(head); }
	FORCE_INLINE ConstIterator end() const		{ return ConstIterator(nullptr); }
	/// @}

	/**
	 * @brief Insert a new element at the end of the end of the list
	 * 
	 * @param [in] elem element to insert
	 * @{
	 */
	/// @return self
	FORCE_INLINE LinkedList<T> & operator+=(const T & elem)
	{
		// Create new link
		LinkRef link = reinterpret_cast<Link*>(allocator->malloc(sizeof(Link)));
		new (link) Link(elem);

		// Push to end, replaces tail
		if (UNLIKELY(tail == nullptr))
			tail = head = link;
		else
		{
			link->linkPrev(tail);
			tail = link;
		}

		// Update count
		++count;
	}
	/// @return ref to inserted element
	FORCE_INLINE T & push(const T & elem) { operator+=(elem); return tail->data; }
	/// @}

	/**
	 * @brief Insert a new element at the beginning of the list
	 * 
	 * @param [in] elem element to insert
	 * 
	 * @return ref to inserted elem
	 */
	FORCE_INLINE T & insert(const T & elem)
	{
		// Create new link
		LinkRef link = reinterpret_cast<Link*>(allocator->malloc(sizeof(Link)));
		new (link) Link(elem);

		// Insert at the beginnig, replaces head
		if (UNLIKELY(head == nullptr))
			head = tail = link;
		else
		{
			link->linkNext(head);
			head = link;
		}

		// Update count
		++count;
	}

	/**
	 * @brief Insert a new element at the provided position
	 * 
	 * @param [in]	elem	element to insert
	 * @param [in]	i		element position
	 * 
	 * @return ref to inserted elem
	 */
	T & insert(const T & elem, uint64 i);

	/// @brief Removes last element of the list
	FORCE_INLINE void pop()
	{
		LinkRef removed = tail;
		tail = tail->prev;
		removed->unlink();
	}

	/// @brief Removes first element of the list
	FORCE_INLINE void remove()
	{
		LinkRef removed = head;
		head = head->next;
		removed->unlink();
	}

	/// @brief Removes i-th element from the list
	void remove(uint64 i);
};

template<typename T>
T & LinkedList<T>::operator[](uint64 i)
{
	// Default return value
	static T def;

	if (UNLIKELY(i < 0 | i >= count))
		// Return if OOB
		return def;
	
	if (i < count / 2)
	{
		LinkRef it = head;
		while (i > 0) it = it->next, --i;

		if (LIKELY(it != nullptr)) return it->data;
	}
	else
	{
		i = (count - 1) - i;
		LinkRef it = tail;
		while (i > 0) it = it->prev, --i;

		if (LIKELY(it != nullptr)) return it->data;
	}

	return def;
}

template<typename T>
T & LinkedList<T>::insert(const T & elem, uint64 i)
{
	if (UNLIKELY(i <= 0))
		return insert(elem);
	else if (UNLIKELY(i >= count - 1))
		return push(elem);
	
	// Create new link
	LinkRef link = reinterpret_cast<Link*>(allocator->malloc(sizeof(Link)));
	new (link) Link(elem);

	LinkRef it;
	if (i < count / 2)
	{
		it = head;
		while (i > 0) it = it->next, --i;
	}
	else
	{
		i = (count - 1) - i, it = tail;
		while (i > 0) it = it->prev, --i;
	}

	// Update links
	link->linkNext(it);

	// Update count
	++count;

	return link->data;
}

template<typename T>
void LinkedList<T>::remove(uint64 i)
{
	if (UNLIKELY(i <= 0))
		remove();
	else if (UNLIKELY(i >= count - 1))
		pop();

	LinkRef it;
	if (i < count / 2)
	{
		it = head;
		while (i > 0) it = it->next, --i;
	}
	else
	{
		i = (count - 1) - i, it = tail;
		while (i > 0) it = it->next, --i;
	}

	// Unlink
	it->unlink();
}

/// @brief Provides access to the unscoped link type
template<typename T> using Link = typename LinkedList<T>::Link;
template<typename T> using LinkRef = typename LinkedList<T>::LinkRef;

#endif