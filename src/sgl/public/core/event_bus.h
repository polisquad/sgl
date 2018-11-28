#ifndef SGL_EVENT_BUS_H
#define SGL_EVENT_BUS_H

#include "event.h"

/// Forward declarations
/// @{
class System;
/// @}

#define EVENT_BUS_MAX_QUEUE_LENGTH 64	// Maximum number of pending events

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
	 * @brief Register a new client (system)
	 * 
	 * @param [in] client client to be registered
	 */
	FORCE_INLINE void registerClient(System * client);

	/**
	 * @brief Dispatch a message to other subsytems
	 * 
	 * @param [in] e dispatched event
	 * @{
	 */
	FORCE_INLINE void notify(Event * e);

	/// @details Forces the message to be delivered right away
	void notifyImmediate(Event * e);
	/// @}

	/// @brief Flush event queue and dispatch events
	void flush();
};

void EventBus::registerClient(System * client)
{
	// Add to client array
	clients.push(client);
}

void EventBus::notify(Event * e)
{
	// Note that the event is not dispatched in place
	// All events are queued and dispatched at a certain point
	eventQ.push(e);
	if (UNLIKELY(eventQ.getNumClients() >= EVENT_BUS_MAX_QUEUE_LENGTH)) flush();
}

#endif