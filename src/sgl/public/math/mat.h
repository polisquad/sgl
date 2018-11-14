#ifndef SGL_MAT_H
#define SGL_MAT_H

#include "core/platform.h"

/**
 * @struct Mat mat.h
 * @brief Base struct for SIMD implementation of a matrix
 */
template<typename T>
struct Mat {};

/**
 * @brief Floating-point specializations
 */
template<>
struct Mat<float32>
{
	typedef __m128 MT[4];
};
/** @} */

#endif