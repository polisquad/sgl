#ifndef SGL_EVENT_H
#define SGL_EVENT_H

#include "coremin.h"

/**
 * @struct Event core/event.h
 * @brief An encapsulated message that can be
 * dispatched by the event bus
 */
class Event
{
protected:
	/// @brief Sender of the message
	class System * sender;

public:
	/// @brief Default-constructor
	Event(System * _sender) : sender(_sender) {}

	/// @brief Return sender
	FORCE_INLINE System * getSender() const { return sender; };
};

#endif