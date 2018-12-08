#ifndef SGL_SIGNED_H
#define SGL_SIGNED_H

#include "core_types.h"
#include "is_integral.h"

/**
 * @struct Unsigned templates/unsigned.h
 * @brief Return unsigned version of integer
 */
template<typename UnsignedInt>
struct Signed
{
	static_assert(IsIntegralV(UnsignedInt), "UnsignedInt must be an integer type");
	typedef UnsignedInt Type;
};

#define SignedT(T) typename Signed<T>::Type

template<> struct Signed<uint8>		{ typedef int8 Type; };
template<> struct Signed<uint16>	{ typedef int16 Type; };
template<> struct Signed<uint32>	{ typedef int32 Type; };
template<> struct Signed<uint64>	{ typedef int64 Type; };

#endif