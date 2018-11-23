#include "core/event_bus.h"
#include "core/event.h"
#include "core/system.h"

void EventBus::notifyImmediate(Event * e)
{
	// Process event
	for (auto sys : clients) sys->handle(e);
}

void EventBus::flush()
{
	/** @todo Implement own queue */
	Event * e = eventQ.pop();
	while (e)
	{		
		// Process event
		for (auto sys : clients) sys->handle(e);

		// Next event
		e = eventQ.pop();
	}
}