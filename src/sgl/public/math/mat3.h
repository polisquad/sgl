#pragma once

#include "core_types.h"
#include "templates/is_void.h"
#include "templates/simd.h"

/**
 * @struct Mat3 math/mat3.h
 * 
 * A 3x3 templated matrix
 * with support for vector intrinsics
 */
template<typename T, bool = hasVectorIntrinsics(T, 4)>
struct Mat3
{
public:
	union
	{
		/// Matrix buffer
		T matrix[3][3];

		/// Linear buffer
		T array[9];

		struct
		{
			/// Single components
			/// @todo Maybe unnecessary
			/// @{
			T _a, _b, _c, _d, _e, _f, _g, _h, _i;
			/// @}
		};
	};

public:
	/// Default constructor
	FORCE_INLINE Mat3() {}

	/// Elements constructor
	FORCE_INLINE Mat3(
		T __a, T __b, T __c,
		T __d, T __e, T __f,
		T __g, T __h, T __i
	) : array{
		__a, __b, __c,
		__d, __e, __f,
		__g, __h, __i
	} {}

	/// Scalar constructor, fill matrix
	FORCE_INLINE Mat3(T s) : array{
		s, s, s,
		s, s, s,
		s, s, s
	} {}

	/// Array constructor
	FORCE_INLINE Mat3(const T _array[9]) { memcpy(array, _array, sizeof(array)); /** @todo Use @ref Memory::memcpy instead */ }
};