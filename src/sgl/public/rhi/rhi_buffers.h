#pragma once

#include "core_types.h"
#include "rhi_resource.h"
#include "rhi_types.h"
#include "containers/array.h"

/**
 * @class RHIUniformBufferLayout rhi/rhi_buffers.h
 * @brief Layout of a uniform buffer (@ref https://www.khronos.org/opengl/wiki/Uniform_Buffer_Object)
 */
struct RHIUniformBufferLayout
{
public:
	/// @brief Size of the buffer
	uint32 bufferSize;

	/// @brief Offset of each resource in the buffer (in bytes)
	Array<uint32> resourceOffsets;

	/// @brief The type of each resource in the buffer
	Array<UniformBufferBaseType> resourceTypes;

public:
	/// @brief Default-constructor
	RHIUniformBufferLayout() = default;
};

/**
 * @class RHIUniformBuffer rhi/rhi_buffers.h
 * @brief Uniform buffer handler
 */
class RHIUniformBuffer : public RHIResource
{
protected:
	/// @brief Uniform buffer layout
	RHIUniformBufferLayout * layout;

public:
	/// @brief Initialization-constructor
	RHIUniformBuffer(RHIUniformBufferLayout * _layout) : layout(_layout) {}

	/// @brief Get buffer size
	FORCE_INLINE uint32 getSize() const { return layout->bufferSize; }

	/// @brief Get layout ref
	FORCE_INLINE RHIUniformBufferLayout & getLayout() { return *layout; }
};

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

