#ifndef SGL_PLATFORM_ATOMICS_H
#define SGL_PLATFORM_ATOMICS_H

#include "core_types.h"

#include "generic/generic_platform_atomics.h"
#if PLATFORM_WINDOWS
	#include "windows/windows_platform_atomics.h"
#elif PLATFORM_APPLE
	#include "apple/apple_platform_atomics.h"
#elif PLATFORM_UNIX
	#include "unix/unix_platform_atomics.h"
#else
	#error "Unkown platform"
#endif

#endif