#pragma once

#include "generic/generic_app_misc.h"

#if PLATFORM_WINDOWS
	#include "windows/windows_app_misc.h"
#elif PLATFORM_APPLE
	#include "apple/apple_app_misc.h"
#elif PLATFORM_UNIX
	#include "unix/unix_app_misc.h"
#else
	#error Unkown platform
#endif