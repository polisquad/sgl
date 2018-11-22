#ifndef SGL_UNIX_PLATFORM_MATH_H
#define SGL_UNIX_PLATFORM_MATH_H

#include "core_types.h"

/**
 * @struct UnixPlatformMath unix/unix_platform_math.h
 * @brief Unix specific math implementation
 */
struct UnixPlatformMath : public GenericPlatformMath
{
	// Empty
};
typedef UnixPlatformMath PlatformMath;

#endif