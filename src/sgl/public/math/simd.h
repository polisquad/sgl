#ifndef SGL_VEC_H
#define SGL_VEC_H

#include "math.tpp"

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

/**
 * @brief Union to access simd vector elements
 */
#define __m128_getter\
	union {\
		__m128 _v_;\
		float32 _a_[4];\
	}

#endif