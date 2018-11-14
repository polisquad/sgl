#ifndef SGL_VEC_H
#define SGL_VEC_H

#include "core/platform.h"

/**
 * @brief Forward declarations
 * @{
 */
template<typename>	struct Vec2;
template<typename>	struct Vec3;
template<typename>	struct Vec4;
template<typename>	struct Quat;
template<typename>	struct Mat4;
/** @} */

/**
 * @struct Vec vec.h
 * @brief Base class for SIMD vector
 * that provides specialization for basic types
 */
template<typename T>
struct Vec
{
	/**
	 * @brief Print vector to stream
	 * 
	 * @param [in] stream output stream
	 */
	inline virtual void print(FILE * stream = stdout) const = 0;
};

/**
 * @brief Specialization for floating-point types
 * @{
 */
template<>
struct Vec<float32>
{
	typedef __m128 VT;
};
/** @} */

/**
 * @brief Specialization for integer types
 * @{
 */
template<>
struct Vec<int32>
{
	typedef __m128i VT;
};
/** @} */

#endif