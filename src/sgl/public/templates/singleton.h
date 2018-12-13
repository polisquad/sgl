#ifndef SGL_SINGLETON_H
#define SGL_SINGLETON_H

#include "core_types.h"

#include <cassert>

/**
 * @class Singletong templates/singleton.h
 * @brief Defines a singleton with methods to access it
 */
template<class T>
class Singleton
{
public:
	/// @brief Default-constructor
	FORCE_INLINE Singleton() = default;

	/// @brief Copy-constructor, removed
	Singleton(const Singleton & s) = delete;

	/// @brief Assignment-operator, removed
	Singleton & operator=(const Singleton & s) = delete;

	/**
	 * @brief Get instance reference
	 * 
	 * @return Reference to instance
	 */
	static FORCE_INLINE T & get();

	/**
	 * @brief Get instance pointer
	 * 
	 * @return Pointer to instance
	 */
	static FORCE_INLINE T * getPtr();
};

template<class T>
T & Singleton<T>::get()
{
	// Return global instance
	static T instance;
	return instance;
}

template<class T>
T * Singleton<T>::getPtr()
{
	return &get();
}

#endif