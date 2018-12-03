#ifndef SGL_ENGINE_LOOP_H
#define SGL_ENGINE_LOOP_H

#include "core_types.h"
#include "templates/singleton.h"
#include "app/app_misc.h"

/**
 * @class EngineLoop engine/engine_loop.h
 * @brief This is the actual application interface
 * 
 * The class exposes methods that covers the application
 * lifetime cycle:
 * - EngineLoop::preInit() ~ Low-level systems initialization (app, renderer, etc.)
 * - EngineLoop::init() ~ High-level systems (game, input, etc.)
 * - EngineLoop::tick() ~ Application master loop
 * - EngineLoop::exit() ~ Application exit point
 */
class EngineLoop
{
public:
	/// @brief Default-constructor
	EngineLoop() = default;

	/// @brief Destructor
	~EngineLoop() VOID_FUNC;

	/**
	 * @brief Initialize low-level systems
	 * 
	 * @return error level
	 */
	int32 preInit();

	/**
	 * @brief Initialize the main loop
	 * 
	 * @return error level
	 */
	int32 init();

	
	/// @brief Application main loop
	void tick();
	
	/// @brief Shutdown engine
	void exit();
};

#endif