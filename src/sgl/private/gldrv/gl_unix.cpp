#include "gldrv/unix/gl_unix.h"
#include "app/unix/unix_app_misc.h"

#include "SDL.h"

#define GL_DECLARE_ENTRYPOINTS(type, func) type func = 0;
namespace GLFuncPointers
{
	GL_ENTRYPOINTS(GL_DECLARE_ENTRYPOINTS)
}
#undef GL_DECLARE_ENTRYPOINTS

// Only visible in this unit
namespace
{
	/// @brief Make context active
	FORCE_INLINE void contextMakeCurrent(SDL_Window * window, SDL_GLContext context)
	{
		auto error = SDL_GL_MakeCurrent(window, context);
		if (error)
		{
			/// @todo Handle error
		}
	}

	/// @brief Returns currently active context
	FORCE_INLINE SDL_GLContext getCurrentContext()
	{
		return SDL_GL_GetCurrentContext();
	}

	/**
	 * @brief Platform specific OpenGL context
	 */
	struct OpenGLContext
	{
		/// @brief Bound window
		SDL_Window * window;

		/// @brief Bound context
		SDL_GLContext context;

		/// @brief Viewport framebuffer ID
		GLuint viewportFramebuffer;

		/// @brief Requirement of OpenGL 3.2 core profile
		GLuint vao;
	};

	/// @brief Creates a dummy window, used for context initialization
	FORCE_INLINE void createDummyGLWindow(OpenGLContext * outContext)
	{
		// Create a new dummy window
		SDL_Window * dummyWindow = SDL_CreateWindow("dump", 0, 0, 0, 0, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
		if (!dummyWindow)
		{
			/// @todo Handle errors
			return;
		}

		// Set context window
		outContext->window	= dummyWindow;
	}

	void createOpenGLContext(OpenGLContext * outContext)
	{
		// Don't lose current context
		SDL_Window * currW	= SDL_GL_GetCurrentWindow();
		SDL_GLContext currC	= SDL_GL_GetCurrentContext();

		outContext->viewportFramebuffer	= 0;
		outContext->context				= SDL_GL_CreateContext(outContext->window);
		if (!outContext->context)
		{
			/// @todo Handle exception
			return;
		}

		contextMakeCurrent(currW, currC);
	}
}

/// @brief Initializes OpenGL using SDL
bool initOpenGL()
{
	static bool bInitialized = false;
	static bool bOpenGLSupported = false;

	// Init SDL
	if (!UnixAppMisc::initSDL())
	{
		/// @todo Handle error
		return false;
	}

	if (!bInitialized) // Init only once
	{
		// Load default OpenGL library
		if (SDL_GL_LoadLibrary(nullptr) != 0)
		{
			/// @todo Handle error
			return false;
		}

		// Default versions
		int majorVersion = 4;
		int minorVersion = 5;

		if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, majorVersion) != 0);
		if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minorVersion) != 0);
		if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != 0);

		// Create a dummy context to load OpenGL
		OpenGLContext dummyContext;
		createDummyGLWindow(&dummyContext);
		createOpenGLContext(&dummyContext);

		if (dummyContext.context != nullptr)
		{
			bOpenGLSupported = true;
			contextMakeCurrent(dummyContext.window, dummyContext.context);
		}

		if (bOpenGLSupported)
		{
		#define GL_GET_PROC_ADDRESS(type, func) GLFuncPointers::func = reinterpret_cast<type>(SDL_GL_GetProcAddress(#func));
			// Load OpenGL functions
			GL_ENTRYPOINTS(GL_GET_PROC_ADDRESS)
		#undef GL_GET_PROC_ADDRESS
		}

		// Release dummy context
		if (dummyContext.context)
		{
			contextMakeCurrent(nullptr, nullptr);
			SDL_GL_DeleteContext(dummyContext.context);
		}

		SDL_DestroyWindow(dummyContext.window);
	}

	return bOpenGLSupported;
}