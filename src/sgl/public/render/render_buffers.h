#ifndef SGL_RENDER_BUFFERS_H
#define SGL_RENDER_BUFFERS_H

#include "core_types.h"
#include "render_resource.h"
#include "templates/singleton.h"

/**
 * @class VertexBuffer render/render_buffers.h
 * @brief A packed collection of vertices, usually stored on the GPU
 */
class VertexBuffer : public RenderResource
{
public:
	/// @brief Destructor, do nothing
	virtual FORCE_INLINE ~VertexBuffer() override {}

	/// @copydoc RenderResource::releaseRHIResource()
	virtual FORCE_INLINE void releaseRHIResource() override
	{
		// We don't have vertexBufferRHI
	}

	/// @copydoc RenderResource::getName()
	virtual FORCE_INLINE String getName() const override { return "VertexBuffer"; }
};

/**
 * @class IndexBuffer render/render_buffers.h
 * @brief A packed collection of vertex indices, usually stored on the GPU
 */
class IndexBuffer : public RenderResource
{
public:
	/// @brief Destructor, do nothing
	virtual FORCE_INLINE ~IndexBuffer() override {}

	/// @copydoc RenderResource::releaseRHIResource()
	virtual FORCE_INLINE void releaseRHIResource() override
	{
		// We don't have indexBufferRHI
	}

	/// @copydoc RenderResource::getName()
	virtual FORCE_INLINE String getName() const override { return "IndexBuffer"; }
};

/**
 * @class GlobalDynamicVertexBuffer render/render_buffers.h
 * @brief A global buffer from which to allocate vertex buffers
 */
class GlobalDynamicVertexBuffer : public Singleton<GlobalDynamicVertexBuffer>
{
public:
	/// @brief Default-constructor, default
	GlobalDynamicVertexBuffer() = default;

	/// @todo
};

/**
 * @class GlobalDynamicIndexBuffer render/render_buffers.h
 * @brief A global buffer from which to allocate index buffers
 */
class GlobalDynamicIndexBuffer : public Singleton<GlobalDynamicIndexBuffer>
{
public:
	/// @brief Default-constructor, default
	GlobalDynamicIndexBuffer() = default;

	/// @todo
};

#endif