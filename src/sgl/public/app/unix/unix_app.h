#ifndef SGL_UNIX_APPLICATION_H
#define SGL_UNIX_APPLICATION_H

#include "../generic/generic_app.h"

/// Simple DirectMedia Library
#include <SDL.h>

/**
 * @class UnixApp app/unix/unix_app.h
 * @brief Unix specific application 
 */
class UnixApp : public GenericApp
{
public:
	/// @brief Application entry point
	static UnixApp * createUnixApp();

public:
	/// @brief Destructor
	virtual ~UnixApp();

	/// @copydoc GenericApp::shutdown()
	virtual void shutdown() override;
};

/// Global Unix application
extern UnixApp * unixApp;

#endif