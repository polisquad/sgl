#ifndef SGL_ENGINE_H
#define SGL_ENGINE_H

#include "coremin.h"

/// Forward declarations
/// @{
class EventBus;
/// @}

/**
 * @class Engine core/engine.h
 * @brief Application entry point
 */
class Engine : public Singleton<Engine>
{
protected:
	/**
	 * @brief Engine subsytems
	 * @{
	 */
	UniquePtr<EventBus> eventBus;
	/** @} */

public:
	/**
	 * @brief Application entry point
	 * 
	 * Constructs the engine and starts up
	 * all its subsytems
	 */
	Engine();

	/**
	 * @brief Application exit point
	 * 
	 * Destruct engine, shutting down
	 * all its subsystems in reverse order
	 */
	~Engine();

	/// @brief Static getters for subsystems
	/// @{
	static EventBus * getEventBus();
	/// @}
};

#endif