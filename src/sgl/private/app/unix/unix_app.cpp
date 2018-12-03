#include "app/unix/unix_app.h"
#include "app/unix/unix_app_misc.h"

/// Global application
UnixApp * unixApp = nullptr;

UnixApp * UnixApp::createUnixApp()
{
	if (!UnixAppMisc::initSDL())
	{
		/// @todo Handle error
		return nullptr; 
	}

	// Create application singleton
	unixApp = new UnixApp();
	return unixApp;
}

UnixApp::~UnixApp() VOID_FUNC

void UnixApp::shutdown() VOID_FUNC