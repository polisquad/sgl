#ifndef SGL_VEC3_H
#define SGL_VEC3_H

#include "vec.h"

/**
 * @struct Vec3 vec3.h
 * @brief a 3-components templated vector
 */
template<typename T = float32>
struct Vec3 : public Vec<T>
{
protected:
	/**
	 * @brief SIMD vector
	 */
	typename Vec3<T>::VT data;

public:
	/**
	 * @brief References to vector coordinates
	 */
	T & x, & y, & z;

public:
	/**
	 * @brief Zero-constructor
	 */
	inline Vec3();

	/**
	 * @brief Data-constructor
	 * 
	 * @param [in] data simd-like data structure
	 */
	inline Vec3(const typename Vec3::VT & data);

	/**
	 * @brief Coordinates-constructor
	 * 
	 * @param [in] x,y,z coordinates values
	 */
	inline Vec3(const T & x, const T & y, const T & z);

	/**
	 * @brief Scalar-constructor
	 * 
	 * @param [in] s scalar value
	 */
	inline Vec3(const T & s);

	/**
	 * @brief Assignment operation
	 * 
	 * @param [in] v other vector
	 * 
	 * @return self
	 */
	Vec3<T> & operator=(const Vec3<T> & v);

	/**
	 * @brief Return read-only element of the simd vector
	 * 
	 * @param [in] i element index
	 * 
	 * @return read-only copy
	 */
	const T & operator[](uint8 i) const;

	/**
	 * @brief Return squared size of vector
	 * 
	 * @return squared size
	 */
	float32 getSquaredSize() const;

	/**
	 * @brief Return size of vector
	 * 
	 * @return size (L^2-norm)
	 */
	float32 getSize() const;

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
	 * 
	 * @{
	 */
	inline bool operator==(const Vec3<T> & v) const;
	inline bool operator!=(const Vec3<T> & v) const;
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
	 * 
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
	 * 
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
	 * 
	 * @{
	 */
	inline Vec3<T> operator+(const T & s) const;
	inline Vec3<T> operator-(const T & s) const;
	inline Vec3<T> operator*(const T & s) const;
	inline Vec3<T> operator/(const T & s) const;
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
	inline Vec3<T> & operator+=(const T & s);
	inline Vec3<T> & operator-=(const T & s);
	inline Vec3<T> & operator*=(const T & s);
	inline Vec3<T> & operator/=(const T & s);
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
	 * @brief Component type conversion operator
	 * 
	 * @result converted vector
	 */
	template<typename T2>
	inline operator Vec3<T2>() const;

	/**
	 * @brief Print vector to stream
	 * 
	 * @param [in] stream output stream
	 */
	inline void print(FILE * stream = stdin);
};

//////////////////
// Constructors //
//////////////////

template<typename T>
Vec3<T>::Vec3() :
	x(((T*)&data)[3]),
	y(((T*)&data)[2]),
	z(((T*)&data)[1]) {}

template<typename T>
Vec3<T>::Vec3(const T & s) : Vec3(s, s, s) {}

template<typename T>
Vec3<T> & Vec3<T>::operator=(const Vec3<T> & v)
{
	data = v.data;
	return *this;
}

template<typename T>
const T & Vec3<T>::operator[](uint8 i) const
{
	// SIMD vector is reversed
	return ((T*)&data)[3 - i];
}

template<typename T>
float32 Vec3<T>::getSquaredSize() const
{
	return x * x + y * y + z * z;
}

template<typename T>
float32 Vec3<T>::getSize() const
{
	return sqrtf(getSquaredSize());
}

template<typename T>
Vec3<T> Vec3<T>::getNormal() const
{
	const float32 size = getSize();
	return size > 0.f ? Vec3<T>(x / size, y / size, z / size) : *Vec3<T>();
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
	/**
	 * @brief IEEE comparison should be
	 * faster than SIMD comparison
	 */
	return x == v.x && y == v.y && z == v.z;
}

template<typename T>
bool Vec3<T>::operator!=(const Vec3<T> & v) const
{
	return x != v.x || y != v.y || z != v.y;
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
Vec3<T> Vec3<T>::operator+(const T & s) const
{
	return Vec3<T>(x + s, y + s, z + s);
}

template<typename T>
Vec3<T> Vec3<T>::operator-(const T & s) const
{
	return Vec3<T>(x - s, y - s, z - s);
}

template<typename T>
Vec3<T> Vec3<T>::operator*(const T & s) const
{
	return Vec3<T>(x * s, y * s, z * s);
}

template<typename T>
Vec3<T> Vec3<T>::operator/(const T & s) const
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
 * 
 * @{
 */
template<typename T>
inline Vec3<T> operator+(const T & s, const Vec3<T> & v)
{
	// Commutative
	return v + s;
}

template<typename T>
inline Vec3<T> operator-(const T & s, const Vec3<T> & v)
{
	return Vec3<T>(s) - v;
}

template<typename T>
inline Vec3<T> operator*(const T & s, const Vec3<T> & v)
{
	// Commutative
	return v * s;
}

template<typename T>
inline Vec3<T> operator/(const T & s, const Vec3<T> & v)
{
	return Vec3<T>(s) / v;
}
/** @} */

///////////////////////////////////////
// Vector-scalar compound operations //
///////////////////////////////////////

template<typename T>
Vec3<T> & Vec3<T>::operator+=(const T & s)
{
	x += s, y += s, z += s;
	return *this;
}

template<typename T>
Vec3<T> & Vec3<T>::operator-=(const T & s)
{
	x -= s, y -= s, z -= s;
	return *this;
}

template<typename T>
Vec3<T> & Vec3<T>::operator*=(const T & s)
{
	x *= s, y *= s, z *= s;
	return *this;
}

template<typename T>
Vec3<T> & Vec3<T>::operator/=(const T & s)
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

template<typename T1>
template<typename T2>
Vec3<T1>::operator Vec3<T2>() const
{
	return Vec3<T2>(static_cast<T2>(x), static_cast<T2>(y), static_cast<T2>(z));
}

/////////////////////////////////////////////////
// Float 32-bit specialization                 //
/////////////////////////////////////////////////

template<>
Vec3<float32>::Vec3(const __m128 & data) : Vec3()
{
	this->data = data;
}

template<>
Vec3<float32>::Vec3(const float32 & x, const float32 & y, const float32 & z) : Vec3()
{
	data = _mm_set_ps(x, y, z, 0.f);
}

template<>
Vec3<float32>::Vec3(const float32 & s) : Vec3()
{
	data = _mm_set_ps1(s);
}

template<>
float32 Vec3<float32>::getSquaredSize() const
{
	// Zero extra element
	*((float32*)&data) = 0.f;
	__m128	res = _mm_mul_ps(data, data);
			res = _mm_hadd_ps(res, res);
			res = _mm_hadd_ps(res, res);
	return *((float*)&res);
}

template<>
Vec3<float32> Vec3<float32>::getNormal() const
{
	const __m128 size = _mm_set1_ps(getSize());
	return Vec3<float32>(_mm_div_ps(data, size));
}

template<>
Vec3<float32> & Vec3<float32>::normalize()
{
	const __m128 size = _mm_set1_ps(getSize());
	data = _mm_div_ps(data, size);
	return *this;
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
Vec3<float32> Vec3<float32>::operator+(const float32 & s) const
{
	return Vec3<float32>(_mm_add_ps(data, _mm_set1_ps(s)));
}

template<>
Vec3<float32> Vec3<float32>::operator-(const float32 & s) const
{
	return Vec3<float32>(_mm_sub_ps(data, _mm_set1_ps(s)));
}

template<>
Vec3<float32> Vec3<float32>::operator*(const float32 & s) const
{
	return Vec3<float32>(_mm_mul_ps(data, _mm_set1_ps(s)));
}

template<>
Vec3<float32> Vec3<float32>::operator/(const float32 & s) const
{
	return Vec3<float32>(_mm_div_ps(data, _mm_set1_ps(s)));
}

template<>
Vec3<float32> & Vec3<float32>::operator+=(const float32 & s)
{
	data = _mm_add_ps(data, _mm_set1_ps(s));
	return *this;
}

template<>
Vec3<float32> & Vec3<float32>::operator-=(const float32 & s)
{
	data = _mm_sub_ps(data, _mm_set1_ps(s));
	return *this;
}

template<>
Vec3<float32> & Vec3<float32>::operator*=(const float32 & s)
{
	data = _mm_mul_ps(data, _mm_set1_ps(s));
	return *this;
}

template<>
Vec3<float32> & Vec3<float32>::operator/=(const float32 & s)
{
	data = _mm_div_ps(data, _mm_set1_ps(s));
	return *this;
}

template<>
float32 Vec3<float32>::operator&(const Vec3<float32> & v) const
{
	*((float32*)&data) = *((float32*)&v.data) = 0.f;
	__m128	res = _mm_mul_ps(data, v.data);
			res = _mm_hadd_ps(res, res);
			res = _mm_hadd_ps(res, res);
	return *((float32*)&res);
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
void Vec3<float32>::print(FILE * stream)
{
	printf("v3(%.3f, %.3f, %.3f)\n", x, y, z);
}

/////////////////////////////////////////////////
// Int 32-bit specialization                   //
/////////////////////////////////////////////////

template<>
Vec3<int32>::Vec3(const __m128i & data) : Vec3()
{
	this->data = data;
}

template<>
Vec3<int32>::Vec3(const int32 & x, const int32 & y, const int32 & z) : Vec3()
{
	data = _mm_set_epi32(x, y, z, 0);
}

template<>
Vec3<int32>::Vec3(const int32 & s) : Vec3()
{
	data = _mm_set1_epi32(s);
}

template<>
void Vec3<int32>::print(FILE * stream)
{
	printf("v3(%d, %d, %d)\n", x, y, z);
}

#endif