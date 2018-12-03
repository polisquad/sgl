#include "engine/engine_loop.h"
#include "gldrv/gldrv.h"

int32 EngineLoop::preInit()
{
	/** @todo Lots of stuff missing here */

	// Init application
	AppMisc::init();

	/// @todo Create something like FSlateApplication
	AppMisc::createApp();

	/// @todo Make this RHIInit()
	initOpenGL();
}