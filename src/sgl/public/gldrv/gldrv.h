#ifndef SGL_GLDRV_H
#define SGL_GLDRV_H

#include "coremin.h"

#if PLATFORM_WINDOWS
	#include "windows/gl_windows.h"
#elif PLATFORM_APPLE
	#include "apple/gl_apple.h"
#elif PLATFORM_UNIX
	#include "unix/gl_unix.h"
#else
	#error Unkown platform
#endif

/////////////////////////////////////////////////
// OpenGL platform API                         //
/////////////////////////////////////////////////

/**
 * @brief Initializes the OpenGL context
 * 
 * @return @c true if cotnext initialized correctly
 */
bool initOpenGL();

#endif