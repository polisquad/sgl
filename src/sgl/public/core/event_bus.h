#ifndef SGL_EVENT_BUS_H
#define SGL_EVENT_BUS_H

#include "coremin.h"
#include "event.h"

/// Forward declarations
/// @{
class System;
/// @}

/**
 * @class EventBus core/event_bus.h
 * @brief System-wide messaging bus using @ref Event
 */
class EventBus : public Singleton<EventBus>
{
protected:
	/// @brief Message queue
	Queue<Event*> eventQ;

	/// @brief Registered systems
	Array<System*> clients;

public:
	/**
	 * @brief Dispatch a message to other subsytems
	 * 
	 * @param [in] e dispatched event
	 * @{
	 */
	void FORCE_INLINE notify(Event * e);

	/// @details Forces the message to be delivered right away
	void notifyImmediate(Event * e);
	/** @} */

	/// @brief Flush event queue and dispatch events
	void flush();
};

void EventBus::notify(Event * e)
{
	// Note that the event is not dispatched in place
	// All events are queued and dispatched at a certain point
	eventQ.push(e);
}

#endif