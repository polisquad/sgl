#pragma once

#include "render_resource.h"

/**
 * @class Texture render/texture.h
 * @brief Base class for texture resources
 */
class Texture : public RenderResource
{
public:
	/// @brief RHI resource
	RHITextureRef resource;

	/// @brief Flag for grayscale images
	uint8 bGrayscale : 1;

public:
	/// @brief Default constructor
	Texture() :
		resource(nullptr),
		bGrayscale(false) {}

	/// @brief Destructor, do nothing
	virtual FORCE_INLINE ~Texture() override {}

	/// @brief Returns texture size in pixels
	/// @{
	virtual FORCE_INLINE uint32 getWidth() const { return 0; }
	virtual FORCE_INLINE uint32 getHeight() const { return 0; }
	virtual FORCE_INLINE uint32 getSize() const { return 0; }
	/// @}

	/// @copydoc RenderResource::getName()
	virtual FORCE_INLINE String getName() const override { return "Texture"; }

	/// @copydoc RenderResource::releaseRHIResource()
	virtual FORCE_INLINE void releaseRHIResource() override
	{
		// Release RHI resource
		resource.release();
	}
};

