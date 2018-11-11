#ifndef SGL_VEC_H
#define SGL_VEC_H

#include "core/platform.h"

/**
 * @struct Vec vec.h
 * @brief Base class for SIMD vector
 * that provides specialization for basic types
 */
template<typename T>
struct Vec {};

/**
 * @brief Specialization for int32
 */
template<>
struct Vec<int32>
{
	typedef __m128i VT;
};

/**
 * @brief Specialization for float32
 */
template<>
struct Vec<float32>
{
	typedef __m128 VT;
};

#endif