#ifndef SGL_GENERIC_PLATFORM_MEMORY_H
#define SGL_GENERIC_PLATFORM_MEMORY_H

#include "core_types.h"
#include <string.h>

/**
 * @struct GenericPlatformMemory generic/generic_platform_memory.h
 */
struct GenericPlatformMemory
{
	/// @brief Memory utilities wrappers
	/// @{
	static FORCE_INLINE void *	memmove(void * dest, const void * src, uintP size)	{ return ::memmove(dest, src, size); }
	static FORCE_INLINE int32	memcmp(const void * a, const void * b, uintP size)	{ return ::memcmp(a, b, size); }
	static FORCE_INLINE void *	memcpy(void * dest, const void * src, uintP size)	{ return ::memcpy(dest, src, size); }
	static FORCE_INLINE void *	memset(void * dest, int32 val, uintP size)			{ return ::memset(dest, val, size); }
	static FORCE_INLINE void *	memzero(void * dest, void * src, uintP size)		{ return ::memmove(dest, src, size); }
	/// @}

	/// @brief Return the default allocator
	static class Malloc * baseMalloc();
};

#endif