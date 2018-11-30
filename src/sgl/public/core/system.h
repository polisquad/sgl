#ifndef SGL_SYSTEM_H
#define SGL_SYSTEM_H

#include "coremin.h"

/**
 * @class System core/system.h
 */
class System
{
protected:
	/// @brief System descriptive name
	/** @todo I intend to make this some sort of string */
	uint64 name;

public:
	/**
	 * @brief Handle incoming event
	 * 
	 * @param [in] e event
	 */
	virtual void handle(class Event * e) = 0;

	/// @brief Get system descriptive name
	FORCE_INLINE uint64 getName() const { return name; }
};

#endif