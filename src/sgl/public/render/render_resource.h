#ifndef SGL_RENDER_RESOURCE
#define SGL_RENDER_RESOURCE

#include "core_types.h"
#include "containers/string.h"
#include "containers/linked_list.h"
#include "rhi/rhi.h"

/**
 * @class RenderResource render/render_resource.h
 * @brief Base class for all render resources
 */
class RenderResource
{
protected:
	/// @brief Flag that indicates if resource is initialized
	bool bInitialized;

	/// @brief Link in the resource global list
	/// 
	/// Resources are linked in a global linked list
	/// of resources
	Link<RenderResource*> resourceLink;

public:
	/// @brief Default constructor
	RenderResource() : bInitialized(false), resourceLink(this) {};

	/// @brief Destructor, virtual
	virtual ~RenderResource();

	/// @brief Get resource display name
	virtual FORCE_INLINE String getName() const { return "Unnamed"; }

	/// @brief Returns @c true if resource was initialized and not yet released
	FORCE_INLINE bool isInitialized() const { return bInitialized; }

	/// @brief Initialize this resource
	virtual void init();

	/// @brief Release this resource
	virtual void release();

	/**
	 * @brief Initializes the RHI resource used by this resource
	 * 
	 * Called when the resource and the RHI are initialized
	 */
	virtual void initRHIResource() {}

	/// @brief Releases the RHI resource used by this resource
	virtual void releaseRHIResource() {}
};

#endif