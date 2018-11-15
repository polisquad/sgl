#ifndef SGL_VEC_H
#define SGL_VEC_H

#include "core/platform.h"

/**
 * @brief Forward declarations of structs
 * that use SIMD data types
 * @{
 */
template<typename>	struct Vec2;
template<typename>	struct Vec3;
template<typename>	struct Vec4;
template<typename>	struct Quat;
template<typename>	struct Mat4;
/** @} */

/**
 * @struct SIMD simd.h
 * @brief Base class for SIMD-based structs
 * that provides specialization for templates
 */
template<typename T>
struct SIMD {};

/**
 * @brief Specialization for floating-point types
 * @{
 */
template<>
struct SIMD<float32>
{
	// Vector type
	typedef __m128 VT;

	// Matrix type
	typedef __m128 MT[4];
};
/** @} */

/**
 * @brief Specialization for integer types
 * @{
 */
template<>
struct SIMD<int32>
{
	// Vector type
	typedef __m128i VT;

	// Matrix type
	typedef __m128i MT[4];
};
/** @} */

#endif