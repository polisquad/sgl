#ifndef SGL_VEC4_H
#define SGL_VEC4_H

#include "vec3.h"

/**
 * @struct Vec4 vec4.h
 * @brief A 4-components templated vector
 */
template<typename T = float32>
struct Vec4 : public SIMD<T>
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
			/**
			 * @brief Vector components
			 * @{
			 */
			T w, z, y, x;
			/** @} */
		};
	};

public:
	/**
	 * @brief Zero-constructor
	 */
	inline Vec4();

	/**
	 * @brief Copy-constructor
	 * 
	 * @param [in] v other vector
	 */
	inline Vec4(const Vec4<T> & v);

	/**
	 * @brief Data-constructor
	 * 
	 * @param [in] data simd-like data structure
	 */
	inline Vec4(const typename Vec4::VT data);

	/**
	 * @brief Vec-constructor
	 * 
	 * @param [in] vec plain C array
	 */
	inline Vec4(const T * __vec);

	/**
	 * @brief Coordinates-constructor
	 * 
	 * @param [in] x,y,z,w coordinates values
	 */
	inline Vec4(const T x, const T y, const T z, const T w);

	/**
	 * @brief Scalar-constructor
	 * 
	 * @param [in] s scalar value
	 */
	inline Vec4(const T s);

	/**
	 * @brief Vec2-constructor
	 * 
	 * @param [in]	v2	Vec2 vector
	 * @param [in]	z,w	missing components
	 */
	inline Vec4(const Vec2<T> & v2, const T z = T(), const T w = T());

	/**
	 * @brief Vec3-constructor
	 * 
	 * @param [in]	v3	Vec3 vector
	 * @param [in]	w	missing component
	 */
	inline Vec4(const Vec3<T> & v3, const T w = T());

	/**
	 * @brief Assignment operation
	 * 
	 * @param [in] v other vector
	 * 
	 * @return self
	 */
	inline Vec4<T> & operator=(const Vec4<T> & v);

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
	inline Vec4<T> getNormal() const;

	/**
	 * @brief Normalize vector in place
	 * 
	 * @return self
	 */
	inline Vec4<T> & normalize();

	/**
	 * @brief Compare two vectors
	 * 
	 * @param [in] v other vector
	 * 
	 * @return Comparison result
	 * @{
	 */
	inline bool operator==(const Vec4<T> & v) const;
	inline bool operator!=(const Vec4<T> & v) const;
	inline bool operator<(const Vec4<T> & v) const;
	inline bool operator<=(const Vec4<T> & v) const;
	inline bool operator>(const Vec4<T> & v) const;
	inline bool operator>=(const Vec4<T> & v) const;
	/** @} */

	/**
	 * @brief Invert vector components
	 * 
	 * @return inverted vectors
	 */
	inline Vec4<T> operator-() const;

	/**
	 * @brief Vector-vector component-wise operations
	 * 
	 * @param [in] v other vector
	 * 
	 * @return result of operation
	 * @{
	 */
	inline Vec4<T> operator+(const Vec4<T> & v) const;
	inline Vec4<T> operator-(const Vec4<T> & v) const;
	inline Vec4<T> operator*(const Vec4<T> & v) const;
	inline Vec4<T> operator/(const Vec4<T> & v) const;
	/** @} */

	/**
	 * @brief Vector-vector compound component-wise operations
	 * 
	 * @param [in] v other vector
	 * 
	 * @return result of operation
	 * @{
	 */
	inline Vec4<T> & operator+=(const Vec4<T> & v);
	inline Vec4<T> & operator-=(const Vec4<T> & v);
	inline Vec4<T> & operator*=(const Vec4<T> & v);
	inline Vec4<T> & operator/=(const Vec4<T> & v);
	/** @} */

	/**
	 * @brief Vector-scalar element-wise operations
	 * 
	 * @param [in] s scalar value
	 * 
	 * @return result of operation
	 * @{
	 */
	inline Vec4<T> operator+(const T s) const;
	inline Vec4<T> operator-(const T s) const;
	inline Vec4<T> operator*(const T s) const;
	inline Vec4<T> operator/(const T s) const;
	/** @} */

	/**
	 * @brief Vector-scalar compound element-wise operations
	 * 
	 * @param [in] s scalar value
	 * 
	 * @return result of operation
	 * @{
	 */
	inline Vec4<T> & operator+=(const T s);
	inline Vec4<T> & operator-=(const T s);
	inline Vec4<T> & operator*=(const T s);
	inline Vec4<T> & operator/=(const T s);
	/** @} */

	/**
	 * @brief Vector-vector dot(scalar) product
	 * 
	 * @param [in] v other vector
	 * 
	 * @return dot product
	 */
	inline T operator&(const Vec4<T> & v) const;

	/**
	 * @brief Linear interpolate with other vector
	 * 
	 * @param [in]	v		other vector
	 * @param [in]	alpha	interpolation step
	 * 
	 * @return self
	 */
	inline Vec4<T> & lerp(const Vec4<T> & v, float32 alpha);

	/**
	 * @brief Print vector to stream
	 * 
	 * @param [in] stream output stream
	 */
	inline void print(FILE * stream = stdout) const;

	/**
	 * @brief Component type conversion operator
	 * 
	 * @return converted vector
	 */
	template<typename T2>
	inline operator Vec4<T2>() const;

	/**
	 * @brief Vec3 and Vec2 conversions
	 * 
	 * @return converted vector
	 * @{
	 */
	inline explicit operator Vec2<T>() const;
	inline explicit operator Vec3<T>() const;
	/** @} */
};

//////////////////
// Constructors //
//////////////////

template<typename T>
Vec4<T>::Vec4() {}

template<typename T>
Vec4<T>::Vec4(const Vec4<T> & v) : data(v.data) {}

template<typename T>
Vec4<T>::Vec4(const typename Vec4<T>::VT data) : data(data) {}

template<typename T>
Vec4<T>::Vec4(const T * __vec) { memcpy(this->__vec, __vec, 4 * sizeof(T)); }

template<typename T>
Vec4<T>::Vec4(const T s) : x(s), y(s), z(s), w(s) {}

template<typename T>
Vec4<T>::Vec4(const Vec2<T> & v2, const T z, const T w) : x(v2.x), y(v2.y), z(z), w(w) {}

template<typename T>
Vec4<T>::Vec4(const Vec3<T> & v3, const T w) : x(v3.x), y(v3.y), z(v3.z), w(w) {}

template<typename T>
Vec4<T> & Vec4<T>::operator=(const Vec4<T> & v)
{
	data = v.data;
	return *this;
}

template<typename T>
T & Vec4<T>::operator[](uint8 i)
{
	// SIMD vector is reversed
	return __vec[3 - i];
}

template<typename T>
float32 Vec4<T>::getSquaredSize() const
{
	return x * x + y * y + z * z + w * w;
}

template<typename T>
float32 Vec4<T>::getSize() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

template<typename T>
Vec4<T> Vec4<T>::getNormal() const
{
	const float32 size = getSize();
	return size > 0.f ? Vec4<T>(x / size, y / size, z / size, w / size) : *Vec4<T>();
}

template<typename T>
Vec4<T> & Vec4<T>::normalize()
{
	const float32 size = getSize();
	x /= size, y /= size, z /= size, w /= size;
	return *this;
}

///////////////////////////////
// Vector-vector comaparison //
///////////////////////////////

template<typename T>
bool Vec4<T>::operator==(const Vec4<T> & v) const
{
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

template<typename T>
bool Vec4<T>::operator!=(const Vec4<T> & v) const
{
	return x != v.x || y != v.y || z != v.y || w != v.w;
}

template<typename T>
bool Vec4<T>::operator<(const Vec4<T> & v) const
{
	return x < v.x & y < v.y & z < v.z & w < v.w;
}

template<typename T>
bool Vec4<T>::operator<=(const Vec4<T> & v) const
{
	return x <= v.x & y <= v.y & z <= v.z & w <= v.w;
}

template<typename T>
bool Vec4<T>::operator>(const Vec4<T> & v) const
{
	return x > v.x & y > v.y & z > v.z & w > v.w;
}

template<typename T>
bool Vec4<T>::operator>=(const Vec4<T> & v) const
{
	return x >= v.x & y >= v.y & z >= v.z & w >= v.w;
}

//////////////////////////////
// Vector-vector operations //
//////////////////////////////

template<typename T>
Vec4<T> Vec4<T>::operator+(const Vec4<T> & v) const
{
	return Vec4<T>(x + v.x, y + v.y, z + v.z, w + v.w);
}

template<typename T>
Vec4<T> Vec4<T>::operator-(const Vec4<T> & v) const
{
	return Vec4<T>(x - v.x, y - v.y, z - v.z, w - v.w);
}

template<typename T>
Vec4<T> Vec4<T>::operator*(const Vec4<T> & v) const
{
	return Vec4<T>(x * v.x, y * v.y, z * v.z, w * v.w);
}

template<typename T>
Vec4<T> Vec4<T>::operator/(const Vec4<T> & v) const
{
	return Vec4<T>(x / v.x, y / v.y, z / v.z, w / v.w);
}

//////////////////////////////////////
// Vector-vector compund operations //
//////////////////////////////////////

template<typename T>
Vec4<T> & Vec4<T>::operator+=(const Vec4<T> & v)
{
	x += v.x, y += v.y, z += v.z, w += v.w;
	return *this;
}

template<typename T>
Vec4<T> & Vec4<T>::operator-=(const Vec4<T> & v)
{
	x -= v.x, y -= v.y, z -= v.z, w -= v.w;
	return *this;
}

template<typename T>
Vec4<T> & Vec4<T>::operator*=(const Vec4<T> & v)
{
	x *= v.x, y *= v.y, z *= v.z, w *= v.w;
	return *this;
}

template<typename T>
Vec4<T> & Vec4<T>::operator/=(const Vec4<T> & v)
{
	x /= v.x, y /= v.y, z /= v.z, w /= v.w;
	return *this;
}

//////////////////////////////
// Vector-scalar operations //
//////////////////////////////

template<typename T>
Vec4<T> Vec4<T>::operator+(const T s) const
{
	return Vec4<T>(x + s, y + s, z + s, w + s);
}

template<typename T>
Vec4<T> Vec4<T>::operator-(const T s) const
{
	return Vec4<T>(x - s, y - s, z - s, w - s);
}

template<typename T>
Vec4<T> Vec4<T>::operator*(const T s) const
{
	return Vec4<T>(x * s, y * s, z * s, w * s);
}

template<typename T>
Vec4<T> Vec4<T>::operator/(const T s) const
{
	return Vec4<T>(x / s, y / s, z / s, w / s);
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
inline Vec4<T> operator+(const T s, const Vec4<T> & v)
{
	// Commutative
	return v + s;
}

template<typename T>
inline Vec4<T> operator-(const T s, const Vec4<T> & v)
{
	return Vec4<T>(s) - v;
}

template<typename T>
inline Vec4<T> operator*(const T s, const Vec4<T> & v)
{
	// Commutative
	return v * s;
}

template<typename T>
inline Vec4<T> operator/(const T s, const Vec4<T> & v)
{
	return Vec4<T>(s) / v;
}
/** @} */

///////////////////////////////////////
// Vector-scalar compound operations //
///////////////////////////////////////

template<typename T>
Vec4<T> & Vec4<T>::operator+=(const T s)
{
	x += s, y += s, z += s, w += s;
	return *this;
}

template<typename T>
Vec4<T> & Vec4<T>::operator-=(const T s)
{
	x -= s, y -= s, z -= s, w -= s;
	return *this;
}

template<typename T>
Vec4<T> & Vec4<T>::operator*=(const T s)
{
	x *= s, y *= s, z *= s, w *= s;
	return *this;
}

template<typename T>
Vec4<T> & Vec4<T>::operator/=(const T s)
{
	x /= s, y /= s, z /= s, w /= s;
	return *this;
}

template<typename T>
T Vec4<T>::operator&(const Vec4<T> & v) const
{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

template<typename T>
Vec4<T> & Vec4<T>::lerp(const Vec4<T> & v, float32 alpha)
{
	*this += alpha * (v - *this);
	return *this;
}

template<typename T1>
template<typename T2>
Vec4<T1>::operator Vec4<T2>() const
{
	return Vec4<T2>(static_cast<T2>(x), static_cast<T2>(y), static_cast<T2>(z), static_cast<T2>(w));
}

template<typename T>
Vec4<T>::operator Vec2<T>() const
{
	return Vec2<T>(data);
}

template<typename T>
Vec4<T>::operator Vec3<T>() const
{
	return Vec3<T>(data);
}

/////////////////////////////////////////////////
// Float 32-bit specialization                 //
/////////////////////////////////////////////////

template<>
Vec4<float32>::Vec4(const float32 x, const float32 y, const float32 z, const float32 w) : data(_mm_set_ps(x, y, z, w)) {}

template<>
Vec4<float32>::Vec4(const float32 s) : data(_mm_set1_ps(s)) {}

template<>
Vec4<float32> Vec4<float32>::getNormal() const
{
	const __m128 size = _mm_sqrt_ps(_mm_set1_ps(x * x + y * y + z * z + w * w));
	return Vec4<float32>(_mm_div_ps(data, size));
}

template<>
Vec4<float32> & Vec4<float32>::normalize()
{
	const __m128 size = _mm_sqrt_ps(_mm_set1_ps(x * x + y * y + z * z + w * w));
	data = _mm_div_ps(data, size);
	return *this;
}

template<>
bool Vec4<float32>::operator==(const Vec4<float32> & v) const
{
	return _mm_movemask_ps(_mm_cmp_ps(
		_mm_or_ps(
			_mm_sub_ps(data, v.data),
			_mm_set1_ps(-0.f)
		),
		_mm_set1_ps(-FLT_EPSILON),
		_CMP_LT_OQ
	)) == 0x0;
}

template<>
bool Vec4<float32>::operator!=(const Vec4<float32> & v) const
{
	return _mm_movemask_ps(_mm_cmp_ps(
		_mm_or_ps(
			_mm_sub_ps(data, v.data),
			_mm_set1_ps(-0.f)
		),
		_mm_set1_ps(-FLT_EPSILON),
		_CMP_LT_OQ
	));
}

template<>
bool Vec4<float32>::operator<(const Vec4<float32> & v) const
{
	return _mm_movemask_ps(_mm_cmp_ps(data, v.data, _CMP_LT_OQ)) == 0xf;
}

template<>
bool Vec4<float32>::operator<=(const Vec4<float32> & v) const
{
	return _mm_movemask_ps(_mm_cmp_ps(data, v.data, _CMP_LE_OQ)) == 0xf;
}

template<>
bool Vec4<float32>::operator>(const Vec4<float32> & v) const
{
	return _mm_movemask_ps(_mm_cmp_ps(data, v.data, _CMP_GT_OQ)) == 0xf;
}

template<>
bool Vec4<float32>::operator>=(const Vec4<float32> & v) const
{
	return _mm_movemask_ps(_mm_cmp_ps(data, v.data, _CMP_GE_OQ)) == 0xf;
}

template<>
Vec4<float32> Vec4<float32>::operator-() const
{
	return Vec4<float32>(_mm_xor_ps(data, _mm_set1_ps(-0.f)));
}

template<>
Vec4<float32> Vec4<float32>::operator+(const Vec4<float32> & v) const
{
	return Vec4<float32>(_mm_add_ps(data, v.data));
}

template<>
Vec4<float32> Vec4<float32>::operator-(const Vec4<float32> & v) const
{
	return Vec4<float32>(_mm_sub_ps(data, v.data));
}

template<>
Vec4<float32> Vec4<float32>::operator*(const Vec4<float32> & v) const
{
	return Vec4<float32>(_mm_mul_ps(data, v.data));
}

template<>
Vec4<float32> Vec4<float32>::operator/(const Vec4<float32> & v) const
{
	return Vec4<float32>(_mm_div_ps(data, v.data));
}

template<>
Vec4<float32> & Vec4<float32>::operator+=(const Vec4<float32> & v)
{
	data = _mm_add_ps(data, v.data);
	return *this;
}

template<>
Vec4<float32> & Vec4<float32>::operator-=(const Vec4<float32> & v)
{
	data = _mm_sub_ps(data, v.data);
	return *this;
}

template<>
Vec4<float32> & Vec4<float32>::operator*=(const Vec4<float32> & v)
{
	data = _mm_mul_ps(data, v.data);
	return *this;
}

template<>
Vec4<float32> & Vec4<float32>::operator/=(const Vec4<float32> & v)
{
	data = _mm_div_ps(data, v.data);
	return *this;
}

template<>
Vec4<float32> Vec4<float32>::operator+(const float32 s) const
{
	return Vec4<float32>(_mm_add_ps(data, _mm_set1_ps(s)));
}

template<>
Vec4<float32> Vec4<float32>::operator-(const float32 s) const
{
	return Vec4<float32>(_mm_sub_ps(data, _mm_set1_ps(s)));
}

template<>
Vec4<float32> Vec4<float32>::operator*(const float32 s) const
{
	return Vec4<float32>(_mm_mul_ps(data, _mm_set1_ps(s)));
}

template<>
Vec4<float32> Vec4<float32>::operator/(const float32 s) const
{
	return Vec4<float32>(_mm_div_ps(data, _mm_set1_ps(s)));
}

template<>
Vec4<float32> & Vec4<float32>::operator+=(const float32 s)
{
	data = _mm_add_ps(data, _mm_set1_ps(s));
	return *this;
}

template<>
Vec4<float32> & Vec4<float32>::operator-=(const float32 s)
{
	data = _mm_sub_ps(data, _mm_set1_ps(s));
	return *this;
}

template<>
Vec4<float32> & Vec4<float32>::operator*=(const float32 s)
{
	data = _mm_mul_ps(data, _mm_set1_ps(s));
	return *this;
}

template<>
Vec4<float32> & Vec4<float32>::operator/=(const float32 s)
{
	data = _mm_div_ps(data, _mm_set1_ps(s));
	return *this;
}

template<>
Vec4<float32> & Vec4<float32>::lerp(const Vec4<float32> & v, float32 alpha)
{
	data = _mm_add_ps(
		data,
		_mm_mul_ps(
			_mm_set1_ps(alpha),
			_mm_sub_ps(
				v.data,
				data
			)
		)
	);
	return *this;
}

template<>
void Vec4<float32>::print(FILE * stream) const
{
	fprintf(stream, "v4(%.3f, %.3f, %.3f, %.3f)\n", x, y, z, w);
}

/////////////////////////////////////////////////
// Int-32 specialization                       //
/////////////////////////////////////////////////

template<>
void Vec4<int32>::print(FILE * stream) const
{
	printf("v4(%d, %d, %d, %d)\n", x, y, z, w);
}

/////////////////////////////////////////////////
// Type definitions                            //
/////////////////////////////////////////////////

/**
 * @brief Type definitions for common vector types
 * @{
 */
typedef Vec4<float32>	vec4;
typedef Vec4<int32>		point4;
/** @} */

#endif