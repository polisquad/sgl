#include "core/engine.h"

#include "core/event_bus.h"

Engine::Engine()
{
	// Init subsystems
	eventBus.reset(new EventBus());
}

/////////////////////////////////////////////////
// Getters for subsystems                      //
/////////////////////////////////////////////////

EventBus * Engine::getEventBus()
{
	return &EventBus::get();
}

Engine::~Engine() = default;