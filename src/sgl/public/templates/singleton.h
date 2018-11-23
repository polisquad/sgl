#ifndef SGL_SINGLETON_H
#define SGL_SINGLETON_H

#include "core_types.h"

/**
 * @class Singletong templates/singleton.h
 * @brief Defines a singleton with methods to access it
 */
template<class T>
class Singleton
{
protected:
	/**
	 * @brief Global singleton instance
	 */
	static T * instance;

public:
	/**
	 * @brief Default-constructor
	 */
	Singleton();

	/**
	 * @brief Deleted copy constructor
	 */
	Singleton(const Singleton & s) = delete;

	/**
	 * @brief Deleted assignment operator
	 */
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

template<typename T>
T * Singleton<T>::instance = nullptr;

template<class T>
Singleton<T>::Singleton()
{
	// Assert single instance
	assert(!instance);
	instance = static_cast<T*>(this);
}

template<class T>
T & Singleton<T>::get()
{
	assert(instance);
	return *instance;
}

template<class T>
T * Singleton<T>::getPtr()
{
	return instance;
}

#endif