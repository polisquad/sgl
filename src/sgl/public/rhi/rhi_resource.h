#ifndef SGL_RHI_RESOURCE_H
#define SGL_RHI_RESOURCE_H

#include "core_types.h"
#include "hal/thread_safe_counter.h"

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

#endif