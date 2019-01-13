#pragma once

#include "core_types.h"
#include "hal/platform_math.h"
#include "math_fwd.h"
#include "templates/is_void.h"
#include "templates/simd.h"

/**
 * A 3-component vector
 * 
 * If possible uses vector intrinsics to
 * boost performance
 */
template<typename T, bool = !IsVoid<typename Simd::Vector<T, 4>::Type>::value>
struct Vec3
{
public:
	union
	{
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
	FORCE_INLINE Vec3() : x(0), y(0), z(0) {}

	/// Components constructor
	FORCE_INLINE Vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

	/// Scalar constructor
	FORCE_INLINE Vec3(T s) : x(s), y(s), z(s) {}

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
		const T size = PlatformMath::sqrt(x * x + y * y + z * z);
		return Vec3<T>(x / size, y / size, z / size);
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
		return x == v.x & y == v.y & z == v.z;
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
		return Vec3<T>(x + v.x, y + v.y, z + v.z);
	}
	FORCE_INLINE Vec3<T> operator-(const Vec3<T> & v) const
	{
		return Vec3<T>(x - v.x, y - v.y, z - v.z);
	}
	FORCE_INLINE Vec3<T> operator*(const Vec3<T> & v) const
	{
		return Vec3<T>(x * v.x, y * v.y, z * v.z);
	}
	FORCE_INLINE Vec3<T> operator/(const Vec3<T> & v) const
	{
		return Vec3<T>(x / v.x, y / v.y, z / v.z);
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
		return Vec3<T>(x + s, y + s, z + s);
	}
	FORCE_INLINE Vec3<T> operator-(T s) const
	{
		return Vec3<T>(x - s, y - s, z - s);
	}
	FORCE_INLINE Vec3<T> operator*(T s) const
	{
		return Vec3<T>(x * s, y * s, z * s);
	}
	FORCE_INLINE Vec3<T> operator/(T s) const
	{
		return Vec3<T>(x / s, y / s, z / s);
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

#if PLATFORM_ENABLE_SIMD
	#include "vec3_simd.h"
#endif

template<>
FORCE_INLINE bool Vec3<float32, false>::isNearlyZero() const
{
	return PlatformMath::abs(x) <= FLT_EPSILON
		& PlatformMath::abs(y) <= FLT_EPSILON
		& PlatformMath::abs(z) <= FLT_EPSILON;
}

template<>
FORCE_INLINE bool Vec3<float32, false>::isEqual(const Vec3<float32> & v) const
{
	return PlatformMath::abs(x - v.x) <= FLT_EPSILON
		& PlatformMath::abs(y - v.y) <= FLT_EPSILON
		& PlatformMath::abs(z - v.z) <= FLT_EPSILON;
}

template<>
FORCE_INLINE void Vec3<float32, false>::print(FILE * out) const
{
	fprintf(out, "v3f(%.3f, %.3f, %.3f)\n", x, y, z);
}

template<>
FORCE_INLINE void Vec3<int32, false>::print(FILE * out) const
{
	fprintf(out, "v3i(%d, %d, %d)\n", x, y, z);
}

template<>
FORCE_INLINE void Vec3<uint32, false>::print(FILE * out) const
{
	fprintf(out, "v3u(%u, %u, %u)\n", x, y, z);
}

/**
 * Scalar-vector operations
 * 
 * @param [in] s,v scalar and vector operands
 * @return new vector
 * @{
 */
template<typename T>
FORCE_INLINE Vec3<T> operator+(T s, const Vec3<T> & v)
{
	return v + s;
}
template<typename T>
FORCE_INLINE Vec3<T> operator-(T s, const Vec3<T> & v)
{
	return Vec3<T>(s) - v;
}
template<typename T>
FORCE_INLINE Vec3<T> operator*(T s, const Vec3<T> & v)
{
	return v * s;
}
template<typename T>
FORCE_INLINE Vec3<T> operator/(T s, const Vec3<T> & v)
{
	return Vec3<T>(s) / v;
}
/// @}