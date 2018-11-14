#ifndef SGL_MAT4_H
#define SGL_MAT4_H

#include "mat.h"
#include "vec4.h"

/**
 * @struct Mat4 mat4.h
 * @brief 4x4 templated matrix
 */
template<typename T = float32>
struct Mat4 : public Mat<T>
{
protected:
	/**
	 * @brief underlying data
	 */
	typename Mat4<T>::MT data;

public:
	/**
	 * @brief Default constructor
	 */
	inline Mat4();

	/**
	 * @brief Data-constructor
	 * 
	 * @param [in] data data to copy
	 */
	inline Mat4(const typename Mat4<T>::MT & data);
	
	/**
	 * @brief Elements-constructor
	 * 
	 * @param [in] a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p elements values
	 */
	inline Mat4(
		const T & a, const T & b, const T & c, const T & d,
		const T & e, const T & f, const T & g, const T & h,
		const T & i, const T & j, const T & k, const T & l,
		const T & m, const T & n, const T & o, const T & p
	);

	/**
	 * @brief Buffer-constructor
	 * 
	 * @param [in] buffer buffer of elements
	 */
	inline Mat4(const T * buffer);

	/**
	 * @brief Get a (modifiable) reference
	 * of the element in position (i,j)
	 * 
	 * @param [in] i,j element coordinates
	 * 
	 * @return reference to the element
	 */
	inline T & operator()(uint8 i, uint8 j) const;

	/**
	 * @brief Get a (modifiable) reference
	 * of the i-th element of the matrix
	 * 
	 * @param [in] i element index
	 * 
	 * @return i-th element reference
	 */
	inline T & operator[](uint8 i) const;

	/**
	 * @brief Comparison operators
	 * 
	 * @param [in] m other matrix
	 * 
	 * @return comparison result
	 * 
	 * @{
	 */
	inline bool operator==(const Mat4<T> & m) const;
	inline bool operator!=(const Mat4<T> & m) const;
	/** @} */

	/**
	 * @brief Get transposed matrix
	 * 
	 * @return transposed matrix
	 */
	inline Mat4<T> getTranspose() const;

	/**
	 * @brief Transpose in-place
	 * 
	 * @return self
	 */
	inline Mat4<T> & transpose();

	/**
	 * @brief Get matrix determinant
	 * 
	 * @return matrix determinant
	 */
	inline T getDeterminant() const;

	/**
	 * @brief Matrix-scalar operations
	 * 
	 * @param [in] s scalar value
	 * 
	 * @return result of operation
	 */
	inline Mat4<T> operator+(const T & s) const;
	inline Mat4<T> operator-(const T & s) const;
	inline Mat4<T> operator*(const T & s) const;
	inline Mat4<T> operator/(const T & s) const;
	/** @} */

	/**
	 * @brief Get inverse matrix
	 * 
	 * @return inverted matrix or zero-matrix if not invertible
	 */
	inline Mat4<T> operator!() const;
	
	/**
	 * @brief inverse matrix in-place
	 * 
	 * @return self
	 */
	inline Mat4<T> & invert();

	/**
	 * @brief Matrix-matrix multiplication
	 * 
	 * @param [in] m other matrix
	 * 
	 * @return multiplied matrix
	 */
	inline Mat4<T> operator*(const Mat4<T> & m) const;

	/**
	 * @brief Matrix-vector multiplication
	 * 
	 * @param [in] v vector to transform
	 * 
	 * @return transformed vector
	 * 
	 * @{
	 */
	inline vec2 operator*(const vec2 & v) const;
	inline vec3 operator*(const vec3 & v) const;
	inline vec4 operator*(const vec4 & v) const;
	/** @} */

	/**
	 * @brief Transformation constructors
	 * 
	 * @return Matrix that represent transformation
	 * 
	 * @{
	 */
	inline static Mat4<T> translation(const vec3 & v);
	inline static Mat4<T> scale(const vec3 & v);
	inline static Mat4<T> rotation(const Quat & q);
	/** @} */

	/**
	 * @brief Print matrix to stream
	 * 
	 * @param [in] stream output stream
	 */
	void print(FILE * stream = stdout);
};

template<typename T>
T & Mat4<T>::operator()(uint8 i, uint8 j) const
{
	// AVX registers are inverted
	j = 3 - j;
	return *((T*)&(data[i]) + j);
}

template<typename T>
T & Mat4<T>::operator[](uint8 i) const
{
	return operator()(i / 4, i % 4);
}

/////////////////////////////////////////////////
// Float 32-bit specialization                 //
/////////////////////////////////////////////////

template<>
Mat4<float32>::Mat4() : data{
	_mm_set1_ps(0.f),
	_mm_set1_ps(0.f),
	_mm_set1_ps(0.f),
	_mm_set1_ps(0.f)
} {}

template<>
Mat4<float32>::Mat4(const Mat4<float32>::MT & data)
{
	// Copy values
	memcpy(this->data, data, 4 * sizeof(__m128));
}

template<>
Mat4<float32>::Mat4(
	const float32 & a, const float32 & b, const float32 & c, const float32 & d,
	const float32 & e, const float32 & f, const float32 & g, const float32 & h,
	const float32 & i, const float32 & j, const float32 & k, const float32 & l,
	const float32 & m, const float32 & n, const float32 & o, const float32 & p
) : data{
	_mm_set_ps(a, b, c, d),
	_mm_set_ps(e, f, g, h),
	_mm_set_ps(i, j, k, l),
	_mm_set_ps(m, n, o, p)
} {}

template<>
bool Mat4<float32>::operator==(const Mat4<float32> & m) const
{
	return	_mm_movemask_ps(_mm_cmp_ps(data[0], m.data[0], _MM_CMPINT_EQ)) &&
			_mm_movemask_ps(_mm_cmp_ps(data[1], m.data[1], _MM_CMPINT_EQ)) &&
			_mm_movemask_ps(_mm_cmp_ps(data[2], m.data[2], _MM_CMPINT_EQ)) &&
			_mm_movemask_ps(_mm_cmp_ps(data[3], m.data[3], _MM_CMPINT_EQ));
}

template<>
bool Mat4<float32>::operator!=(const Mat4<float32> & m) const
{
	return	_mm_movemask_ps(_mm_cmp_ps(data[0], m.data[0], _MM_CMPINT_NE)) ||
			_mm_movemask_ps(_mm_cmp_ps(data[1], m.data[1], _MM_CMPINT_NE)) ||
			_mm_movemask_ps(_mm_cmp_ps(data[2], m.data[2], _MM_CMPINT_NE)) ||
			_mm_movemask_ps(_mm_cmp_ps(data[3], m.data[3], _MM_CMPINT_NE));
}

template<>
Mat4<float32> Mat4<float32>::getTranspose() const
{
	// Transposed data
	__m128 new_data[4];
	__m128 a, b, c, d;
	
	// First and second row
	a = _mm_unpackhi_ps(data[1], data[0]);
	b = _mm_unpackhi_ps(data[3], data[2]);
	new_data[0] = _mm_movehl_ps(a, b);
	new_data[1] = _mm_movelh_ps(b, a);

	// Third and fourth row
	c = _mm_unpacklo_ps(data[1], data[0]);
	d = _mm_unpacklo_ps(data[3], data[2]);
	new_data[2] = _mm_movehl_ps(c, d);
	new_data[3] = _mm_movelh_ps(d, c);
	
	return Mat4<float32>(new_data);
}

template<>
Mat4<float32> & Mat4<float32>::transpose()
{
	// Helper registers
	__m128 a, b, c, d;
	
	// Compute intermediary registers
	a = _mm_unpackhi_ps(data[1], data[0]);
	b = _mm_unpackhi_ps(data[3], data[2]);
	c = _mm_unpacklo_ps(data[1], data[0]);
	d = _mm_unpacklo_ps(data[3], data[2]);

	// Mutate
	data[0] = _mm_movehl_ps(a, b);
	data[1] = _mm_movelh_ps(b, a);
	data[2] = _mm_movehl_ps(c, d);
	data[3] = _mm_movelh_ps(d, c);

	// Return self
	return *this;
}

template<>
float32 Mat4<float32>::getDeterminant() const
{
	__m128 det;

	// Compute complement
	#define d(i) this->data[i]
	#define shuffle(v, x, y, z, w) _mm_shuffle_ps(v, v, _MM_SHUFFLE(x, y, z, w))
	#define mul(v1, v2) _mm_mul_ps(v1, v2)
	#define add(v1, v2) _mm_add_ps(v1, v2)
	#define sub(v1, v2) _mm_sub_ps(v1, v2)

	det = mul(
		_mm_set_ps(1.f, -1.f, 1.f, -1.f),
		mul(
			d(0),
			add(
				sub(
					mul(
						shuffle(d(1), 2, 3, 3, 3),
						sub(
							mul(
								shuffle(d(2), 1, 1, 2, 2),
								shuffle(d(3), 0, 0, 0, 1)
							),
							mul(
								shuffle(d(2), 0, 0, 0, 1),
								shuffle(d(3), 1, 1, 2, 2)
							)
						)
					),
					mul(
						shuffle(d(1), 1, 1, 2, 2),
						sub(
							mul(
								shuffle(d(2), 2, 3, 3, 3),
								shuffle(d(3), 0, 0, 0, 1)
							),
							mul(
								shuffle(d(2), 0, 0, 0, 1),
								shuffle(d(3), 2, 3, 3, 3)
							)
						)
					)
				),
				mul(
					shuffle(d(1), 0, 0, 0, 1),
					sub(
						mul(
							shuffle(d(2), 2, 3, 3, 3),
							shuffle(d(3), 1, 1, 2, 2)
						),
						mul(
							shuffle(d(2), 1, 1, 2, 2),
							shuffle(d(3), 2, 3, 3, 3)
						)
					)
				)
			)
		)
	);

	#undef d
	#undef shuffle
	#undef mul
	#undef add
	#undef sub

	det = _mm_hadd_ps(det, det);
	det = _mm_hadd_ps(det, det);

	return *((float32*)&det);
}

template<>
Mat4<float32> Mat4<float32>::operator+(const float32 & s) const
{
	__m128 new_data[4];
	for (uint8 i = 0; i < 4; i++) new_data[i] = _mm_add_ps(data[i], _mm_set1_ps(s));
	return Mat4<float32>(new_data);
}

template<>
Mat4<float32> Mat4<float32>::operator-(const float32 & s) const
{
	__m128 new_data[4];
	for (uint8 i = 0; i < 4; i++) new_data[i] = _mm_sub_ps(data[i], _mm_set1_ps(s));
	return Mat4<float32>(new_data);
}

template<>
Mat4<float32> Mat4<float32>::operator*(const float32 & s) const
{
	__m128 new_data[4];
	for (uint8 i = 0; i < 4; i++) new_data[i] = _mm_mul_ps(data[i], _mm_set1_ps(s));
	return Mat4<float32>(new_data);
}

template<>
Mat4<float32> Mat4<float32>::operator/(const float32 & s) const
{
	__m128 new_data[4];
	for (uint8 i = 0; i < 4; i++) new_data[i] = _mm_div_ps(data[i], _mm_set1_ps(s));
	return Mat4<float32>(new_data);
}

template<>
Mat4<float32> Mat4<float32>::operator!() const
{
	// Algebraic complement
	__m128 cmpl[4];

	// Complement sign
	const __m128	even	= _mm_set_ps(1.f, -1.f, 1.f, -1.f),
					odd		= _mm_set_ps(-1.f, 1.f, -1.f, 1.f);

	for (uint8 i = 0; i < 4; ++i)
	{
		// Get working rows
		const uint8	j = !i,
					k = i < 2 ? 2 : 1,
					l = i < 3 ? 3 : 2;

		// Compute complement
		#define d(i) this->data[i]
		#define shuffle(v, x, y, z, w) _mm_shuffle_ps(v, v, _MM_SHUFFLE(x, y, z, w))
		#define mul(v1, v2) _mm_mul_ps(v1, v2)
		#define add(v1, v2) _mm_add_ps(v1, v2)
		#define sub(v1, v2) _mm_sub_ps(v1, v2)

		cmpl[i] = mul(
			(i & 0x1 ? odd : even),
			add(
				sub(
					mul(
						shuffle(d(j), 2, 3, 3, 3),
						sub(
							mul(
								shuffle(d(k), 1, 1, 2, 2),
								shuffle(d(l), 0, 0, 0, 1)
							),
							mul(
								shuffle(d(k), 0, 0, 0, 1),
								shuffle(d(l), 1, 1, 2, 2)
							)
						)
					),
					mul(
						shuffle(d(j), 1, 1, 2, 2),
						sub(
							mul(
								shuffle(d(k), 2, 3, 3, 3),
								shuffle(d(l), 0, 0, 0, 1)
							),
							mul(
								shuffle(d(k), 0, 0, 0, 1),
								shuffle(d(l), 2, 3, 3, 3)
							)
						)
					)
				),
				mul(
					shuffle(d(j), 0, 0, 0, 1),
					sub(
						mul(
							shuffle(d(k), 2, 3, 3, 3),
							shuffle(d(l), 1, 1, 2, 2)
						),
						mul(
							shuffle(d(k), 1, 1, 2, 2),
							shuffle(d(l), 2, 3, 3, 3)
						)
					)
				)
			)
		);

		#undef d
		#undef shuffle
		#undef mul
		#undef add
		#undef sub
	}

	// Compute det
	__m128	det = _mm_mul_ps(data[0], cmpl[0]);
			det = _mm_hadd_ps(det, det);
			det = _mm_hadd_ps(det, det);

	// Divide all rows
	for (uint8 i = 0; i < 4; ++i) cmpl[i] = _mm_div_ps(cmpl[i], det);

	// Return matrix
	return Mat4<float32>(cmpl).transpose();
}

template<>
Mat4<float32> & Mat4<float32>::invert()
{
	// Algebraic complement
	__m128 cmpl[4];

	// Complement sign
	const __m128	even	= _mm_set_ps(1.f, -1.f, 1.f, -1.f),
					odd		= _mm_set_ps(-1.f, 1.f, -1.f, 1.f);

	for (uint8 i = 0; i < 4; ++i)
	{
		// Get working rows
		const uint8	j = !i,
					k = i < 2 ? 2 : 1,
					l = i < 3 ? 3 : 2;

		// Compute complement
		#define d(i) this->data[i]
		#define shuffle(v, x, y, z, w) _mm_shuffle_ps(v, v, _MM_SHUFFLE(x, y, z, w))
		#define mul(v1, v2) _mm_mul_ps(v1, v2)
		#define add(v1, v2) _mm_add_ps(v1, v2)
		#define sub(v1, v2) _mm_sub_ps(v1, v2)

		cmpl[i] = mul(
			(i & 0x1 ? odd : even),
			add(
				sub(
					mul(
						shuffle(d(j), 2, 3, 3, 3),
						sub(
							mul(
								shuffle(d(k), 1, 1, 2, 2),
								shuffle(d(l), 0, 0, 0, 1)
							),
							mul(
								shuffle(d(k), 0, 0, 0, 1),
								shuffle(d(l), 1, 1, 2, 2)
							)
						)
					),
					mul(
						shuffle(d(j), 1, 1, 2, 2),
						sub(
							mul(
								shuffle(d(k), 2, 3, 3, 3),
								shuffle(d(l), 0, 0, 0, 1)
							),
							mul(
								shuffle(d(k), 0, 0, 0, 1),
								shuffle(d(l), 2, 3, 3, 3)
							)
						)
					)
				),
				mul(
					shuffle(d(j), 0, 0, 0, 1),
					sub(
						mul(
							shuffle(d(k), 2, 3, 3, 3),
							shuffle(d(l), 1, 1, 2, 2)
						),
						mul(
							shuffle(d(k), 1, 1, 2, 2),
							shuffle(d(l), 2, 3, 3, 3)
						)
					)
				)
			)
		);

		#undef d
		#undef shuffle
		#undef mul
		#undef add
		#undef sub
	}

	// Compute det
	__m128	det = _mm_mul_ps(data[0], cmpl[0]);
			det = _mm_hadd_ps(det, det);
			det = _mm_hadd_ps(det, det);

	// Divide all rows and update data
	for (uint8 i = 0; i < 4; ++i) data[i] = _mm_div_ps(cmpl[i], det);

	// Return self
	return this->transpose();
}

/**
 * @todo maybe there's a better way, don't know
 */
template<>
Mat4<float32> Mat4<float32>::operator*(const Mat4<float32> & m) const
{
	// Multiplied data
	__m128 new_data[4];
	__m128 v;

	// Transpose operand
	Mat4<float32> mt = m.getTranspose();

	// Perform multiplication
	for (uint8 i = 0; i < 4; ++i)
		for (uint8 j = 0; j < 4; ++j)
		{
			// Perform dot product
			v = _mm_mul_ps(data[i], mt.data[j]);
			v = _mm_hadd_ps(v, v);
			v = _mm_hadd_ps(v, v);

			*((float32*)&(new_data[i]) + (3 - j)) = *((float32*)&v);
		}
	
	return Mat4<float32>(new_data);
}

template<>
vec2 Mat4<float32>::operator*(const vec2 & v) const
{
	// Out vector
	vec2 out(0.f);

	// Get SIMD data
	const __m128 v_data = v.data;
	*((float32*)&v_data) = *((float32*)&v_data + 1) = 1.f;
	__m128 dest;

	// Row 0
	dest = _mm_mul_ps(data[0], v_data);
	dest = _mm_hadd_ps(dest, dest);
	dest = _mm_hadd_ps(dest, dest);
	out.x = *((float32*)&dest);
	
	// Row 1
	dest = _mm_mul_ps(data[1], v_data);
	dest = _mm_hadd_ps(dest, dest);
	dest = _mm_hadd_ps(dest, dest);
	out.y = *((float32*)&dest);

	return out;
}

template<>
vec3 Mat4<float32>::operator*(const vec3 & v) const
{
	// Out vector
	vec3 out(0.f);

	// Get SIMD data
	const __m128 v_data = v.data;
	*((float32*)&v_data) = 1.f;
	__m128 dest;
	
	// Row 0
	dest = _mm_mul_ps(data[0], v_data);
	dest = _mm_hadd_ps(dest, dest);
	dest = _mm_hadd_ps(dest, dest);
	out.x = *((float32*)&dest);
	
	// Row 1
	dest = _mm_mul_ps(data[1], v_data);
	dest = _mm_hadd_ps(dest, dest);
	dest = _mm_hadd_ps(dest, dest);
	out.y = *((float32*)&dest);
	
	// Row 2
	dest = _mm_mul_ps(data[2], v_data);
	dest = _mm_hadd_ps(dest, dest);
	dest = _mm_hadd_ps(dest, dest);
	out.z = *((float32*)&dest);

	return out;
}

template<>
vec4 Mat4<float32>::operator*(const vec4 & v) const
{
	// Out vector
	vec4 out(0.f);
	
	// Destination register
	__m128 dest;
	
	// Row 0
	dest = _mm_mul_ps(data[0], v.data);
	dest = _mm_hadd_ps(dest, dest);
	dest = _mm_hadd_ps(dest, dest);
	out.x = *((float32*)&dest);
	
	// Row 1
	dest = _mm_mul_ps(data[1], v.data);
	dest = _mm_hadd_ps(dest, dest);
	dest = _mm_hadd_ps(dest, dest);
	out.y = *((float32*)&dest);
	
	// Row 2
	dest = _mm_mul_ps(data[2], v.data);
	dest = _mm_hadd_ps(dest, dest);
	dest = _mm_hadd_ps(dest, dest);
	out.z = *((float32*)&dest);
	
	// Row 3
	dest = _mm_mul_ps(data[3], v.data);
	dest = _mm_hadd_ps(dest, dest);
	dest = _mm_hadd_ps(dest, dest);
	out.w = *((float32*)&dest);

	return out;
}

template<>
Mat4<float32> Mat4<float32>::translation(const vec3 & v)
{
	// Translation matrix
	const __m128 data[4] = {
		_mm_set_ps(1.f, 0.f, 0.f, v.x),
		_mm_set_ps(0.f, 1.f, 0.f, v.y),
		_mm_set_ps(0.f, 0.f, 1.f, v.z),
		_mm_set_ps(0.f, 0.f, 0.f, 1.f)
	};
	return Mat4<float32>(data);
}

template<>
void Mat4<float32>::print(FILE * stream)
{
	fprintf(stream, "m4(\n");
	for (uint8 i = 0; i < 4; ++i)
	{
		const float32 * vals = (float32*)&data[i];
		fprintf(stream, "    %.3f, %.3f, %.3f, %.3f,\n", vals[3], vals[2], vals[1], vals[0]);
	}
	fprintf(stream, ")\n");
}

/////////////////////////////////////////////////
// Type definitions                            //
/////////////////////////////////////////////////

/**
 * @brief Type definitions for common vector types
 * @{
 */
typedef Mat4<float32> mat4;
/** @} */

#endif