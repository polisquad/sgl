#ifndef SGL_VEC3_H
#define SGL_VEC3_H

#include "vec2.h"

/**
 * @struct Vec3 vec3.h
 * @brief a 3-components templated vector
 */
template<typename T = float32>
struct Vec3 : public SIMD<T>
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
			T __gap;

			/**
			 * @brief Vector components
			 * @{
			 */
			T z, y, x;
			/** @} */
		};
	};

	/**
	 * @brief Pre-defined vectors
	 */
	static const Vec3	zero,
						forward, backward,
						right, left,
						up, down;

public:
	/**
	 * @brief Zero-constructor
	 */
	inline Vec3();

	/**
	 * @brief Copy-constructor
	 * 
	 * @param [in] v other vector
	 */
	inline Vec3(const Vec3<T> & v);

	/**
	 * @brief Data-constructor
	 * 
	 * @param [in] data simd-like data structure
	 */
	inline Vec3(const typename Vec3::VT data);

	/**
	 * @brief Vec-constructor
	 * 
	 * @param [in] vec plain c array
	 */
	inline Vec3(const T * vec);

	/**
	 * @brief Coordinates-constructor
	 * 
	 * @param [in] x,y,z coordinates values
	 */
	inline Vec3(const T x, const T y, const T z);

	/**
	 * @brief Scalar-constructor
	 * 
	 * @param [in] s scalar value
	 */
	inline Vec3(const T s);

	/**
	 * @brief Vec2-constructor
	 * 
	 * @param [in]	v2	Vec2 vector
	 * @param [in]	z	missing component
	 */
	inline Vec3(const Vec2<T> & v2, const T z = T());

	/**
	 * @brief Assignment operation
	 * 
	 * @param [in] v other vector
	 * 
	 * @return self
	 */
	inline Vec3<T> & operator=(const Vec3<T> & v);

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
	inline Vec3<T> getNormal() const;

	/**
	 * @brief Normalize vector in place
	 * 
	 * @return self
	 */
	inline Vec3<T> & normalize();

	/**
	 * @brief Compare two vectors
	 * 
	 * @param [in] v other vector
	 * 
	 * @return Comparison result
	 * @{
	 */
	inline bool operator==(const Vec3<T> & v) const;
	inline bool operator!=(const Vec3<T> & v) const;
	inline bool operator<(const Vec3<T> & v) const;
	inline bool operator<=(const Vec3<T> & v) const;
	inline bool operator>(const Vec3<T> & v) const;
	inline bool operator>=(const Vec3<T> & v) const;
	/** @} */

	/**
	 * @brief Invert vector components
	 * 
	 * @return inverted vectors
	 */
	inline Vec3<T> operator-() const;

	/**
	 * @brief Vector-vector component-wise operations
	 * 
	 * @param [in] v other vector
	 * 
	 * @return result of operation
	 * @{
	 */
	inline Vec3<T> operator+(const Vec3<T> & v) const;
	inline Vec3<T> operator-(const Vec3<T> & v) const;
	inline Vec3<T> operator*(const Vec3<T> & v) const;
	inline Vec3<T> operator/(const Vec3<T> & v) const;
	/** @} */

	/**
	 * @brief Vector-vector compound component-wise operations
	 * 
	 * @param [in] v other vector
	 * 
	 * @return result of operation
	 * @{
	 */
	inline Vec3<T> & operator+=(const Vec3<T> & v);
	inline Vec3<T> & operator-=(const Vec3<T> & v);
	inline Vec3<T> & operator*=(const Vec3<T> & v);
	inline Vec3<T> & operator/=(const Vec3<T> & v);
	/** @} */

	/**
	 * @brief Vector-scalar element-wise operations
	 * 
	 * @param [in] s scalar value
	 * 
	 * @return result of operation
	 * @{
	 */
	inline Vec3<T> operator+(const T s) const;
	inline Vec3<T> operator-(const T s) const;
	inline Vec3<T> operator*(const T s) const;
	inline Vec3<T> operator/(const T s) const;
	/** @} */

	/**
	 * @brief Vector-scalar compound element-wise operations
	 * 
	 * @param [in] s scalar value
	 * 
	 * @return result of operation
	 * @{
	 */
	inline Vec3<T> & operator+=(const T s);
	inline Vec3<T> & operator-=(const T s);
	inline Vec3<T> & operator*=(const T s);
	inline Vec3<T> & operator/=(const T s);
	/** @} */

	/**
	 * @brief Vector-vector dot(scalar) product
	 * 
	 * @param [in] v other vector
	 * 
	 * @return dot product
	 */
	inline T operator&(const Vec3<T> & v) const;

	/**
	 * @brief Vector-vector cross product
	 * 
	 * @param [in] v other vector
	 * 
	 * @return cross product
	 */
	inline Vec3<T> operator^(const Vec3<T> & v) const;

	/**
	 * @brief Linear interpolate with other vector
	 * 
	 * @param [in]	v		other vector
	 * @param [in]	alpha	interpolation step
	 * 
	 * @return self
	 */
	inline Vec3<T> & lerp(const Vec3<T> & v, float32 alpha);

	/**
	 * @copydoc SIMD<T>::print()
	 */
	inline void print(FILE * stream = stdout) const;

	/**
	 * @brief Component type conversion operator
	 * 
	 * @return converted vector
	 */
	template<typename T2>
	inline operator Vec3<T2>() const;

	/**
	 * @brief Vec2 conversion
	 * 
	 * @return converted vector
	 */
	inline explicit operator Vec2<T>() const;
};

//////////////////
// Constructors //
//////////////////

template<typename T>
Vec3<T>::Vec3() {}

template<typename T>
Vec3<T>::Vec3(const Vec3<T> & v) : data(v.data) {}

template<typename T>
Vec3<T>::Vec3(const typename Vec3<T>::VT data) : data(data) {}

template<typename T>
Vec3<T>::Vec3(const T * __vec) { memcpy(this->__vec, __vec, 4 * sizeof(T)); }

template<typename T>
Vec3<T>::Vec3(const T x, const T y, const T z) : x(x), y(y), z(z) {}

template<typename T>
Vec3<T>::Vec3(const T s) : x(s), y(s), z(s) {}

template<typename T>
Vec3<T>::Vec3(const Vec2<T> & v2, const T z) : x(v2.x), y(v2.y), z(z) {}

template<typename T>
Vec3<T> & Vec3<T>::operator=(const Vec3<T> & v)
{
	data = v.data;
	return *this;
}

template<typename T>
T & Vec3<T>::operator[](uint8 i)
{
	// SIMD vector is reversed
	return __vec[3 - i];
}

template<typename T>
float32 Vec3<T>::getSquaredSize() const
{
	return x * x + y * y + z * z;
}

template<typename T>
float32 Vec3<T>::getSize() const
{
	return sqrtf(x * x + y * y + z * z);
}

template<typename T>
Vec3<T> Vec3<T>::getNormal() const
{
	const float32 size = getSize();
	return Vec3<T>(x / size, y / size, z / size);
}

template<typename T>
Vec3<T> & Vec3<T>::normalize()
{
	const float32 size = getSize();
	x /= size, y /= size, z /= size;
	return *this;
}

///////////////////////////////
// Vector-vector comaparison //
///////////////////////////////

template<typename T>
bool Vec3<T>::operator==(const Vec3<T> & v) const
{
	return x == v.x && y == v.y && z == v.z;
}

template<typename T>
bool Vec3<T>::operator!=(const Vec3<T> & v) const
{
	return x != v.x || y != v.y || z != v.y;
}

template<typename T>
bool Vec3<T>::operator<(const Vec3<T> & v) const
{
	return x < v.x && y < v.y && z < v.z;
}

template<typename T>
bool Vec3<T>::operator<=(const Vec3<T> & v) const
{
	return x <= v.x && y <= v.y && z <= v.z;
}

template<typename T>
bool Vec3<T>::operator>(const Vec3<T> & v) const
{
	return x > v.x && y > v.y && z > v.z;
}

template<typename T>
bool Vec3<T>::operator>=(const Vec3<T> & v) const
{
	return x >= v.x && y >= v.y && z >= v.z;
}

template<typename T>
Vec3<T> Vec3<T>::operator-() const
{
	return Vec3<T>(-x, -y, -z);
}

//////////////////////////////
// Vector-vector operations //
//////////////////////////////

template<typename T>
Vec3<T> Vec3<T>::operator+(const Vec3<T> & v) const
{
	return Vec3<T>(x + v.x, y + v.y, z + v.z);
}

template<typename T>
Vec3<T> Vec3<T>::operator-(const Vec3<T> & v) const
{
	return Vec3<T>(x - v.x, y - v.y, z - v.z);
}

template<typename T>
Vec3<T> Vec3<T>::operator*(const Vec3<T> & v) const
{
	return Vec3<T>(x * v.x, y * v.y, z * v.z);
}

template<typename T>
Vec3<T> Vec3<T>::operator/(const Vec3<T> & v) const
{
	return Vec3<T>(x / v.x, y / v.y, z / v.z);
}

//////////////////////////////////////
// Vector-vector compund operations //
//////////////////////////////////////

template<typename T>
Vec3<T> & Vec3<T>::operator+=(const Vec3<T> & v)
{
	x += v.x, y += v.y, z += v.z;
	return *this;
}

template<typename T>
Vec3<T> & Vec3<T>::operator-=(const Vec3<T> & v)
{
	x -= v.x, y -= v.y, z -= v.z;
	return *this;
}

template<typename T>
Vec3<T> & Vec3<T>::operator*=(const Vec3<T> & v)
{
	x *= v.x, y *= v.y, z *= v.z;
	return *this;
}

template<typename T>
Vec3<T> & Vec3<T>::operator/=(const Vec3<T> & v)
{
	x /= v.x, y /= v.y, z /= v.z;
	return *this;
}

//////////////////////////////
// Vector-scalar operations //
//////////////////////////////

template<typename T>
Vec3<T> Vec3<T>::operator+(const T s) const
{
	return Vec3<T>(x + s, y + s, z + s);
}

template<typename T>
Vec3<T> Vec3<T>::operator-(const T s) const
{
	return Vec3<T>(x - s, y - s, z - s);
}

template<typename T>
Vec3<T> Vec3<T>::operator*(const T s) const
{
	return Vec3<T>(x * s, y * s, z * s);
}

template<typename T>
Vec3<T> Vec3<T>::operator/(const T s) const
{
	return Vec3<T>(x / s, y / s, z / s);
}

/**
 * @brief Symmetric version of vector-scalar operations
 * 
 * @param [in]	v	vector
 * @param [in]	s	scalar
 * 
 * @return result of operation
 * @{
 */
template<typename T>
inline Vec3<T> operator+(const T s, const Vec3<T> & v)
{
	// Commutative
	return v + s;
}

template<typename T>
inline Vec3<T> operator-(const T s, const Vec3<T> & v)
{
	return Vec3<T>(s) - v;
}

template<typename T>
inline Vec3<T> operator*(const T s, const Vec3<T> & v)
{
	// Commutative
	return v * s;
}

template<typename T>
inline Vec3<T> operator/(const T s, const Vec3<T> & v)
{
	return Vec3<T>(s) / v;
}
/** @} */

///////////////////////////////////////
// Vector-scalar compound operations //
///////////////////////////////////////

template<typename T>
Vec3<T> & Vec3<T>::operator+=(const T s)
{
	x += s, y += s, z += s;
	return *this;
}

template<typename T>
Vec3<T> & Vec3<T>::operator-=(const T s)
{
	x -= s, y -= s, z -= s;
	return *this;
}

template<typename T>
Vec3<T> & Vec3<T>::operator*=(const T s)
{
	x *= s, y *= s, z *= s;
	return *this;
}

template<typename T>
Vec3<T> & Vec3<T>::operator/=(const T s)
{
	x /= s, y /= s, z /= s;
	return *this;
}

template<typename T>
T Vec3<T>::operator&(const Vec3<T> & v) const
{
	return x * v.x + y * v.y + z * v.z;
}

template<typename T>
Vec3<T> Vec3<T>::operator^(const Vec3<T> & v) const
{
	return Vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

template<typename T>
Vec3<T> & Vec3<T>::lerp(const Vec3<T> & v, float32 alpha)
{
	*this += alpha * (v - *this);
	return *this;
}

template<typename T1>
template<typename T2>
Vec3<T1>::operator Vec3<T2>() const
{
	return Vec3<T2>(static_cast<T2>(x), static_cast<T2>(y), static_cast<T2>(z));
}

template<typename T>
Vec3<T>::operator Vec2<T>() const
{
	return Vec2<T>(data);
}

/////////////////////////////////////////////////
// Float 32-bit specialization                 //
/////////////////////////////////////////////////

template<>
Vec3<float32>::Vec3(const float32 x, const float32 y, const float32 z) : data(_mm_set_ps(x, y, z, 0.f)) {}

template<>
Vec3<float32>::Vec3(const float32 s) : data(_mm_set1_ps(s)) {}

template<> const Vec3<float32> Vec3<float32>::zero(0.f);
template<> const Vec3<float32> Vec3<float32>::forward(0.f, 0.f, -1.f);
template<> const Vec3<float32> Vec3<float32>::backward(0.f, 0.f, 1.f);
template<> const Vec3<float32> Vec3<float32>::right(1.f, 0.f, 0.f);
template<> const Vec3<float32> Vec3<float32>::left(-1.f, 0.f, 0.f);
template<> const Vec3<float32> Vec3<float32>::up(0.f, 1.f, 0.f);
template<> const Vec3<float32> Vec3<float32>::down(0.f, -1.f, 0.f);

template<>
Vec3<float32> Vec3<float32>::getNormal() const
{
	const __m128 size = _mm_sqrt_ps(_mm_set1_ps(x * x + y * y + z * z));
	return Vec3<float32>(_mm_div_ps(data, size));
}

template<>
Vec3<float32> & Vec3<float32>::normalize()
{
	const __m128 size = _mm_sqrt_ps(_mm_set1_ps(x * x + y * y + z * z));
	data = _mm_div_ps(data, size);
	return *this;
}

template<>
bool Vec3<float32>::operator==(const Vec3<float32> & v) const
{
	return (_mm_movemask_ps(_mm_cmp_ps(
		_mm_or_ps(
			_mm_sub_ps(data, v.data),
			_mm_set1_ps(-0.f)
		),
		_mm_set1_ps(-FLT_EPSILON),
		_CMP_LT_OQ
	)) & 0xe) == 0x0;
}

template<>
bool Vec3<float32>::operator!=(const Vec3<float32> & v) const
{
	return _mm_movemask_ps(_mm_cmp_ps(
		_mm_or_ps(
			_mm_sub_ps(data, v.data),
			_mm_set1_ps(-0.f)
		),
		_mm_set1_ps(-FLT_EPSILON),
		_CMP_LT_OQ
	)) & 0xe;
}

template<>
bool Vec3<float32>::operator<=(const Vec3<float32> & v) const
{
	return (_mm_movemask_ps(_mm_cmp_ps(data, v.data, _CMP_LE_OQ)) & 0xe) == 0xe;
}

template<>
bool Vec3<float32>::operator>=(const Vec3<float32> & v) const
{
	return (_mm_movemask_ps(_mm_cmp_ps(data, v.data, _CMP_GE_OQ)) & 0xe) == 0xe;
}

template<>
Vec3<float32> Vec3<float32>::operator-() const
{
	return Vec3<float32>(_mm_xor_ps(data, _mm_set1_ps(-0.f)));
}

template<>
Vec3<float32> Vec3<float32>::operator+(const Vec3<float32> & v) const
{
	return Vec3<float32>(_mm_add_ps(data, v.data));
}

template<>
Vec3<float32> Vec3<float32>::operator-(const Vec3<float32> & v) const
{
	return Vec3<float32>(_mm_sub_ps(data, v.data));
}

template<>
Vec3<float32> Vec3<float32>::operator*(const Vec3<float32> & v) const
{
	return Vec3<float32>(_mm_mul_ps(data, v.data));
}

template<>
Vec3<float32> Vec3<float32>::operator/(const Vec3<float32> & v) const
{
	return Vec3<float32>(_mm_div_ps(data, v.data));
}

template<>
Vec3<float32> & Vec3<float32>::operator+=(const Vec3<float32> & v)
{
	data = _mm_add_ps(data, v.data);
	return *this;
}

template<>
Vec3<float32> & Vec3<float32>::operator-=(const Vec3<float32> & v)
{
	data = _mm_sub_ps(data, v.data);
	return *this;
}

template<>
Vec3<float32> & Vec3<float32>::operator*=(const Vec3<float32> & v)
{
	data = _mm_mul_ps(data, v.data);
	return *this;
}

template<>
Vec3<float32> & Vec3<float32>::operator/=(const Vec3<float32> & v)
{
	data = _mm_div_ps(data, v.data);
	return *this;
}

template<>
Vec3<float32> Vec3<float32>::operator+(const float32 s) const
{
	return Vec3<float32>(_mm_add_ps(data, _mm_set1_ps(s)));
}

template<>
Vec3<float32> Vec3<float32>::operator-(const float32 s) const
{
	return Vec3<float32>(_mm_sub_ps(data, _mm_set1_ps(s)));
}

template<>
Vec3<float32> Vec3<float32>::operator*(const float32 s) const
{
	return Vec3<float32>(_mm_mul_ps(data, _mm_set1_ps(s)));
}

template<>
Vec3<float32> Vec3<float32>::operator/(const float32 s) const
{
	return Vec3<float32>(_mm_div_ps(data, _mm_set1_ps(s)));
}

template<>
Vec3<float32> & Vec3<float32>::operator+=(const float32 s)
{
	data = _mm_add_ps(data, _mm_set1_ps(s));
	return *this;
}

template<>
Vec3<float32> & Vec3<float32>::operator-=(const float32 s)
{
	data = _mm_sub_ps(data, _mm_set1_ps(s));
	return *this;
}

template<>
Vec3<float32> & Vec3<float32>::operator*=(const float32 s)
{
	data = _mm_mul_ps(data, _mm_set1_ps(s));
	return *this;
}

template<>
Vec3<float32> & Vec3<float32>::operator/=(const float32 s)
{
	data = _mm_div_ps(data, _mm_set1_ps(s));
	return *this;
}

template<>
Vec3<float32> Vec3<float32>::operator^(const Vec3<float32> & v) const
{
	const __m128 res =_mm_sub_ps(
		_mm_mul_ps(_mm_shuffle_ps(data, data, _MM_SHUFFLE(1, 3, 2, 0)), v.data),
		_mm_mul_ps(data, _mm_shuffle_ps(v.data, v.data, _MM_SHUFFLE(1, 3, 2, 0)))
	);
	return _mm_shuffle_ps(res, res, _MM_SHUFFLE(1, 3, 2, 0));
}

template<>
Vec3<float32> & Vec3<float32>::lerp(const Vec3<float32> & v, float32 alpha)
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
void Vec3<float32>::print(FILE * stream) const
{
	fprintf(stream, "v3(%.3f, %.3f, %.3f)\n", x, y, z);
}

/////////////////////////////////////////////////
// Int-32 specialization                       //
/////////////////////////////////////////////////

template<>
void Vec3<int32>::print(FILE * stream) const
{
	printf("v3(%d, %d, %d)\n", x, y, z);
}

/////////////////////////////////////////////////
// Type definitions                            //
/////////////////////////////////////////////////

/**
 * @brief Type definitions for common vector types
 * @{
 */
typedef Vec3<float32>	vec3;
typedef Vec3<int32>		point3;
/** @} */

#endif