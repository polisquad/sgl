#include "gldrv/gldrv.h"

OpenGLRHI::OpenGLRHI() :
	platformDevice(nullptr)
{
	// Init OpenGL
	initOpenGL();

	// Create a platform device
	platformDevice = createDefaultGLDevice();

	/// @todo Then I should init RHI globals
	/// ... but I don't have RHI globals LOL
}

void initGLContextWithDefaults()
{
	/// @todo I don't think I need to set anything here
}