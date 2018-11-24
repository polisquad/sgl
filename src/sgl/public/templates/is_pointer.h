#ifndef SGL_IS_POINTER_H
#define SGL_IS_POINTER_H

#include "core_types.h"
#include <type_traits>

/**
 * Just a typedef.
 * And it's gonna stay like that
 */
template<typename T>
using IsPointer = std::is_pointer<T>;

/// @brief Quick value of @ref IsPointer
#define IsPointerV(expr) IsPointer<expr>::value

#endif