#ifndef SGL_IS_INTEGRAL_H
#define SGL_IS_INTEGRAL_H

#include "core_types.h"
#include <type_traits>

/**
 * Just a typedef.
 * And it's gonna stay like that
 */
template<typename T>
using IsIntegral = std::is_integral<T>;

/// @brief Quick value of @ref IsIntegral
#define IsIntegralV(expr) IsIntegral<expr>::value

#endif