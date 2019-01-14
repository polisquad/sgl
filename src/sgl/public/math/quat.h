#pragma once

#include "core_types.h"
#include "vec4.h"
#include "templates/is_void.h"
#include "templates/simd.h"

/**
 * A 4-components vector that used to describe a rotation
 */
template<typename T, bool = /* !IsVoid<typename Simd::Vector<T, 4>::Type>::value */false>
struct Quat : public Vec4<T, false>
{
public:
	/// Default constructor
	CONSTEXPR FORCE_INLINE Quat() : Vec4<T, false>(T(1), T(0), T(0), T(0)) {};

	/// Convert @ref Vec4
	CONSTEXPR FORCE_INLINE Quat(const Vec4<T, false> & v) : Vec4<T, false>(v) {};

	/// Inherit all @ref Vec4 constructors
	using Vec4<T, false>::Vec4;

	/// Angle and axis constructor
	template<bool bHVI>
	CONSTEXPR FORCE_INLINE Quat(T angle, const Vec3<T, bHVI> & axis) :
		Vec4<T, bHVI>(axis.getNormal() * PlatformMath::sin(angle / 2.f), PlatformMath::cos(angle / 2.f)) {}

	/// Override normalization methods to return quaternion
	/// @{
	FORCE_INLINE Quat<T, false> getNormal() const
	{
		const T size = this->getSize();
		return Quat<T, false>(this->x / size, this->y / size, this->x / size, this->w / size);
	}
	/// In-place
	FORCE_INLINE Quat<T, false> & normalize()
	{
		const T size = this->getSize();
		this->x /= size, this->y /= size,
		this->z /= size, this->w /= size;
		return *this;
	}
	/// @}

	/// Get inverse quaternion
	FORCE_INLINE Quat<T, false> operator!() const
	{
		return Quat<T, false>(-this->x, -this->y, -this->z, -this->w);
	}

	/**
	 * Quat-quat multiplication
	 * 
	 * Like matrix multiplication, it's not commutative
	 * 
	 * @param [in] q quat operand
	 * @return combined rotation quaternion
	 */
	CONSTEXPR FORCE_INLINE Quat<T, false> operator*(const Quat<T, false> & q) const
	{
		/// @ref https://en.wikipedia.org/wiki/Quaternion#Hamilton_product
		/// Wikipedia uses (angle, axis<i, j, k>) notation
		/// Here we use (axis<x, y, z>, angle) notation,
		/// thus everything is inverted
		return Quat<T, false>(
			this->w * q.x + this->x * q.w + this->y * q.z - this->z * q.y,
			this->w * q.y - this->x * q.z + this->y * q.w + this->z * q.x,
			this->w * q.z + this->x * q.y - this->y * q.x + this->z * q.w,
			this->w * q.w - this->x * q.x - this->y * q.y - this->z * q.z
		);
		/// @todo should we normalize?
	}
	
	/**
	 * Quat-vector multiplication
	 * 
	 * @param [in] v vector operand
	 * @return rotated vector
	 * @{
	 */
	template<bool bHVI>
	CONSTEXPR FORCE_INLINE Vec3<T, bHVI> operator*(const Vec3<T, bHVI> & v) const
	{
		/** @see http://people.csail.mit.edu/bkph/articles/Quaternions.pdf */
		const Vec3<T, bHVI> q(this->x, this->y, this->z);
		const Vec3<T, bHVI> t = T(2) * (q ^ v);
		return v + (this->w * t) + (q ^ t);
	}
	template<bool bHVI>
	FORCE_INLINE Vec4<T, bHVI> operator*(const Vec4<T, bHVI> & v) const { return Vec4<T, bHVI>(operator*(Vec3<T, bHVI>(v)), v.w); }
	/// @}
};

#if PLATFORM_ENABLE_SIMD
	#include "quat_simd.h"
#endif