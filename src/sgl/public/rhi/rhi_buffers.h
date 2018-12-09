#ifndef SGL_RHI_BUFFERS_H
#define SGL_RHI_BUFFERS_H

#include "core_types.h"
#include "rhi_resource.h"

/**
 * @class RHIVertexBuffer rhi/rhi_buffers.h
 * @brief Handler for platform-specific vertex buffer
 */
class RHIVertexBuffer : public RHIResource
{
protected:
	/// @brief Size of the buffer in bytes
	uint32 size;

	/// @brief Declared usage of the buffer
	uint32 usage;

public:
	/// @brief Initialization-constructor
	RHIVertexBuffer(uint32 _size, uint32 _usage) : size(_size), usage(_usage) {}

	/// @brief Returns size of buffer in bytes
	FORCE_INLINE uint32 getSize() const { return size; }

	/// @brief Returns the declared usage of this buffer
	FORCE_INLINE uint32 getUsage() const { return usage; }
};

/**
 * @class RHIIndexBuffer rhi/rhi_buffers.h
 * @brief Handler for platform-specific index buffer
 */
class RHIIndexBuffer : public RHIResource
{
protected:
	/// @brief Size of the buffer in bytes
	uint32 size;

	/// @brief Stride of the buffer (i.e. distance between indices)
	uint32 stride;

	/// @brief Declared usage of the buffer
	uint32 usage;

public:
	/// @brief Initialization-constructor
	RHIIndexBuffer(uint32 _size, uint32 _stride, uint32 _usage) :
		size(_size),
		stride(_stride),
		usage(_usage) {}

	/// @brief Returns size of buffer in bytes
	FORCE_INLINE uint32 getSize() const { return size; }

	/// @brief Returns indices stride
	FORCE_INLINE uint32 getStride() const { return stride; }

	/// @brief Returns the declared usage of this buffer
	FORCE_INLINE uint32 getUsage() const { return usage; }
};

#endif