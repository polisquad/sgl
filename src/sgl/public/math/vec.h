#pragma once

#include "core_types.h"
#include "hal/platform_math.h"

/**
 * A fixed-length mathematical vector
 */
template<typename T, uint32 N>
struct Vec
{
public:
	/// Array buffer
	T buffer[N];
};