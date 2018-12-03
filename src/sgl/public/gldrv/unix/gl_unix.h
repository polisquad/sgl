#ifndef SGL_GL_UNIX_H
#define SGL_GL_UNIX_H

#include "coremin.h"

/// @brief OpenGL libraries includes
#include <GL/glcorearb.h>
#include <GL/glext.h>

/////////////////////////////////////////////////
// OpenGL functions entry points               //
/////////////////////////////////////////////////

#define GL_ENTRYPOINTS(expansion) \
	expansion(PFNGLCLEARCOLORPROC, glClearColor) \
	expansion(PFNGLCLEARPROC, glClear) \
	expansion(PFNGLCREATEPROGRAMPROC, glCreateProgram) \
	expansion(PFNGLBINDTEXTUREPROC, glBindTexture)

#define GL_DECLARE_ENTRYPOINTS(type, func) extern type func;

/// @brief Namespace for OpenGL functions

namespace GLFuncPointers
{
	GL_ENTRYPOINTS(GL_DECLARE_ENTRYPOINTS)
}
using namespace GLFuncPointers;

#undef GL_DECLARE_ENTRYPOINTS


#endif