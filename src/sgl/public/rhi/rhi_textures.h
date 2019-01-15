#pragma once

#include "core_types.h"
#include "rhi_resource.h"
#include "rhi_types.h"
#include "math/vec2.h"

/**
 * @class RHITexture rhi/rhi_textures.h
 * @brief Base class for all rhi textures
 */
class RHITexture : public RHIResource
{
protected:
	/// @brief Number of mip-maps
	uint32 numMips;
	
	/// @brief Pixel format of the texture
	PixelFormat pixelFormat;

public:
	/// @brief Default-constructor, removed
	RHITexture() = delete;

	/// @brief Initialization-constructor
	RHITexture(uint32 _numMips, PixelFormat _pixelFormat) :
		numMips(_numMips),
		pixelFormat(_pixelFormat) {}

	/// @brief Cast to subclasses, avoids dynamic_cast calls
	/// @{
	virtual FORCE_INLINE class RHITexture2D * getTexture2D()			{ return nullptr; }
	virtual FORCE_INLINE class RHITexture3D * getTexture3D()			{ return nullptr; }
	virtual FORCE_INLINE class RHITexture2DArray * getTexture2DArray()	{ return nullptr; }
	virtual FORCE_INLINE class RHITextureCube * getTextureCube()		{ return nullptr; }
	/// @}
	
	/// @brief Returns number of mip-maps
	uint32 FORCE_INLINE getNumMips() const { return numMips; }

	/// @brief Returns pixel format of the texture
	PixelFormat FORCE_INLINE getPixelFormat() const { return pixelFormat; }
};

/**
 * @class RHITexture2D rhi/rhi_textures.h
 * @brief A 2D texture
 */
class RHITexture2D : public RHITexture
{
protected:
	/// @brief Texture size
	dim2 size;

public:
	/// @Initialization-constructor
	RHITexture2D(uint32 _size, uint32 _numMips, PixelFormat _pixelFormat) : RHITexture(_numMips, _pixelFormat),
		size(_size) {}
	
	/// @brief Dynamic cast method
	virtual RHITexture2D * getTexture2D() override { return this; }

	/// @brief Returns size of the texture
	/// @{
	FORCE_INLINE uint32	getWidth() const	{ return size.x; }
	FORCE_INLINE uint32	getHeight() const	{ return size.y; }
	FORCE_INLINE dim2	getSize() const		{ return size; }
	/// @}
};

