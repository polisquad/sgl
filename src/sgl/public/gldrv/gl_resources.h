#ifndef SGL_GL_RESOURCES_H
#define SGL_GL_RESOURCES_H

#include "rhi/rhi_resources.h"

/// Forward declarations
struct GLContext;



/**
 * @class GLViewport gldrv/gl_resources.h
 * @brief OpenGL implementation of RHI viewport
 */
class GLViewport : public Viewport
{
private:
	/// @brief Context that owns this viewport
	GLContext * context;

	/// @brief Screen size
	/// @{
	uint32 width, height;
	/// @}

	/// @brief Indicates if viewport is full-screen
	bool bFullscreen;

public:
	/// @brief Resize event
	void resize(uint32 _width, uint32 _height);

	/// @brief Returns screen size
	/// @{
	FORCE_INLINE void getSize(uint32 & _width, uint32 & _height) const { _width = width; _height = height; }
	/// @}

	/// @brief Returns fullscreen flag
	FORCE_INLINE bool isFullscreen() const { return bFullscreen; }

	/// @brief Returns OpenGL context
	FORCE_INLINE GLContext * getContext() const { return context; }

	//////////////////////////////////////////////////
	// Viewport interface                           //
	//////////////////////////////////////////////////
	
	/// @copydoc Viewport::getNativeWindow()
	virtual void * getNativeWindow() const override;
};

#endif