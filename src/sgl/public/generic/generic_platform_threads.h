#ifndef SGL_GENERIC_PLATFORM_THREADS_H
#define SGL_GENERIC_PLATFORM_THREADS_H

#include "core_types.h"

/**
 * @struct GenericPlatformTLS generic/generic_platform_threads.h
 * @brief Generic interface for TLS (Thread Local Storage)
 */
struct GenericPlatformTLS
{
	/// @brief Returns if it's valid slot
	static FORCE_INLINE bool isValidSlot(uint32 slotIdx) { return slotIdx != 0xffffffff; }
};

#endif