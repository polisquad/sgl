#ifndef SGL_VEC_H
#define SGL_VEC_H

#include "math.hpp"

#if PLATFORM_ENABLE_SIMD
	#include <immintrin.h> // AVX header
#endif

/**
 * @struct SIMD simd.h
 * @brief Base class for SIMD-based structs
 * that provides specialization for templates
 */
template<typename T>
struct SIMD
{
	// Vector type
	typedef T VT[4];

	// Matrix type
	typedef T MT[4][4];
};

/////////////////////////////////////////////////
// SIMD types                                  //
/////////////////////////////////////////////////

#if PLATFORM_ENABLE_SIMD
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

template<>
struct SIMD<float64>
{
	// Vector type
	typedef __m256d VT;

	// Matrix type
	typedef __m256d MT[4];
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

template<>
struct SIMD<uint32>
{
	// Vector type
	typedef __m128i VT;

	// Matrix type
	typedef __m128i MT[4];
};

template<>
struct SIMD<int64>
{
	// Vector type
	typedef __m256i VT;

	// Matrix type
	typedef __m256i MT[4];
};
/** @} */

/**
 * @brief Getter for float-32 128-bit register
 */
#define __m128_getter_ps \
	union\
	{\
		__m128 _data_;\
		float32 _vec_[4];\
	}
#endif

#endif