#ifndef SGL_VEC4_H
#define SGL_VEC4_H

#include "vec3.h"

/**
 * @struct Vec4 vec4.h
 * @brief A 4-components templated vector
 */
template<typename T = float32>
struct Vec4 : public Vec<T>
{
protected:
	/**
	 * @brief SIMD vector
	 */
	typename Vec4<T>::VT data;

public:
	/**
	 * @brief References to vector coordinates
	 */
	T & x, & y, & z, & w;

public:
	/**
	 * @brief Zero-constructor
	 */
	inline Vec4();

	/**
	 * @brief Data-constructor
	 * 
	 * @param [in] data simd-like data structure
	 */
	inline Vec4(const typename Vec4::VT & data);

	/**
	 * @brief Coordinates-constructor
	 * 
	 * @param [in] x,y,z coordinates values
	 */
	inline Vec4(const T & x, const T & y, const T & z, const T & w);

	/**
	 * @brief Scalar-constructor
	 * 
	 * @param [in] s scalar value
	 */
	inline Vec4(const T & s);

	/**
	 * @brief Assignment operation
	 * 
	 * @param [in] v other vector
	 * 
	 * @return self
	 */
	Vec4<T> & operator=(const Vec4<T> & v);

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
	 * 
	 * @{
	 */
	inline bool operator==(const Vec4<T> & v) const;
	inline bool operator!=(const Vec4<T> & v) const;
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
	 * 
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
	 * 
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
	 * 
	 * @{
	 */
	inline Vec4<T> operator+(const T & s) const;
	inline Vec4<T> operator-(const T & s) const;
	inline Vec4<T> operator*(const T & s) const;
	inline Vec4<T> operator/(const T & s) const;
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
	inline Vec4<T> & operator+=(const T & s);
	inline Vec4<T> & operator-=(const T & s);
	inline Vec4<T> & operator*=(const T & s);
	inline Vec4<T> & operator/=(const T & s);
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
	 * @brief Component type conversion operator
	 * 
	 * @result converted vector
	 */
	template<typename T2>
	inline operator Vec4<T2>() const;

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
Vec4<T>::Vec4() :
	x(((T*)&data)[3]),
	y(((T*)&data)[2]),
	z(((T*)&data)[1]),
	w(((T*)&data)[0]) {}

template<typename T>
Vec4<T>::Vec4(const T & s) : Vec4(s, s, s, s) {}

template<typename T>
Vec4<T> & Vec4<T>::operator=(const Vec4<T> & v)
{
	data = v.data;
	return *this;
}

template<typename T>
const T & Vec4<T>::operator[](uint8 i) const
{
	// SIMD vector is reversed
	return ((T*)&data)[3 - i];
}

template<typename T>
float32 Vec4<T>::getSquaredSize() const
{
	return x * x + y * y + z * z + w * w;
}

template<typename T>
float32 Vec4<T>::getSize() const
{
	return sqrtf(getSquaredSize());
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
	/**
	 * @brief IEEE comparison should be
	 * faster than SIMD comparison
	 */
	return x == v.x && y == v.y && z == v.z && x == v.w;
}

template<typename T>
bool Vec4<T>::operator!=(const Vec4<T> & v) const
{
	return x != v.x || y != v.y || z != v.y || w != v.w;
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
Vec4<T> Vec4<T>::operator+(const T & s) const
{
	return Vec4<T>(x + s, y + s, z + s, w + s);
}

template<typename T>
Vec4<T> Vec4<T>::operator-(const T & s) const
{
	return Vec4<T>(x - s, y - s, z - s, w - s);
}

template<typename T>
Vec4<T> Vec4<T>::operator*(const T & s) const
{
	return Vec4<T>(x * s, y * s, z * s, w * s);
}

template<typename T>
Vec4<T> Vec4<T>::operator/(const T & s) const
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
inline Vec4<T> operator+(const T & s, const Vec4<T> & v)
{
	// Commutative
	return v + s;
}

template<typename T>
inline Vec4<T> operator-(const T & s, const Vec4<T> & v)
{
	return Vec4<T>(s) - v;
}

template<typename T>
inline Vec4<T> operator*(const T & s, const Vec4<T> & v)
{
	// Commutative
	return v * s;
}

template<typename T>
inline Vec4<T> operator/(const T & s, const Vec4<T> & v)
{
	return Vec4<T>(s) / v;
}
/** @} */

///////////////////////////////////////
// Vector-scalar compound operations //
///////////////////////////////////////

template<typename T>
Vec4<T> & Vec4<T>::operator+=(const T & s)
{
	x += s, y += s, z += s, w += s;
	return *this;
}

template<typename T>
Vec4<T> & Vec4<T>::operator-=(const T & s)
{
	x -= s, y -= s, z -= s, w -= s;
	return *this;
}

template<typename T>
Vec4<T> & Vec4<T>::operator*=(const T & s)
{
	x *= s, y *= s, z *= s, w *= s;
	return *this;
}

template<typename T>
Vec4<T> & Vec4<T>::operator/=(const T & s)
{
	x /= s, y /= s, z /= s, w /= s;
	return *this;
}

template<typename T>
T Vec4<T>::operator&(const Vec4<T> & v) const
{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

template<typename T1>
template<typename T2>
Vec4<T1>::operator Vec4<T2>() const
{
	return Vec4<T2>(static_cast<T2>(x), static_cast<T2>(y), static_cast<T2>(z), static_cast<T2>(w));
}

/////////////////////////////////////////////////
// Float 32-bit specialization                 //
/////////////////////////////////////////////////

template<>
Vec4<float32>::Vec4(const __m128 & data) : Vec4()
{
	this->data = data;
}

template<>
Vec4<float32>::Vec4(const float32 & x, const float32 & y, const float32 & z, const float32 & w) : Vec4()
{
	data = _mm_set_ps(x, y, z, w);
}

template<>
Vec4<float32>::Vec4(const float32 & s) : Vec4()
{
	data = _mm_set1_ps(s);
}

template<>
float32 Vec4<float32>::getSquaredSize() const
{
	__m128	res = _mm_mul_ps(data, data);
			res = _mm_hadd_ps(res, res);
			res = _mm_hadd_ps(res, res);
	return *((float*)&res);
}

template<>
Vec4<float32> Vec4<float32>::getNormal() const
{
	const __m128 size = _mm_set1_ps(getSize());
	return Vec4<float32>(_mm_div_ps(data, size));
}

template<>
Vec4<float32> & Vec4<float32>::normalize()
{
	const __m128 size = _mm_set1_ps(getSize());
	data = _mm_div_ps(data, size);
	return *this;
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
Vec4<float32> Vec4<float32>::operator+(const float32 & s) const
{
	return Vec4<float32>(_mm_add_ps(data, _mm_set1_ps(s)));
}

template<>
Vec4<float32> Vec4<float32>::operator-(const float32 & s) const
{
	return Vec4<float32>(_mm_sub_ps(data, _mm_set1_ps(s)));
}

template<>
Vec4<float32> Vec4<float32>::operator*(const float32 & s) const
{
	return Vec4<float32>(_mm_mul_ps(data, _mm_set1_ps(s)));
}

template<>
Vec4<float32> Vec4<float32>::operator/(const float32 & s) const
{
	return Vec4<float32>(_mm_div_ps(data, _mm_set1_ps(s)));
}

template<>
Vec4<float32> & Vec4<float32>::operator+=(const float32 & s)
{
	data = _mm_add_ps(data, _mm_set1_ps(s));
	return *this;
}

template<>
Vec4<float32> & Vec4<float32>::operator-=(const float32 & s)
{
	data = _mm_sub_ps(data, _mm_set1_ps(s));
	return *this;
}

template<>
Vec4<float32> & Vec4<float32>::operator*=(const float32 & s)
{
	data = _mm_mul_ps(data, _mm_set1_ps(s));
	return *this;
}

template<>
Vec4<float32> & Vec4<float32>::operator/=(const float32 & s)
{
	data = _mm_div_ps(data, _mm_set1_ps(s));
	return *this;
}

template<>
float32 Vec4<float32>::operator&(const Vec4<float32> & v) const
{
	__m128	res = _mm_mul_ps(data, v.data);
			res = _mm_hadd_ps(res, res);
			res = _mm_hadd_ps(res, res);
	return *((float32*)&res);
}

template<>
void Vec4<float32>::print(FILE * stream)
{
	printf("v4(%.3f, %.3f, %.3f, %.3f)\n", x, y, z, w);
}

#endif