#ifndef SGL_VEC2_H
#define SGL_VEC2_H

#include "simd.h"

/**
 * @struct Vec2 vec2.h
 * @brief a 2-components templated vector
 */
template<typename T = float32>
struct Vec2 : public SIMD<T>
{
public:
	union
	{
		/**
		 * @brief SIMD data
		 */
		typename Vec2<T>::VT data;

		/**
		 * @brief C array representation
		 */
		T __vec[4];

		/**
		 * @brief Struct for single item access
		 */
		struct
		{
			T __gap[2];

			/**
			 * @brief Vector components
			 * @{
			 */
			T y, x;
			/** @} */
		};
	};

public:
	/**
	 * @brief Zero-constructor
	 */
	inline Vec2();

	/**
	 * @brief Copy-constructor
	 * 
	 * @param [in] v other vector
	 */
	inline Vec2(const Vec2<T> & v);

	/**
	 * @brief Data-constructor
	 * 
	 * @param [in] data simd-like data structure
	 */
	inline Vec2(const typename Vec2::VT data);

	/**
	 * @brief Vec-constructor
	 * 
	 * @param [in] vec plain array
	 */
	inline Vec2(const T * __vec);

	/**
	 * @brief Coordinates-constructor
	 * 
	 * @param [in] x,y coordinates values
	 */
	inline Vec2(const T x, const T y);

	/**
	 * @brief Scalar-constructor
	 * 
	 * @param [in] s scalar value
	 */
	inline Vec2(const T s);

	/**
	 * @brief Assignment operation
	 * 
	 * @param [in] v other vector
	 * 
	 * @return self
	 */
	inline Vec2<T> & operator=(const Vec2<T> & v);

	/**
	 * @brief Return read-only element of the simd vector
	 * 
	 * @param [in] i element index
	 * 
	 * @return read-only copy
	 */
	inline T & operator[](uint8 i);

	/**
	 * @brief Return squared size of vector
	 * 
	 * @return squared size
	 */
	inline float32 getSquaredSize() const;

	/**
	 * @brief Return size of vector
	 * 
	 * @return size (L^2-norm)
	 */
	inline float32 getSize() const;

	/**
	 * @brief Get normalized copy of vector
	 * 
	 * @return normalized vector (copy)
	 */
	inline Vec2<T> getNormal() const;

	/**
	 * @brief Normalize vector in place
	 * 
	 * @return self
	 */
	inline Vec2<T> & normalize();

	/**
	 * @brief Compare two vectors
	 * 
	 * @param [in] v other vector
	 * 
	 * @return Comparison result
	 * 
	 * @{
	 */
	inline bool operator==(const Vec2<T> & v) const;
	inline bool operator!=(const Vec2<T> & v) const;
	inline bool operator<(const Vec2<T> & v) const;
	inline bool operator<=(const Vec2<T> & v) const;
	inline bool operator>(const Vec2<T> & v) const;
	inline bool operator>=(const Vec2<T> & v) const;
	/** @} */

	/**
	 * @brief Invert vector components
	 * 
	 * @return inverted vectors
	 */
	inline Vec2<T> operator-() const;

	/**
	 * @brief Vector-vector component-wise operations
	 * 
	 * @param [in] v other vector
	 * 
	 * @return result of operation
	 * 
	 * @{
	 */
	inline Vec2<T> operator+(const Vec2<T> & v) const;
	inline Vec2<T> operator-(const Vec2<T> & v) const;
	inline Vec2<T> operator*(const Vec2<T> & v) const;
	inline Vec2<T> operator/(const Vec2<T> & v) const;
	/** @} */

	/**
	 * @brief Vector-vector compound component-wise operations
	 * 
	 * @param [in] v other vector
	 * 
	 * @return result of operation
	 * 
	 * @{
	 */
	inline Vec2<T> & operator+=(const Vec2<T> & v);
	inline Vec2<T> & operator-=(const Vec2<T> & v);
	inline Vec2<T> & operator*=(const Vec2<T> & v);
	inline Vec2<T> & operator/=(const Vec2<T> & v);
	/** @} */

	/**
	 * @brief Vector-scalar element-wise operations
	 * 
	 * @param [in] s scalar value
	 * 
	 * @return result of operation
	 * 
	 * @{
	 */
	inline Vec2<T> operator+(const T s) const;
	inline Vec2<T> operator-(const T s) const;
	inline Vec2<T> operator*(const T s) const;
	inline Vec2<T> operator/(const T s) const;
	/** @} */

	/**
	 * @brief Vector-scalar compound element-wise operations
	 * 
	 * @param [in] s scalar value
	 * 
	 * @return result of operation
	 * 
	 * @{
	 */
	inline Vec2<T> & operator+=(const T s);
	inline Vec2<T> & operator-=(const T s);
	inline Vec2<T> & operator*=(const T s);
	inline Vec2<T> & operator/=(const T s);
	/** @} */

	/**
	 * @brief Vector-vector dot(scalar) product
	 * 
	 * @param [in] v other vector
	 * 
	 * @return dot product
	 */
	inline T operator&(const Vec2<T> & v) const;

	/**
	 * @brief Vector-vector cross product
	 * 
	 * @param [in] v other vector
	 * 
	 * @return cross product
	 */
	inline float32 operator^(const Vec2<T> & v) const;

	/**
	 * @copydoc SIMD<T>::print()
	 */
	inline void print(FILE * stream = stdout) const;

	/**
	 * @brief Component type conversion operator
	 * 
	 * @result converted vector
	 */
	template<typename T2>
	inline operator Vec2<T2>() const;
};

/////////////////////////////////////////////////
// Constructors                                //
/////////////////////////////////////////////////

template<typename T>
Vec2<T>::Vec2() {}

template<typename T>
Vec2<T>::Vec2(const Vec2<T> & v) : data(v.data) {}

template<typename T>
Vec2<T>::Vec2(const typename Vec2<T>::VT data) : data(data) {}

template<typename T>
Vec2<T>::Vec2(const T * __vec) { memcpy(&this->__vec, __vec, 4 * sizeof(T)); }

template<typename T>
Vec2<T>::Vec2(const T x, const T y) : x(x), y(y) {}

template<typename T>
Vec2<T>::Vec2(const T s) : x(s), y(s) {}

template<typename T>
Vec2<T> & Vec2<T>::operator=(const Vec2<T> & v)
{
	data = v.data;
	return *this;
}

template<typename T>
T & Vec2<T>::operator[](uint8 i)
{
	// SIMD vector is reversed
	return __vec[3 - i];
}

template<typename T>
float32 Vec2<T>::getSquaredSize() const
{
	return x * x + y * y;
}

template<typename T>
float32 Vec2<T>::getSize() const
{
	return sqrtf(getSquaredSize());
}

template<typename T>
Vec2<T> Vec2<T>::getNormal() const
{
	const float32 size = getSize();
	return Vec2<T>(x / size, y / size);
}

template<typename T>
Vec2<T> & Vec2<T>::normalize()
{
	const float32 size = getSize();
	x /= size, y /= size;
	return *this;
}

///////////////////////////////
// Vector-vector comaparison //
///////////////////////////////

template<typename T>
bool Vec2<T>::operator==(const Vec2<T> & v) const
{
	/**
	 * @brief IEEE comparison should be
	 * faster than SIMD comparison
	 */
	return x == v.x && y == v.y;
}

template<typename T>
bool Vec2<T>::operator!=(const Vec2<T> & v) const
{
	return x != v.x || y != v.y;
}

//////////////////////////////
// Vector-vector operations //
//////////////////////////////

template<typename T>
Vec2<T> Vec2<T>::operator+(const Vec2<T> & v) const
{
	return Vec2<T>(x + v.x, y + v.y);
}

template<typename T>
Vec2<T> Vec2<T>::operator-(const Vec2<T> & v) const
{
	return Vec2<T>(x - v.x, y - v.y);
}

template<typename T>
Vec2<T> Vec2<T>::operator*(const Vec2<T> & v) const
{
	return Vec2<T>(x * v.x, y * v.y);
}

template<typename T>
Vec2<T> Vec2<T>::operator/(const Vec2<T> & v) const
{
	return Vec2<T>(x / v.x, y / v.y);
}

//////////////////////////////////////
// Vector-vector compund operations //
//////////////////////////////////////

template<typename T>
Vec2<T> & Vec2<T>::operator+=(const Vec2<T> & v)
{
	x += v.x, y += v.y;
	return *this;
}

template<typename T>
Vec2<T> & Vec2<T>::operator-=(const Vec2<T> & v)
{
	x -= v.x, y -= v.y;
	return *this;
}

template<typename T>
Vec2<T> & Vec2<T>::operator*=(const Vec2<T> & v)
{
	x *= v.x, y *= v.y;
	return *this;
}

template<typename T>
Vec2<T> & Vec2<T>::operator/=(const Vec2<T> & v)
{
	x /= v.x, y /= v.y;
	return *this;
}

//////////////////////////////
// Vector-scalar operations //
//////////////////////////////

template<typename T>
Vec2<T> Vec2<T>::operator+(const T s) const
{
	return Vec2<T>(x + s, y + s);
}

template<typename T>
Vec2<T> Vec2<T>::operator-(const T s) const
{
	return Vec2<T>(x - s, y - s);
}

template<typename T>
Vec2<T> Vec2<T>::operator*(const T s) const
{
	return Vec2<T>(x * s, y * s);
}

template<typename T>
Vec2<T> Vec2<T>::operator/(const T s) const
{
	return Vec2<T>(x / s, y / s);
}

/**
 * @brief Symmetric version of vector-scalar operations
 * 
 * @param [in]	v	vector
 * @param [in]	s	scalar
 * 
 * @return result of operation
 * 
 * @{
 */
template<typename T>
inline Vec2<T> operator+(const T s, const Vec2<T> & v)
{
	// Commutative
	return v + s;
}

template<typename T>
inline Vec2<T> operator-(const T s, const Vec2<T> & v)
{
	return Vec2<T>(s) - v;
}

template<typename T>
inline Vec2<T> operator*(const T s, const Vec2<T> & v)
{
	// Commutative
	return v * s;
}

template<typename T>
inline Vec2<T> operator/(const T s, const Vec2<T> & v)
{
	return Vec2<T>(s) / v;
}
/** @} */

///////////////////////////////////////
// Vector-scalar compound operations //
///////////////////////////////////////

template<typename T>
Vec2<T> & Vec2<T>::operator+=(const T s)
{
	x += s, y += s;
	return *this;
}

template<typename T>
Vec2<T> & Vec2<T>::operator-=(const T s)
{
	x -= s, y -= s;
	return *this;
}

template<typename T>
Vec2<T> & Vec2<T>::operator*=(const T s)
{
	x *= s, y *= s;
	return *this;
}

template<typename T>
Vec2<T> & Vec2<T>::operator/=(const T s)
{
	x /= s, y /= s;
	return *this;
}

template<typename T>
T Vec2<T>::operator&(const Vec2<T> & v) const
{
	return x * v.x + y * v.y;
}

template<typename T>
float32 Vec2<T>::operator^(const Vec2<T> & v) const
{
	return x * v.y - y * v.x;
}

template<typename T1>
template<typename T2>
Vec2<T1>::operator Vec2<T2>() const
{
	return Vec2<T2>(static_cast<T2>(x), static_cast<T2>(y));
}

/////////////////////////////////////////////////
// Float 32-bit specialization                 //
/////////////////////////////////////////////////

template<>
Vec2<float32>::Vec2(const float32 s) : data(_mm_set1_ps(s)) {}

template<>
Vec2<float32> Vec2<float32>::getNormal() const
{
	const __m128 size = _mm_sqrt_ps(_mm_set1_ps(x * x + y * y));
	return Vec2<float32>(_mm_div_ps(data, size));
}

template<>
Vec2<float32> & Vec2<float32>::normalize()
{
	const __m128 size = _mm_sqrt_ps(_mm_set1_ps(x * x + y * y));
	data = _mm_div_ps(data, size);
	return *this;
}

template<>
void Vec2<float32>::print(FILE * stream) const
{
	printf("v2(%.3f, %.3f)\n", x, y);
}

/////////////////////////////////////////////////
// Int-32 specialization                       //
/////////////////////////////////////////////////

template<>
void Vec2<int32>::print(FILE * stream) const
{
	printf("v2(%d, %d)\n", x, y);
}

/////////////////////////////////////////////////
// Type definitions                            //
/////////////////////////////////////////////////

/**
 * @brief Type definitions for common vector types
 * @{
 */
typedef Vec2<float32>	vec2;
typedef Vec2<int32>		point2;
/** @} */

#endif