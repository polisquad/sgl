#pragma once

#include "core_types.h"
#include "rhi_resource.h"
#include "math/vec2.h"

class RHIViewport : public RHIResource
{
public:
	/// @brief Returns reference of a backbuffer texture
	virtual void * getNativeBackBufferTexture() const { return nullptr; }

	/// @brief Returns reference of a backbuffer render target
	virtual void * getNativeBackBufferRenderTarget() const { return nullptr; }

	/// @brief Returns the native window
	virtual void * getNativeWindow() const { return nullptr; }

	/// @brief Tick event called by the game thread
	virtual void tick(float dt) {}
};

