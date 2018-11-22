#ifndef SGL_GENERIC_PLATFORM_H
#define SGL_GENERIC_PLATFORM_H

/**
 * @struct GenericPlatformTypes generic/generic_platform.h
 * @brief Generic type definitions
 */
struct GenericPlatformTypes
{
	/**
	 * @brief Integer types
	 * @{
	 */
	typedef signed char			byte;
	typedef signed char			int8;
	typedef signed short		int16;
	typedef signed int			int32;
	typedef signed long long	int64;

	typedef unsigned char		ubyte;
	typedef unsigned char		uint8;
	typedef unsigned short		uint16;
	typedef unsigned int		uint32;
	typedef unsigned long long	uint64;
	/** @} */

	/**
	 * @brief Floating-point types
	 * @{
	 */
	typedef float		float32;
	typedef double		float64;
	typedef long double	float128;
	/** @} */
};

#endif