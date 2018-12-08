#ifndef SGL_UNIX_PLATFORM_ATOMICS_H
#define SGL_UNIX_PLATFORM_ATOMICS_H

#include "generic/generic_platform_atomics.h"
#include "templates/enable_if.h"
#include "templates/is_integral.h"

/**
 * @struct UnixPlatformAtomics unix/unix_platform_atomics
 * @brief Unix implementation of atomic operations
 * 
 * We use GCC atomic builtins on unix, easy
 */
typedef struct UnixPlatformAtomics
{
	template<typename Int>
	static FORCE_INLINE EnableIfT<IsIntegralV(Int), Int> increment(volatile Int * val) { return __sync_fetch_and_add(val, 1); }

	template<typename Int>
	static FORCE_INLINE EnableIfT<IsIntegralV(Int), Int> decrement(volatile Int * val) { return __sync_fetch_and_sub(val, 1); }
	
	template<typename Int, typename T>
	static FORCE_INLINE EnableIfT<IsIntegralV(Int) & IsIntegralV(T), Int> add(volatile Int * val, T offset) { return __sync_fetch_and_add(val, offset); }

	template<typename Int, typename T>
	static FORCE_INLINE EnableIfT<IsIntegralV(Int) & IsIntegralV(T), Int> exchange(volatile Int * val, T exchange) { return __sync_lock_test_and_set(val, exchange); }
	
	template<typename Int>
	static FORCE_INLINE EnableIfT<IsIntegralV(Int), Int> read(volatile const Int * src)
	{
		Int out;
		__atomic_load((volatile Int*)(src), &out, __ATOMIC_SEQ_CST);
		return out;
	}

	template<typename Int>
	static FORCE_INLINE EnableIfT<IsIntegralV(Int), Int> readRelaxed(volatile const Int * src)
	{
		Int out;
		__atomic_load((volatile Int*)(src), &out, __ATOMIC_RELAXED);
		return out;
	}

	template<typename Int, typename T = Int>
	static FORCE_INLINE void store(volatile EnableIfT<IsIntegralV(Int) & IsIntegralV(T), Int> * src, Int val) { __atomic_store((volatile Int*)src, &val, __ATOMIC_SEQ_CST); }

	template<typename Int, typename T = Int>
	static FORCE_INLINE void storeRelaxed(volatile EnableIfT<IsIntegralV(Int) & IsIntegralV(T), Int> * src, Int val) { __atomic_store((volatile Int*)src, &val, __ATOMIC_RELAXED); }
} PlatformAtomics;

#endif