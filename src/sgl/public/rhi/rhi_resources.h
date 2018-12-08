#ifndef SGL_RHI_RESOURCES_H
#define SGL_RHI_RESOURCES_H

#include "core_types.h"
#include "hal/thread_safe_counter.h"
#include "templates/ref_count.h"

/**
 * @class RHIResource rhi/rhi.h
 * @brief Base class for all RHI resources
 */
class RHIResource
{
protected:
	/// @brief Reference counter
	mutable ThreadSafeCounterU32 refCount;

public:
	/// @brief Default-constructor, default
	RHIResource() = default;

	/// @brief Destructor, ensures reference count is zero
	virtual FORCE_INLINE ~RHIResource() {}

	/// @brief Add referencing object
	FORCE_INLINE uint32 addRef() const { return refCount.increment(); }

	/// @brief Remove referencing object
	FORCE_INLINE uint32 release() const // Sooo, they are using const AND mutable ... why?
	{
		// Decrement count and check if delete is required
		uint32 counter = refCount.decrement();
		if (counter == 0) delete this;

		return counter;
	}

	/// @brief Return number of observers that reference this resource
	FORCE_INLINE uint32 getRefCount() const { return refCount.get(); }
};

class Viewport : public RHIResource
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

/////////////////////////////////////////////////
// Resources ref pointers                      //
/////////////////////////////////////////////////

typedef RefCountPtr<Viewport> ViewportRef;

#endif