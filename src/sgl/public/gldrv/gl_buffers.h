#ifndef SGL_GL_BUFFERS_H
#define SGL_GL_BUFFERS_H

#include "core_types.h"
#include "rhi/rhi_buffers.h"

/// Just for autocompletion
#include "gldrv.h"

/// @brief Function that binds buffer to a context
typedef void (*BufferBindFunc)(GLuint);

/// @brief Buffer bind functions, defined in @ref gldrv.cpp
/// @{
extern void bindArrayBuffer(GLuint buffer);
extern void bindElementArrayBuffer(GLuint buffer);
/// @}

/**
 * @class GLBuffer gldrv/gl_buffers.h
 * @brief OpenGL wrapper for different buffer types
 */
template<class BufferClass, GLenum bufferType, BufferBindFunc bindFunc>
class GLBuffer : public BufferClass
{
protected:
	/// @brief OpenGL resource index
	GLuint resource;

	/// @brief Buffer real size
	uint32 usedSize;
	
public:
	/// @brief Initialization-constructor
	GLBuffer(uint32 _size, uint32 _stride, uint32 _usage, const void * data,
		GLuint reusedResource = 0, uint32 resourceSize = 0) : BufferClass(_size, _stride, _usage),
		resource(0)
	{
		if (true)
		{
			createBuffer(data, reusedResource, resourceSize);
		}
	}

	/// @brief Destructor, destory OpenGL resource
	~GLBuffer()
	{
		/// @todo No threads, just delete gl resource
		if (resource)
		{
			glDeleteBuffers(1, &resource);	
		}
	}

	/**
	 * @brief Create a new OpenGL buffer
	 * 
	 * @param [in]	data			data to submit to the GPU
	 * @param [in]	reusedResource	0 or existing resource
	 * @param [in]	resourceSize	resource size if known
	 */
	void createBuffer(const void * data, GLuint reusedResource, const uint32 resourceSize)
	{
		uint32 size = this->GetSize();
		usedSize = resourceSize ? resourceSize : size;

		if (reusedResource)
		{
			resource = reusedResource;

			// Bind buffer and submit data
			bindBuffer();
			glBufferSubData(bufferType, 0, size, data);
		}
		else
		{
			if (LIKELY(true)) // We assume every type is supported by OpenGL
			{
				// Generate a new buffer resource
				glGenBuffers(1, &resource);
				bindBuffer();
				
				glBufferData(bufferType, usedSize, nullptr, GL_STATIC_DRAW); /// @todo Don't hard code access
				if (data != nullptr) loadData(0U, PlatformMath::min(size, usedSize), data);
			}
		}
	}

	/// @brief Make current in context
	FORCE_INLINE void bindBuffer()
	{
		bindFunc(resource);
	}

	/**
	 * @brief Udpate buffer content on OpenGL server
	 * 
	 * @param [in]	data	new data
	 * @param [in]	size	data size
	 */
	void update(const void * data, uint32 size)
	{
		// Bind and resubmit data
		bindBuffer();
		loadData(0U, size, data);
	}

private:
	/// @brief Submit data to GPU
	FORCE_INLINE void loadData(uint32 offset, uint32 size, const void * data)
	{
		glBufferSubData(bufferType, offset, size, data);
	}
};

class GLBaseVertexBuffer : public RHIVertexBuffer
{
public:
	/// @brief Initialization-constructor
	GLBaseVertexBuffer(uint32 _size, uint32 _stride, uint32 _usage) : RHIVertexBuffer(_size, _usage) {}

	/// @brief Always false
	static bool isStructuredBuffer() { return false; }
};

class GLBaseIndexBuffer : public RHIIndexBuffer
{
public:
	/// @brief Initialization-constructor
	GLBaseIndexBuffer(uint32 _size, uint32 _stride, uint32 _usage) : RHIIndexBuffer(_size, _stride, _usage) {}

	/// @brief Always false
	static bool isStructuredBuffer() { return false; }
};

/// @brief OpenGL buffer types
/// @{
typedef GLBuffer<RHIVertexBuffer, GL_ARRAY_BUFFER, bindArrayBuffer> GLVertexBuffer;
typedef GLBuffer<GLBaseIndexBuffer, GL_ELEMENT_ARRAY_BUFFER, bindElementArrayBuffer> GLIndexBuffer;
/// @}

#endif