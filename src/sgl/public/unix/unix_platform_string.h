#ifndef SGL_UNIX_PLATFORM_STRING_H
#define SGL_UNIX_PLATFORM_STRING_H

#include "core_types.h"

/**
 * @struct UnixPlatformString unix/unix_platform_string.h
 */
struct UnixPlatformString : public GenericPlatformString
{
	// Empty
};
typedef UnixPlatformString PlatformString;

#endif