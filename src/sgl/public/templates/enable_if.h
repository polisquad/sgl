#pragma once

#include "core_types.h"
#include <type_traits>

/**
 * Just a typedef.
 * This will probably remain a typedef
 */
template<bool V, typename T = void>
using EnableIf = std::enable_if<V, T>;

/**
 * Just a typedef.
 * This will probably remain a typedef
 */
template<bool V, typename T = void>
using EnableIfT = std::enable_if_t<V, T>;

