#pragma once

#include "core_types.h"
#include "hal/platform_math.h"

/// Returns true if <T, N> has a vector intrinsics type
#define hasVectorIntrinsics(T, N) !IsVoid<typename Simd::Vector<T, N>::Type>::value

namespace Simd
{
	/**
	 * Expose a vector intrinsic type for the given
	 * type and size and provides convenient
	 * methods to manipulate vector intrinsics
	 */
	template<typename T, uint8 N>
	struct Vector { using Type = void; };
} // Simd

#if PLATFORM_ENABLE_SIMD

namespace Simd
{
	/**
	 * Comparison operations
	 */
	enum CompareOp : int32
	{
		CMP_EQ = _MM_CMPINT_EQ,
		CMP_NE = _MM_CMPINT_NE,
		CMP_GT = _MM_CMPINT_GT,
		CMP_LT = _MM_CMPINT_LT,
		CMP_GE = _MM_CMPINT_GE,
		CMP_LE = _MM_CMPINT_LE
	};

	/// Float specialization
	template<>
	struct Vector<float32, 4>
	{
	public:
		/// Intrinsic type
		using Type = __m128;

		/// Constants
		/// @{
		static const Type zero;
		static const Type unit;
		static const Type neg;
		static const Type rowEven;
		static const Type rowOdd;
		/// @}

	public:
		/// Load methods
		/// @{
		/**
		 * Creates a vector from scalar values
		 * 
		 * @param [in] a-h scalar operands
		 * @return new vector
		 */
		static CONSTEXPR FORCE_INLINE Type load(float32 a, float32 b, float32 c, float32 d)
		{
			return Type{a, b, c, d};
		}

		/**
		 * Creates a vector from a single scalar value
		 * 
		 * @param [in] s scalar operand
		 * @return new vector
		 */
		static CONSTEXPR FORCE_INLINE Type load(float32 s) { return Type{s, s, s, s}; }

		/**
		 * Creates a vector from a memory buffer
		 * 
		 * Memory is required to be 32-Bytes aligned
		 * 
		 * @param [in] buffer memory buffer
		 * @return new vector
		 */
		static FORCE_INLINE Type load(const float32 * buffer) { return _mm_load_ps(buffer); }

		/**
		 * Same as @copydoc load, but no alignment required
		 */
		static FORCE_INLINE Type loadu(const float32 * buffer) { return _mm_loadu_ps(buffer); }
		/// @}

		/**
		 * Arithmetic operations
		 * 
		 * @param [in] v1,v2 intrinics operands
		 * @return new vector
		 * @{
		 */
		static FORCE_INLINE Type add(Type v1, Type v2) { return _mm_add_ps(v1, v2); }
		static FORCE_INLINE Type sub(Type v1, Type v2) { return _mm_sub_ps(v1, v2); }
		static FORCE_INLINE Type mul(Type v1, Type v2) { return _mm_mul_ps(v1, v2); }
		static FORCE_INLINE Type div(Type v1, Type v2) { return _mm_div_ps(v1, v2); }

		/// Horizontal add
		/// @see https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=3306,5554,5559,3327,3328,6068,6068,6011,3826,3816,4169,569,2394,3826,4169,4164,6011,3820,4085,3816,4896,3898,2914,2914&text=_mm256_hadd_ps
		static FORCE_INLINE Type hadd(Type v1, Type v2) { return _mm_hadd_ps(v1, v2); }
		/// @}

		/**
		 * Bitwise operations
		 * 
		 * @param [in] v1,v2 intrinsics operand
		 * @return new vector
		 */
		static FORCE_INLINE Type bor(Type v1, Type v2) { return _mm_or_ps(v1, v2); }
		static FORCE_INLINE Type bxor(Type v1, Type v2) { return _mm_xor_ps(v1, v2); }
		/// @}

		/**
		 * Compare two vector intrinsics
		 * 
		 * @param [in] op comparison operation
		 * @param [in] v1,v2 intrinsics operands
		 * @return comparison result as bitmask
		 */
		template<int32 Op = CMP_EQ>
		static FORCE_INLINE int32 cmp(Type v1, Type v2) { return _mm_movemask_ps(_mm_cmp_ps(v1, v2, Op)); }

		/**
		 * Shuffles elements of two vectors
		 * 
		 * @param [in] a,b,c,d permutation indices
		 * @param [in] v1,v2 vectors to shuffle
		 * @return new vector
		 * @{
		 */
		template<uint8 a, uint8 b, uint8 c, uint8 d>
		static FORCE_INLINE Type shuffle(Type v1, Type v2)
		{
			return _mm_shuffle_ps(v1, v2, _MM_SHUFFLE(d, c, b, a));
		}
		/// @}	

		/**
		 * Shuffles (permutes) elements of one vectors
		 * 
		 * @param [in] a,b,c,d permutation indices
		 * @param [in] v vector to shuffle
		 * @return new vector
		 */
		template<uint8 a, uint8 b, uint8 c, uint8 d>
		static FORCE_INLINE Type shuffle(Type v)
		{
			return _mm_permute_ps(v, _MM_SHUFFLE(d, c, b, a));
		}
		
		/// Unpack and interleave components from two vectors
		/// @{
		static FORCE_INLINE Type unpacklo(Type v1, Type v2)
		{
			return _mm_unpacklo_ps(v1, v2);
		}
		static FORCE_INLINE Type unpackhi(Type v1, Type v2)
		{
			return _mm_unpackhi_ps(v1, v2);
		}
		/// @}
	};

	/// Shuffle specializations
	/// @{
	template<>
	FORCE_INLINE Vector<float32, 4>::Type Vector<float32, 4>::shuffle<0, 1, 0, 1>(Type v1, Type v2)
	{
		return _mm_movelh_ps(v1, v2);
	}
	template<>
	FORCE_INLINE Vector<float32, 4>::Type Vector<float32, 4>::shuffle<2, 3, 2, 3>(Type v1, Type v2)
	{
		return _mm_movehl_ps(v2, v1);
	}
	/// @}

	/**
	 * Float specialization
	 */
	template<>
	struct Vector<float32, 8>
	{
	public:
		/// Intrinsics data type
		using Type = __m256;

		/// Constants
		/// @{
		static const Type zero;
		static const Type unit;
		static const Type neg;
		static const Type row;
		/// @}

	public:
		/// Load methods
		/// @{
		/**
		 * Creates a vector from scalar values
		 * 
		 * @param [in] a-h scalar operands
		 * @return new vector
		 */
		static CONSTEXPR FORCE_INLINE Type load(float32 a, float32 b, float32 c, float32 d, float32 e, float32 f, float32 g, float32 h)
		{
			return Type{a, b, c, d, e, f, g, h};
		}

		/**
		 * Creates a vector from a single scalar value
		 * 
		 * @param [in] s scalar operand
		 * @return new vector
		 */
		static CONSTEXPR FORCE_INLINE Type load(float32 s) { return Type{s, s, s, s, s, s, s, s}; }

		/**
		 * Creates a vector from a memory buffer
		 * 
		 * Memory is required to be 32-Bytes aligned
		 * 
		 * @param [in] buffer memory buffer
		 * @return new vector
		 */
		static FORCE_INLINE Type load(const float32 * buffer) { return _mm256_load_ps(buffer); }

		/**
		 * Same as @copydoc load, but no alignment required
		 */
		static FORCE_INLINE Type loadu(const float32 * buffer) { return _mm256_loadu_ps(buffer); }
		/// @}

		/**
		 * Arithmetic operations
		 * 
		 * @param [in] v1,v2 intrinics operands
		 * @return new vector
		 * @{
		 */
		static FORCE_INLINE Type add(Type v1, Type v2) { return _mm256_add_ps(v1, v2); }
		static FORCE_INLINE Type sub(Type v1, Type v2) { return _mm256_sub_ps(v1, v2); }
		static FORCE_INLINE Type mul(Type v1, Type v2) { return _mm256_mul_ps(v1, v2); }
		static FORCE_INLINE Type div(Type v1, Type v2) { return _mm256_div_ps(v1, v2); }

		/// Horizontal add
		/// @see https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=3306,5554,5559,3327,3328,6068,6068,6011,3826,3816,4169,569,2394,3826,4169,4164,6011,3820,4085,3816,4896,3898,2914,2914&text=_mm256_hadd_ps
		static FORCE_INLINE Type hadd(Type v1, Type v2) { return _mm256_hadd_ps(v1, v2); }
		/// @}

		/**
		 * Compare two vector intrinsics
		 * 
		 * @param [in] op comparison operation
		 * @param [in] v1,v2 intrinsics operands
		 * @return comparison result as bitmask
		 */
		template<int32 Op = CMP_EQ>
		static FORCE_INLINE int32 cmp(Type v1, Type v2) { return _mm256_movemask_ps(_mm256_cmp_ps(v1, v2, Op)); }

		/// Unpacks and interleaves vector components
		/// @see https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=3306,5554,5559,3327,3328,6068,6068&text=_mm256_unpacklo_ps
		static FORCE_INLINE Type unpacklo(Type v1, Type v2) { return _mm256_unpacklo_ps(v1, v2); }

		/// Unpacks and interleaves vector components
		/// @see https://software.intel.com/sites/landingpage/IntrinsicsGuide/#expand=3306,5554,5559,3327,3328,6068,6068,6011&text=_mm256_unpackhi_ps
		static FORCE_INLINE Type unpackhi(Type v1, Type v2) { return _mm256_unpackhi_ps(v1, v2); }

		/**
		 * Shuffles (permutes) elements of two vectors
		 * 
		 * @param [in] v vector to shuffle
		 * @param [in] a-h permutation source indices
		 * @return new vector
		 */
		static FORCE_INLINE Type shuffle(Type v, uint32 a, uint32 b, uint32 c, uint32 d, uint32 e, uint32 f, uint32 g, uint32 h)
		{
			return _mm256_permutevar8x32_ps(v, _mm256_set_epi32(h, g, f, e, d, c, b, a));
		}
	};
} // Simd

#endif