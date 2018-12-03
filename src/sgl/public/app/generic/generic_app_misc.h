#ifndef SGL_GENERIC_APP_MISC_H
#define SGL_GENERIC_APP_MISC_H

#include "core_types.h"
#include "generic_app.h"

/**
 * @struct GenericAppMisc app/generic/generic_app_misc.h
 * @brief Misc application functions
 */
struct GenericAppMisc
{
	/// @brief Init application
	static void init();

	/// @brief Creates the application
	/// @note Called by the @ref SlateApplication
	static GenericApp * createApp();
};

#endif