#ifndef SGL_GLDRV_H
#define SGL_GLDRV_H

#include "coremin.h"
#include "rhi/rhi.h"

#if PLATFORM_WINDOWS
	#include "windows/gl_windows.h"
#elif PLATFORM_APPLE
	#include "apple/gl_apple.h"
#elif PLATFORM_UNIX
	#include "unix/gl_unix.h"
#else
	#error Unkown platform
#endif

/**
 * @class OpenGLRHI gldrv/gldrv.h
 * @brief OpenGL specific RHI implementation
 */
class OpenGLRHI : public RHI
{
protected:
	/// @brief Platform specific device
	struct OpenGLDevice * platformDevice;

public:
	/// @brief Default-constructor
	OpenGLRHI();
};

/////////////////////////////////////////////////
// OpenGL platform API                         //
/////////////////////////////////////////////////

struct OpenGLContext;
struct OpenGLDevice;

/**
 * @brief Initializes the OpenGL context
 * 
 * @return @c true if cotnext initialized correctly
 */
bool initOpenGL();

/**
 * @brief Create a default platform device
 * 
 * @return pointer to created device
 */
OpenGLDevice * createDefaultOpenGLDevice();

/////////////////////////////////////////////////

/**
 * @brief Initializes a new context with a default state
 */
void initGLContextWithDefaults();

#endif