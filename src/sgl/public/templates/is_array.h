#ifndef SGL_IS_ARRAY_H
#define SGL_IS_ARRAY_H

#include "core_types.h"
#include <type_traits>

/**
 * Just a typedef.
 * And it's gonna stay like that
 */
template<typename T>
using IsArray = std::is_array<T>;

/// @brief Quick value of @ref IsPointer
#define IsArrayV(type) IsArray<type>::value

#endif