#ifndef SGL_GENERIC_PLATFORM_MATH_H
#define SGL_GENERIC_PLATFORM_MATH_H

#include "core_types.h"
#include "hal/platform_stdlib.h"

/**
 * @struct GenericPlatformMath generic/generic_platform_math.h
 * @brief Common math functions
 */
struct GenericPlatformMath
{
	/**
	 * @brief Trigonometric functions
	 * @{
	 */
	static FORCE_INLINE float32 sin(float32 s)	{ return ::sinf(s); }
	static FORCE_INLINE float32 cos(float32 s)	{ return ::cosf(s); }
	static FORCE_INLINE float32 tan(float32 s)	{ return ::tanf(s); }
	static FORCE_INLINE float32 asin(float32 s)	{ return ::asinf(s); }
	static FORCE_INLINE float32 acos(float32 s)	{ return ::acosf(s); }
	static FORCE_INLINE float32 atan(float32 s)	{ return ::atanf(s); }
	/** @} */

	/**
	 * @brief Power functions
	 * @{
	 */
	static FORCE_INLINE float32 sqrt(float32 s)				{ return ::sqrtf(s); }
	static FORCE_INLINE float32 pow(float32 a, float32 b)	{ return ::powf(a, b); }
	/** @} */

	/**
	 * @brief Random functions
	 * @{
	 */
	static FORCE_INLINE void initRand(int32 seed)	{ ::srand(seed); }
	static FORCE_INLINE int32 rand()				{ return ::rand(); }
	static FORCE_INLINE float32 randf()				{ return ::rand() / static_cast<float32>(RAND_MAX); }
	/** @} */

	/**
	 * @brief Templated common functions
	 * @{
	 */
	template<typename T>
	static CONSTEXPR FORCE_INLINE T abs(const T s)
	{
		return s < T(0) ? -s : s;
	}

	template<typename T>
	static CONSTEXPR FORCE_INLINE T sign(const T s)
	{
		return s < T(0) ? T(-1) : (s > T(0) ? T(1) : T(0));
	}

	template<typename T>
	static CONSTEXPR FORCE_INLINE T max(const T a, const T b)
	{
		return a > b ? a : b;
	}

	template<typename T>
	static CONSTEXPR FORCE_INLINE T min(const T a, const T b)
	{
		return a < b ? a : b;
	}
	/** @} */

	/**
	 * @brief Get smallest power of two greater than n
	 * 
	 * @param n lower bound
	 * 
	 * @return next p2
	 * @{
	 */
	static CONSTEXPR FORCE_INLINE uint64 getNextPowerOf2(uint64 n)
	{
		// Case where n is already 2^x
		if (!(n & (n - 1))) return n;

		uint32 out = 1;
		while (n) n >>= 1, out <<= 1;
		return out;
	}

	static CONSTEXPR FORCE_INLINE uint8 getNextPowerOf2Index(uint64 n)
	{
		uint32 out = 0;
		while (n) n >>= 1, out += 1;
		return out;
	}
	/** @} */
};

/// Float-32 specialization
template<>
FORCE_INLINE float32 GenericPlatformMath::abs(float32 s)
{
	return fabsf(s);
}

#endif