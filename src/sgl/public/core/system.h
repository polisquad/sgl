#ifndef SGL_SYSTEM_H
#define SGL_SYSTEM_H

/// Forward declarations
/// @{
class Event;
/// @}

/**
 * @class System core/system.h
 */
class System
{
public:
	/**
	 * @brief Handle incoming event
	 * 
	 * @param [in] e event
	 */
	virtual void handle(Event * e) = 0;
};

#endif