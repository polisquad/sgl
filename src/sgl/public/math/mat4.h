#ifndef SGL_MAT4_H
#define SGL_MAT4_H

#include "quat.h"

/**
 * @struct Mat4 mat4.h
 * @brief 4x4 templated matrix
 */
template<typename T = float32>
struct Mat4 : public SIMD<T>
{
	/**
	 * @brief Friendship declarations
	 * @{
	 */
	friend struct Math;
	/** @} */

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
		const T a, const T b, const T c, const T d,
		const T e, const T f, const T g, const T h,
		const T i, const T j, const T k, const T l,
		const T m, const T n, const T o, const T p
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
	 * @brief Row and column accessors
	 * 
	 * @param [in] i row[column] index
	 * 
	 * @return Vec4 representing row[column]
	 * @{
	 */
	inline Vec4<T> getRow(uint8 i) const;
	inline Vec4<T> getCol(uint8 i) const;
	/** @} */

	/**
	 * @brief Returns a buffer that contains ordered data
	 * @details it is up to the user to
	 * free the buffer after its use
	 * 
	 * @return buffer with data
	 */
	T * getDataBuffer() const;

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
	inline Mat4<T> operator+(const T s) const;
	inline Mat4<T> operator-(const T s) const;
	inline Mat4<T> operator*(const T s) const;
	inline Mat4<T> operator/(const T s) const;
	/** @} */

	/**
	 * @brief Vector-matrix operations
	 * @details Element-wise operations where vector
	 * is replicated 4 times
	 * 
	 * @param [in]	v	vector
	 * @param [in]	m	matrix
	 * 
	 * @return multiplied matrix
	 * @{
	 */
	template<typename F> inline friend Mat4<F> operator+(const Vec4<F> & v, const Mat4<F> & m);
	template<typename F> inline friend Mat4<F> operator-(const Vec4<F> & v, const Mat4<F> & m);
	template<typename F> inline friend Mat4<F> operator*(const Vec4<F> & v, const Mat4<F> & m);
	template<typename F> inline friend Mat4<F> operator/(const Vec4<F> & v, const Mat4<F> & m);
	/** @} */

	/**
	 * @brief Get inverse matrix
	 * 
	 * @return inverted matrix or zero-matrix if not invertible
	 * 
	 * @{
	 */
	inline Mat4<T> operator!() const;
	inline Mat4<T> getInverse() const;
	/** @} */
	
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
	 * @brief Static constructors
	 * 
	 * @return constructed matrix
	 * 
	 * @{
	 */
	inline static Mat4<T> fill(const T s);
	inline static Mat4<T> eye(const T s);
	inline static Mat4<T> diag(const Vec4<T> & d);
	inline static Mat4<T> diag(const T x, const T y, const T z, const T w);
	inline static Mat4<T> translation(const Vec3<T> & v);
	inline static Mat4<T> translation(const T x, const T y, const T z);
	inline static Mat4<T> scale(const Vec3<T> & v);
	inline static Mat4<T> scale(const T s);
	inline static Mat4<T> rotation(const Quat<T> & q);
	inline static Mat4<T> rotation(const Vec3<T> & v, const T a);
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
	j -= 3;
	return *((T*)&(data[i]) - j);
}

template<typename T>
T & Mat4<T>::operator[](uint8 i) const
{
	return operator()(i / 4, i % 4);
}

template<typename T>
Vec4<T> Mat4<T>::getRow(uint8 i) const
{
	return Vec4<T>(data[i]);
}

template<typename T>
Vec4<T> Mat4<T>::getCol(uint8 i) const
{
	Mat4<T> t = getTranspose();
	return Vec4<T>(t.data[i]);
}

template<typename T>
T * Mat4<T>::getDataBuffer() const
{
	// Allocate buffer and copy data
	T * buff = static_cast<T*>(malloc(16 * sizeof(T)));
	for (uint8 i = 0; i < 4; i++) _mm_store_ps(buff + i * 4, data);

	return buff;
}

template<typename T>
Mat4<T> Mat4<T>::getInverse() const
{
	return operator!();
}

/**
 * @brief Scalar-matrix operations
 * @details Commutativity is exploited in + and *
 * Vector-Matrix operations are used for - and /
 * 
 * @param [in]	s	scalar
 * @param [in]	m	matrix
 * 
 * @return result of operation
 * @{
 */
template<typename T>
inline Mat4<T> operator+(const T s, const Mat4<T> & m)
{
	return m + s;
}

template<typename T>
inline Mat4<T> operator-(const T s, const Mat4<T> & m)
{
	return Vec4<T>(s) - m;
}

template<typename T>
inline Mat4<T> operator*(const T s, const Mat4<T> & m)
{
	return m * s;
}

template<typename T>
inline Mat4<T> operator/(const T s, const Mat4<T> & m)
{
	return Vec4<T>(s) / m;
}
/** @} */

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
	const float32 a, const float32 b, const float32 c, const float32 d,
	const float32 e, const float32 f, const float32 g, const float32 h,
	const float32 i, const float32 j, const float32 k, const float32 l,
	const float32 m, const float32 n, const float32 o, const float32 p
) : data{
	_mm_set_ps(a, b, c, d),
	_mm_set_ps(e, f, g, h),
	_mm_set_ps(i, j, k, l),
	_mm_set_ps(m, n, o, p)
} {}

template<>
float32 * Mat4<float32>::getDataBuffer() const
{
	// Allocate buffer and copy data
	float32 * buff = static_cast<float32*>(malloc(16 * sizeof(float32)));
	for (uint8 i = 0; i < 4; i++) _mm_storer_ps(buff + i * 4, data[i]);

	return buff;
}

template<>
bool Mat4<float32>::operator==(const Mat4<float32> & m) const
{
	return ~(
		_mm_movemask_ps(_mm_cmp_ps(_mm_or_ps(_mm_sub_ps(data[0], m.data[0]), _mm_set1_ps(-0.f)), _mm_set1_ps(-FLT_EPSILON), _CMP_LT_OQ)) |
		_mm_movemask_ps(_mm_cmp_ps(_mm_or_ps(_mm_sub_ps(data[1], m.data[1]), _mm_set1_ps(-0.f)), _mm_set1_ps(-FLT_EPSILON), _CMP_LT_OQ)) |
		_mm_movemask_ps(_mm_cmp_ps(_mm_or_ps(_mm_sub_ps(data[2], m.data[2]), _mm_set1_ps(-0.f)), _mm_set1_ps(-FLT_EPSILON), _CMP_LT_OQ)) |
		_mm_movemask_ps(_mm_cmp_ps(_mm_or_ps(_mm_sub_ps(data[3], m.data[3]), _mm_set1_ps(-0.f)), _mm_set1_ps(-FLT_EPSILON), _CMP_LT_OQ))
	);
}

template<>
bool Mat4<float32>::operator!=(const Mat4<float32> & m) const
{
	return	_mm_movemask_ps(_mm_cmp_ps(_mm_or_ps(_mm_sub_ps(data[0], m.data[0]), _mm_set1_ps(-0.f)), _mm_set1_ps(-FLT_EPSILON), _CMP_LT_OQ)) |
			_mm_movemask_ps(_mm_cmp_ps(_mm_or_ps(_mm_sub_ps(data[1], m.data[1]), _mm_set1_ps(-0.f)), _mm_set1_ps(-FLT_EPSILON), _CMP_LT_OQ)) |
			_mm_movemask_ps(_mm_cmp_ps(_mm_or_ps(_mm_sub_ps(data[2], m.data[2]), _mm_set1_ps(-0.f)), _mm_set1_ps(-FLT_EPSILON), _CMP_LT_OQ)) |
			_mm_movemask_ps(_mm_cmp_ps(_mm_or_ps(_mm_sub_ps(data[3], m.data[3]), _mm_set1_ps(-0.f)), _mm_set1_ps(-FLT_EPSILON), _CMP_LT_OQ));
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

	__m128_getter _det_ = {det};
	return _det_._a_[3];
}

template<>
Mat4<float32> Mat4<float32>::operator+(const float32 s) const
{
	const __m128 v = _mm_set1_ps(s);
	__m128 new_data[4] = {
		_mm_add_ps(data[0], v),
		_mm_add_ps(data[1], v),
		_mm_add_ps(data[2], v),
		_mm_add_ps(data[3], v)
	};
	return Mat4<float32>(new_data);
}

template<>
Mat4<float32> Mat4<float32>::operator-(const float32 s) const
{
	const __m128 v = _mm_set1_ps(s);
	__m128 new_data[4] = {
		_mm_sub_ps(data[0], v),
		_mm_sub_ps(data[1], v),
		_mm_sub_ps(data[2], v),
		_mm_sub_ps(data[3], v)
	};
	return Mat4<float32>(new_data);
}

template<>
Mat4<float32> Mat4<float32>::operator*(const float32 s) const
{
	const __m128 v = _mm_set1_ps(s);
	__m128 new_data[4] = {
		_mm_mul_ps(data[0], v),
		_mm_mul_ps(data[1], v),
		_mm_mul_ps(data[2], v),
		_mm_mul_ps(data[3], v)
	};
	return Mat4<float32>(new_data);
}

template<>
Mat4<float32> Mat4<float32>::operator/(const float32 s) const
{
	const __m128 v = _mm_set1_ps(s);
	__m128 new_data[4] = {
		_mm_div_ps(data[0], v),
		_mm_div_ps(data[1], v),
		_mm_div_ps(data[2], v),
		_mm_div_ps(data[3], v)
	};
	return Mat4<float32>(new_data);
}

template<>
inline Mat4<float32> operator+(const Vec4<float32> & v, const Mat4<float32> & m)
{
	__m128 new_data[4] = {
		_mm_add_ps(v.data, m.data[0]),
		_mm_add_ps(v.data, m.data[1]),
		_mm_add_ps(v.data, m.data[2]),
		_mm_add_ps(v.data, m.data[3])
	};
	return Mat4<float32>(new_data);
}

template<>
inline Mat4<float32> operator-(const Vec4<float32> & v, const Mat4<float32> & m)
{
	__m128 new_data[4] = {
		_mm_sub_ps(v.data, m.data[0]),
		_mm_sub_ps(v.data, m.data[1]),
		_mm_sub_ps(v.data, m.data[2]),
		_mm_sub_ps(v.data, m.data[3])
	};
	return Mat4<float32>(new_data);
}

template<>
inline Mat4<float32> operator*(const Vec4<float32> & v, const Mat4<float32> & m)
{
	__m128 new_data[4] = {
		_mm_mul_ps(v.data, m.data[0]),
		_mm_mul_ps(v.data, m.data[1]),
		_mm_mul_ps(v.data, m.data[2]),
		_mm_mul_ps(v.data, m.data[3])
	};
	return Mat4<float32>(new_data);
}

template<>
inline Mat4<float32> operator/(const Vec4<float32> & v, const Mat4<float32> & m)
{
	__m128 new_data[4] = {
		_mm_div_ps(v.data, m.data[0]),
		_mm_div_ps(v.data, m.data[1]),
		_mm_div_ps(v.data, m.data[2]),
		_mm_div_ps(v.data, m.data[3])
	};
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
 * @todo Probably there's a better way, don't know
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
	// Get SIMD data
	const __m128 v_data = _mm_shuffle_ps(v.data, _mm_set1_ps(1.f), _MM_SHUFFLE(3, 2, 1, 0));
	__m128 x, y;

	x = _mm_mul_ps(data[0], v_data);
	x = _mm_hadd_ps(x, x);
	x = _mm_hadd_ps(x, x);
	
	y = _mm_mul_ps(data[1], v_data);
	y = _mm_hadd_ps(y, y);
	y = _mm_hadd_ps(y, y);

	return Vec2<float32>(_mm_unpackhi_ps(y, x));
}

template<>
vec3 Mat4<float32>::operator*(const vec3 & v) const
{
	// Get SIMD data
	const __m128 v_data = _mm_move_ss(v.data, _mm_set1_ps(1.f));
	__m128 x, y, z, zero = _mm_set1_ps(0.f);
	
	x = _mm_mul_ps(data[0], v_data);
	x = _mm_hadd_ps(x, x);
	x = _mm_hadd_ps(x, x);
	
	y = _mm_mul_ps(data[1], v_data);
	y = _mm_hadd_ps(y, y);
	y = _mm_hadd_ps(y, y);
	
	z = _mm_mul_ps(data[2], v_data);
	z = _mm_hadd_ps(z, z);
	z = _mm_hadd_ps(z, z);

	return Vec3<float32>(_mm_movehl_ps(_mm_unpackhi_ps(y, x), _mm_unpackhi_ps(zero, z)));
}

template<>
vec4 Mat4<float32>::operator*(const vec4 & v) const
{
	// Get SIMD data
	const __m128 v_data = v.data;
	__m128 x, y, z, w;
	
	x = _mm_mul_ps(data[0], v_data);
	x = _mm_hadd_ps(x, x);
	x = _mm_hadd_ps(x, x);
	
	y = _mm_mul_ps(data[1], v_data);
	y = _mm_hadd_ps(y, y);
	y = _mm_hadd_ps(y, y);
	
	z = _mm_mul_ps(data[2], v_data);
	z = _mm_hadd_ps(z, z);
	z = _mm_hadd_ps(z, z);
	
	w = _mm_mul_ps(data[3], v_data);
	w = _mm_hadd_ps(w, w);
	w = _mm_hadd_ps(w, w);

	return Vec4<float32>(_mm_movehl_ps(_mm_unpackhi_ps(y, x), _mm_unpackhi_ps(w, z)));
}

/////////////////////////////////////////////////
// Static constructors                         //
/////////////////////////////////////////////////

template<>
Mat4<float32> Mat4<float32>::fill(const float32 s)
{
	// Eye matrix
	const __m128 row = _mm_set1_ps(s);
	const __m128 data[4] = {
		row,
		row,
		row,
		row
	};
	return Mat4<float32>(data);
}

template<>
Mat4<float32> Mat4<float32>::eye(const float32 s)
{
	// Eye matrix
	const __m128 data[4] = {
		_mm_set_ps(s, 0.f, 0.f, 0.f),
		_mm_set_ps(0.f, s, 0.f, 0.f),
		_mm_set_ps(0.f, 0.f, s, 0.f),
		_mm_set_ps(0.f, 0.f, 0.f, s),
	};
	return Mat4<float32>(data);
}

template<>
Mat4<float32> Mat4<float32>::diag(const Vec4<float32> & d)
{
	// Eye matrix
	const __m128 data[4] = {
		_mm_set_ps(d.x, 0.f, 0.f, 0.f),
		_mm_set_ps(0.f, d.y, 0.f, 0.f),
		_mm_set_ps(0.f, 0.f, d.z, 0.f),
		_mm_set_ps(0.f, 0.f, 0.f, d.w)
	};
	return Mat4<float32>(data);
}

template<>
Mat4<float32> Mat4<float32>::diag(const float32 x, const float32 y, const float32 z, const float32 w)
{
	// Eye matrix
	const __m128 data[4] = {
		_mm_set_ps(x, 0.f, 0.f, 0.f),
		_mm_set_ps(0.f, y, 0.f, 0.f),
		_mm_set_ps(0.f, 0.f, z, 0.f),
		_mm_set_ps(0.f, 0.f, 0.f, w)
	};
	return Mat4<float32>(data);
}

template<>
Mat4<float32> Mat4<float32>::translation(const Vec3<float32> & v)
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
Mat4<float32> Mat4<float32>::translation(const float32 x, const float32 y, const float32 z)
{
	// Translation matrix
	const __m128 data[4] = {
		_mm_set_ps(1.f, 0.f, 0.f, x),
		_mm_set_ps(0.f, 1.f, 0.f, y),
		_mm_set_ps(0.f, 0.f, 1.f, z),
		_mm_set_ps(0.f, 0.f, 0.f, 1.f)
	};
	return Mat4<float32>(data);
}

template<>
Mat4<float32> Mat4<float32>::scale(const Vec3<float32> & v)
{
	// Scale matrix
	const __m128 data[4] = {
		_mm_set_ps(v.x, 0.f, 0.f, 0.f),
		_mm_set_ps(0.f, v.y, 0.f, 0.f),
		_mm_set_ps(0.f, 0.f, v.z, 0.f),
		_mm_set_ps(0.f, 0.f, 0.f, 1.f)
	};
	return Mat4<float32>(data);
}

template<>
Mat4<float32> Mat4<float32>::scale(const float32 s)
{
	// Scale matrix
	const __m128 data[4] = {
		_mm_set_ps(s, 0.f, 0.f, 0.f),
		_mm_set_ps(0.f, s, 0.f, 0.f),
		_mm_set_ps(0.f, 0.f, s, 0.f),
		_mm_set_ps(0.f, 0.f, 0.f, 1.f)
	};
	return Mat4<float32>(data);
}

template<>
Mat4<float32> Mat4<float32>::rotation(const Quat<float32> & q)
{
	// Scale matrix
	__m128 data[4];

	#define d q.data
	#define set(x, y, z, w) _mm_set_ps(x, y, z, w)
	#define shuffle(v, x, y, z, w) _mm_shuffle_ps(v, v, _MM_SHUFFLE(x, y, z, w))
	#define mul(v1, v2) _mm_mul_ps(v1, v2)
	#define add(v1, v2) _mm_add_ps(v1, v2)
	#define sgn(v1, v2) _mm_xor_ps(v1, v2)

	// First row
	data[0] = add(
		set(1.f, 0.f, 0.f, 0.f),
		mul(
			set(-2.f, 2.f, 2.f, 0.f),
			add(
				mul(
					shuffle(d, 2, 3, 3, 0),
					shuffle(d, 2, 2, 1, 0)
				),
				sgn(
					set(0.f, -0.f, 0.f, 0.f),
					mul(
						shuffle(d, 1, 1, 2, 0),
						shuffle(d, 1, 0, 0, 0)
					)
				)
			)
		)
	);

	// Second row
	data[1] = add(
		set(0.f, 1.f, 0.f, 0.f),
		mul(
			set(2.f, -2.f, 2.f, 0.f),
			add(
				mul(
					shuffle(d, 3, 3, 2, 0),
					shuffle(d, 2, 3, 1, 0)
				),
				sgn(
					set(0.f, -0.f, -0.f, 0.f),
					mul(
						shuffle(d, 1, 1, 3, 0),
						shuffle(d, 0, 1, 0, 0)
					)
				)
			)
		)
	);

	// Third row
	data[2] = add(
		set(0.f, 0.f, 1.f, 0.f),
		mul(
			set(2.f, 2.f, -2.f, 0.f),
			add(
				mul(
					shuffle(d, 3, 2, 3, 0),
					shuffle(d, 1, 1, 3, 0)
				),
				sgn(
					set(-0.f, 0.f, 0.f, 0.f),
					mul(
						shuffle(d, 2, 3, 2, 0),
						shuffle(d, 0, 0, 2, 0)
					)
				)
			)
		)
	);

	// Fourth row
	data[3] = set(0.f, 0.f, 0.f, 1.f);

	#undef d
	#undef set
	#undef shuffle
	#undef mul
	#undef add
	#undef sgn

	return Mat4<float32>(data);
}

template<>
Mat4<float32> Mat4<float32>::rotation(const Vec3<float32> & v, const float32 a)
{
	/**
	 * @todo May be a little faster to compute directly from @c v and @c a
	 */
	return Mat4<float32>::rotation(Quat<float32>(v, a));
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