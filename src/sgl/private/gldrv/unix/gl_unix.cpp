#include "gldrv/unix/gl_unix.h"
#include "gldrv/gldrv.h"
#include "hal/critical_section.h"
#include "app/unix/unix_app_misc.h"

#include "SDL.h"

#define GL_DECLARE_ENTRYPOINTS(type, func) type func = 0;
namespace GLFuncPointers
{
	GL_ENTRYPOINTS(GL_DECLARE_ENTRYPOINTS)
}
#undef GL_DECLARE_ENTRYPOINTS

/**
 * @brief Platform specific OpenGL context
 */
struct GLContext
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

	/// @brief Creates a dummy window, used for context initialization
	FORCE_INLINE void createDummyGLWindow(GLContext * outContext)
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

	void createGLContextCore(GLContext * outContext)
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

/**
 * @class ScopeContext
 * @brief A scope context automatically activate and deactivate
 * a given context in its scope
 */
class ScopeContext
{
protected:
	/// @brief Saved context
	/// @{
	SDL_Window * prevWindow;
	SDL_GLContext prevContext;
	bool bSameContext;
	/// @}

public:
	FORCE_INLINE ScopeContext(GLContext * context)
	{
		// Save current context for restoring it later
		prevWindow = SDL_GL_GetCurrentWindow();
		prevContext = SDL_GL_GetCurrentContext();
		bSameContext = prevContext == context->context;

		if (!bSameContext)
		{
			// Flush queue of previous context before
			// switching context
			if (prevContext != nullptr) glFlush();
			contextMakeCurrent(context->window, context->context);
		}
	}

	FORCE_INLINE ~ScopeContext()
	{
		// We need to restore the previous context
		// if it is different from scoped
		if (!bSameContext)
		{
			// Finish queue of scoped
			glFlush();

			if (prevContext != nullptr)
				contextMakeCurrent(prevWindow, prevContext);
			else
				contextMakeCurrent(nullptr, nullptr);
		}
	}
};

/**
 * @struct GLDevice
 * @brief Platform specific OpenGL device
 */
struct GLDevice
{
	/// @brief Shared context
	GLContext sharedContext;

	/// @brief Rendering context
	GLContext renderingContext;

	/// @brief Guards against mulit-thread access
	CriticalSection * accessControl;

	/// @brief Default-constructor
	GLDevice()
	{
		// Init access control
		accessControl = new CriticalSection;

		// Shared context
		assert(SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 0) == 0);
		createDummyGLWindow(&sharedContext);
		createGLContextCore(&sharedContext);

		if (UNLIKELY(sharedContext.context == nullptr))
		{
			/// @todo Handle error
			return;
		}

		{
			ScopeContext scope(&sharedContext);

			// Generate a vao
			glGenVertexArrays(1, &sharedContext.vao);
			glBindVertexArray(sharedContext.vao);

			initGLContextWithDefaults();
		}
		assert(SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1) == 0);

		// Rendering context
		createDummyGLWindow(&renderingContext);
		createGLContextCore(&renderingContext);

		if (UNLIKELY(renderingContext.context == nullptr))
		{
			/// @todo Handle error
			return;
		}

		{
			ScopeContext scope(&renderingContext);
			
			// Generate a vao
			glGenVertexArrays(1, &renderingContext.vao);
			glBindVertexArray(renderingContext.vao);

			initGLContextWithDefaults();
		}
	}

	/// @brief Destructor
	~GLDevice()
	{
		// Detach current context
		contextMakeCurrent(nullptr, nullptr);

		delete accessControl;
	}
};

/// @brief Create a default platform device
GLDevice * createDefaultGLDevice()
{
	return new GLDevice;
}

/// @todo I'm waiting to bring in math.h
template<typename T>
struct Vec2
{
	T x,y;

	Vec2() = default;
	Vec2(T _x, T _y) : x(_x), y(_y) {}
};

/// @brief Transfers data to on-screen framebuffer (0)
bool blitToViewport(GLDevice * device, const GLViewport & viewport, const Vec2<uint32> & backbufferSize)
{
	GLContext * const context = viewport.getContext();

	// Lock device usage
	ScopeLock _(device->accessControl);

	{
		// Make active viewport context
		ScopeContext _(context);

		// Bind screen framebuffer to draw buffer
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK);

		// Bind viewport buffer to read buffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, context->viewportFramebuffer);
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		glDisable(GL_FRAMEBUFFER_SRGB);

		int width = 0, height = 0;
		SDL_GL_GetDrawableSize(context->window, &width, &height);

		// Draw filter and rectangle
		GLenum filter;
		GLint destX0, destY0, destX1, destY1;

		// Nothing to draw
		if (!width | !height) return false;

		if (width == backbufferSize.x & height == backbufferSize.y)
		{
			// Size matches, use nearest filter
			filter = GL_NEAREST;

			// Flip vertically
			destX0 = destY1 = 0;
			destX1 = width, destY0 = height;
		}
		else
		{
			filter = GL_LINEAR;

			const float32 targetAspectRatio = float32(backbufferSize.x) / float32(backbufferSize.y);
			const float32 windowAspectRatio = float32(width) / float32(height);

			if (targetAspectRatio > windowAspectRatio)
			{
				const float32 scaledW = height * targetAspectRatio;
				const float32 scaledX = (float32(width) - scaledW) / 2.f;

				destX0 = scaledX, destY1 = 0;
				destX1 = scaledX + scaledW, destY1 = height;
			}
			else if (targetAspectRatio < windowAspectRatio)
			{
				/// @todo this is wrong
				const float32 scaledH = width * targetAspectRatio;
				const float32 scaledY = (float32(height) - scaledH) / 2.f;

				destX0 = 0, destY1 = scaledY;
				destX1 = width, destY1 = scaledY + scaledH;
			}
			else
			{
				// Flip vertically
				destX0 = destY1 = 0;
				destX1 = width, destY0 = height;
			}
		}

		// Blit viewport framebuffer to screen
		glBlitFramebuffer(0, 0, backbufferSize.x, backbufferSize.y, destX0, destY0, destX1, destY1, GL_COLOR_BUFFER_BIT, filter);

		{
			// Swap backbuffer
			SDL_GL_SwapWindow(context->window);
			glEnable(GL_FRAMEBUFFER_SRGB);
		}
	} // ScopeContext(context)

	return true;
}

/// @brief Wrapper for glFinish
FORCE_INLINE void flushIfNeeded() { glFinish(); }

/// @brief Get current window backbuffer size
Vec2<uint32> getBackbufferSize()
{
	auto window = SDL_GL_GetCurrentWindow();

	int width = 0, height = 0;
	if (window) SDL_GL_GetDrawableSize(window, &width, &height);

	return Vec2<uint32>(width, height);
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
		if (UNLIKELY(SDL_GL_LoadLibrary(nullptr) != 0))
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
		GLContext dummyContext;
		createDummyGLWindow(&dummyContext);
		createGLContextCore(&dummyContext);

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