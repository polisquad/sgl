#pragma once

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

