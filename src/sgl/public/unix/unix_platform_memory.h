#ifndef SGL_UNIX_PLATFORM_MEMORY_H
#define SGL_UNIX_PLATFORM_MEMORY_H

#include "core_types.h"

/**
 * @struct UnixPlatformMemory unix/unix_platform_memory.h
 */
struct UnixPlatformMemory : public GenericPlatformMemory
{
	// Empty
};
typedef UnixPlatformMemory PlatformMemory;

#endif