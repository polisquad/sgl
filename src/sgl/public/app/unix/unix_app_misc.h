#ifndef SGL_UNIX_APP_MISC_H
#define SGL_UNIX_APP_MISC_H

#include "../generic/generic_app_misc.h"

/**
 * @struct UnixAppMisc app/unix/unix_app_misc.h
 * @brief Unix specific application misc functions
 */
struct UnixAppMisc : public GenericAppMisc
{
	/// @copydoc GenericAppMisc::init()
	static void init();

	/// @copydoc GenericAppMisc::createApp()
	static GenericApp * createApp();

	/// @brief Initialize SDL
	static bool initSDL();
};

typedef UnixAppMisc AppMisc;

#endif