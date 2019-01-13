#pragma once

#include "vec3.h"

/**
 * Vector intrinsics specialization
 */
template<typename T>
struct GCC_PACK(16) Vec3<T, true>
{
public:
	/// Vector operations class
	using VecOps = Simd::Vector<T, 4>;

	/// Intrinsic type
	using VecT = typename VecOps::Type;

	union
	{
		/// Intrinsic vector
		VecT data;

		/// Data buffer
		T buffer[3];

		struct
		{
			/// Vector components
			/// @{
			T x, y, z;
			/// @}
		};
	};

public:
	/// Default constructor, zero-initialize
	FORCE_INLINE Vec3() : data(VecOps::load(T(0))) {}

	/// Intrinsic constructor
	FORCE_INLINE Vec3(VecT _data) : data(_data) {}

	/// Components constructor
	FORCE_INLINE Vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

	/// Scalar constructor
	FORCE_INLINE Vec3(T s) : data(VecOps::load(s)) {}

	/// Buffer-access operator
	/// @{
	FORCE_INLINE const T & operator[](uint8 i) const { return buffer[i]; }
	FORCE_INLINE T & operator[](uint8 i) { return buffer[i]; }
	/// @}

	/// Returns squared size of the vector
	FORCE_INLINE T getSquaredSize() const
	{
		return x * x + y * y + z * z;
	}

	/// Returns size of the vector
	FORCE_INLINE T getSize() const
	{
		return PlatformMath::sqrt(x * x + y * y + z * z);
	}

	/// Returns normal vector
	FORCE_INLINE Vec3<T> getNormal() const
	{
		return Vec3<T>(VecOps::div(data, VecOps::load(PlatformMath::sqrt(x * x + y * y + z * z))));
	}

	/// Normalizes in place
	FORCE_INLINE Vec3<T> & normalize()
	{
		const T size = PlatformMath::sqrt(x * x + y * y + z * z);
		x /= size, y /= size, z /= size;
		return *this;
	}

	//////////////////////////////////////////////////
	// Comparison operations
	//////////////////////////////////////////////////
	
	/// Returns true if components are almost zero
	/// This functions really has only meaning for floats
	FORCE_INLINE bool isNearlyZero() const;

	/// Returns true if vectors are almost equal
	/// @see isNearlyZero()
	FORCE_INLINE bool isEqual(const Vec3<T> & v) const;

	/**
	 * Vector-vector comparison operators
	 * 
	 * @param [in] v vector operand
	 * @returns result of comparison
	 * @{
	 */
	/// Use @ref isNearlyZero() for floating points
	FORCE_INLINE bool operator==(const Vec3<T> & v) const
	{
		return x < v.x == y < v.y == z < v.z;
	}
	FORCE_INLINE bool operator!=(const Vec3<T> & v) const
	{
		return x != v.x | y != v.y | z != v.z;
	}
	/// Strictly less, all components must be less
	FORCE_INLINE bool operator<(const Vec3<T> & v) const
	{
		return x < v.x & y < v.y & z < v.z;
	}
	/// Strictly greater, all components must be greater
	FORCE_INLINE bool operator>(const Vec3<T> & v) const
	{
		return x > v.x & y > v.y & z > v.z;
	}
	/// Weakly less
	FORCE_INLINE bool operator<=(const Vec3<T> & v) const
	{
		return x <= v.x & y <= v.y & z <= v.z;
	}
	/// Weakly greater
	FORCE_INLINE bool operator>=(const Vec3<T> & v) const
	{
		return x >= v.x & y >= v.y & z >= v.z;
	}
	/// @}

	//////////////////////////////////////////////////
	// Arithmetic operations
	//////////////////////////////////////////////////
	
	/// Invert vector direction
	FORCE_INLINE Vec3<T> & operator-()
	{
		x = -x, y = -y, z = -z;
		return *this;
	}

	/**
	 * Vector-vector compound assignment operators
	 * 
	 * @param [in] v second operand
	 * @return self
	 * @{
	 */
	FORCE_INLINE Vec3<T> & operator+=(const Vec3<T> & v)
	{
		x += v.x, y += v.y, z += v.z;
		return *this;
	}
	FORCE_INLINE Vec3<T> & operator-=(const Vec3<T> & v)
	{
		x -= v.x, y -= v.y, z -= v.z;
		return *this;
	}
	FORCE_INLINE Vec3<T> & operator*=(const Vec3<T> & v)
	{
		x *= v.x, y *= v.y, z *= v.z;
		return *this;
	}
	FORCE_INLINE Vec3<T> & operator/=(const Vec3<T> & v)
	{
		x /= v.x, y /= v.y, z /= v.z;
		return *this;
	}
	/// @}

	/**
	 * Vector-scalar compounf assignment
	 * 
	 * @param [in] s scalar value
	 * @return self
	 * @{
	 */
	FORCE_INLINE Vec3<T> & operator+=(T s)
	{
		x += s, y += s, z += s;
		return *this;
	}
	FORCE_INLINE Vec3<T> & operator-=(T s)
	{
		x -= s, y -= s, z -= s;
		return *this;
	}
	FORCE_INLINE Vec3<T> & operator*=(T s)
	{
		x *= s, y *= s, z *= s;
		return *this;
	}
	FORCE_INLINE Vec3<T> & operator/=(T s)
	{
		x /= s, y /= s, z /= s;
		return *this;
	}
	/// @}

	/**
	 * Vector-vector element-wise operations
	 * 
	 * @param [in] v vector operand
	 * @return new vector
	 * @{
	 */
	FORCE_INLINE Vec3<T> operator+(const Vec3<T> & v) const
	{
		return Vec3<T>(VecOps::add(data, v.data));
	}
	FORCE_INLINE Vec3<T> operator-(const Vec3<T> & v) const
	{
		return Vec3<T>(VecOps::sub(data, v.data));
	}
	FORCE_INLINE Vec3<T> operator*(const Vec3<T> & v) const
	{
		return Vec3<T>(VecOps::mul(data, v.data));
	}
	FORCE_INLINE Vec3<T> operator/(const Vec3<T> & v) const
	{
		return Vec3<T>(VecOps::div(data, v.data));
	}
	/// @}

	/**
	 * Vector-scalar operations
	 * 
	 * @param [in] s scalar operand
	 * @return new vector
	 * @{
	 */
	FORCE_INLINE Vec3<T> operator+(T s) const
	{
		return Vec3<T>(VecOps::add(data, VecOps::load(s)));
	}
	FORCE_INLINE Vec3<T> operator-(T s) const
	{
		return Vec3<T>(VecOps::sub(data, VecOps::load(s)));
	}
	FORCE_INLINE Vec3<T> operator*(T s) const
	{
		return Vec3<T>(VecOps::mul(data, VecOps::load(s)));
	}
	FORCE_INLINE Vec3<T> operator/(T s) const
	{
		return Vec3<T>(VecOps::div(data, VecOps::load(s)));
	}
	/// @}
	
	/**
	 * Vector dot (scalar) product
	 * 
	 * @param [in] v vector operand
	 * @return dot product as T
	 */
	FORCE_INLINE Vec3<T> operator&(const Vec3<T> & v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}
	
	/**
	 * Vector cross product
	 * 
	 * @param [in] v vector operand
	 * @return vector orthogonal to the (v1 x v2) plane
	 */
	FORCE_INLINE Vec3<T> operator^(const Vec3<T> & v) const
	{
		return Vec3<T>(y * v.z - z * v.y, x * v.z - z * v.x, x * v.y - y * v.x);
	}

	/// Convert to another underlying type
	template<typename U>
	FORCE_INLINE operator Vec3<U>() const
	{
		return Vec3<U>(U(x), U(y), U(z));
	}

	/// Print vector to stdout or to specified file
	void print(FILE * out = stdout) const;
};

template<>
FORCE_INLINE bool Vec3<float32, true>::isNearlyZero() const
{
	return VecOps::cmp<Simd::CMP_GE>(VecOps::bor(data, VecOps::load(-0.f)), VecOps::load(-FLT_EPSILON)) >= 0xe;
}

template<>
FORCE_INLINE bool Vec3<float32, true>::isEqual(const Vec3<float32> & v) const
{
	return VecOps::cmp<Simd::CMP_GE>(VecOps::bor(VecOps::sub(data, v.data), VecOps::load(-0.f)), VecOps::load(-FLT_EPSILON)) >= 0xe;
}

template<>
FORCE_INLINE void Vec3<float32, true>::print(FILE * out) const
{
	fprintf(out, "v3f(%.3f, %.3f, %.3f)\n", x, y, z);
}