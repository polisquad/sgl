#pragma once

#include "core_types.h"
#include "containers/string.h"
#include "templates/singleton.h"

/**
 * @class GenericApp app/generic/generic_app.h
 * @brief Generic application
 */
class GenericApp : public Singleton<GenericApp>
{
public:
	/// @brief Default-constructor
	GenericApp() VOID_FUNC;

	/// @brief Virtual destructor
	virtual ~GenericApp() VOID_FUNC;

	/// @brief Master tick event
	virtual void tick(const float dt) VOID_FUNC;

	/// @brief Shuts down application
	virtual void shutdown() VOID_FUNC;
};

