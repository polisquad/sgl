#ifndef SGL_MATH_H
#define SGL_MATH_H

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "quat.h"
#include "mat4.h"

/**
 * @brief Pi alias
 */
#define PI M_PI

/**
 * @brief Angle conversion macros
 * @{
 */
#define RAD(alpha) alpha * PI / 180.f
#define DEG(alpha) alpha * 180.f / PI
/** @} */

/**
 * @struct Math math.h
 * @brief A set of math functions
 */
struct Math
{
	/**
	 * @brief Return delta angle (in radians)
	 * 
	 * @param [in] a,b angles values (in radians)
	 * 
	 * @return min delta angle
	 */
	template<typename T> inline static T findDeltaAngle(T a, T b);

	/**
	 * @brief Flip angle if out of bounds [-PI;PI]
	 * 
	 * @param [in] a angle
	 * 
	 * @return adjusted angle
	 */
	template<typename T> inline static T unwindAngle(T a);

	/**
	 * @brief compute min[max] vector element-wise
	 * 
	 * @param [in] v1,v2 two vectors
	 * 
	 * @return vector with min[max] elements
	 * @{
	 */
	template<typename T> inline static Vec2<T> min(const Vec2<T> & v1, const Vec2<T> & v2);
	template<typename T> inline static Vec3<T> min(const Vec3<T> & v1, const Vec3<T> & v2);
	template<typename T> inline static Vec4<T> min(const Vec4<T> & v1, const Vec4<T> & v2);

	template<typename T> inline static Vec2<T> max(const Vec2<T> & v1, const Vec2<T> & v2);
	template<typename T> inline static Vec3<T> max(const Vec3<T> & v1, const Vec3<T> & v2);
	template<typename T> inline static Vec4<T> max(const Vec4<T> & v1, const Vec4<T> & v2);
	/** @} */

	/**
	 * @brief Linear interpolation (templated)
	 * 
	 * @param [in]	a,b		values to interpolate
	 * @param [in]	alpha	interpolation value
	 * 
	 * @return interpolated value
	 */
	template<typename T, typename U> inline static T lerp(const T & a, const T & b, U alpha);

	/**
	 * @brief Perform a 2D lerp
	 * 
	 * @param [in]	a,b,c,d	values to interpolate
	 * @param [in]	a1,a2	interpolation steps
	 */
	template<typename T, typename U> inline static T bilerp(const T & a, const T & b, const T & c, const T & d, U a1, U a2);

	/**
	 * @brief Return reflected vector
	 * 
	 * @param [in]	v	bounced vector
	 * @param [in]	n	surface normal
	 * 
	 * @return reflected vector
	 */
	template<typename T> inline static Vec3<T> getReflectedVector(const Vec3<T> & v, const Vec3<T> & n);

	/**
	 * @brief Return intersection between ray
	 * (origin, direction) and plane
	 * @details Assumes ray and plane not parallel
	 * 
	 * @param [in]	ro,rd	origin and direction of the ray
	 * @param [in]	po,pn	origin and normal of the plane
	 * 
	 * @return point of intersection
	 */
	template<typename T> inline static Vec3<T> rayPlaneIntersect(const Vec3<T> & ro, const Vec3<T> & rd, const Vec3<T> & po, const Vec3<T> & pn);

	/**
	 * @brief Return intersection between line
	 * (p0, p1) and plane
	 * 
	 * @param [in]	p0,p1	two points defining the line
	 * @param [in]	po, pn	origin and normal of the plane
	 * 
	 * @return point of intersection
	 */
	template<typename T> inline static Vec3<T> linePlaneIntersect(const Vec3<T> & p0, const Vec3<T> & p1, const Vec3<T> & po, const Vec3<T> & pn);

	/**
	 * @brief Intersect segment with plane
	 * 
	 * @param [in]	p0,p1	segment vertices
	 * @param [in]	po,pn	origin and normal of the plane
	 * @param [out]	hit		intersection point
	 * 
	 * @return @c true if intersect, @c false otherwise
	 */
	template<typename T> inline static bool intersect(const Vec3<T> & p0, const Vec3<T> & p1, const Vec3<T> & po, const Vec3<T> & pn, Vec3<T> & hit);
};

template<typename T>
T Math::findDeltaAngle(T a, T b)
{
	T delta = b - a;

	// Flip angle
	if (delta > PI)
		delta -= T(2) * PI;
	else if (delta < -PI)
		delta += T(2) * PI;
	
	return delta;
}

template<typename T>
T Math::unwindAngle(T a)
{
	while (a > PI)
		a -= T(2) * PI;
	while (a < -PI)
		a += T(2) * PI;
	
	return a;
}

template<typename T>
Vec2<T> Math::min(const Vec2<T> & v1, const Vec2<T> & v2)
{
	return Vec2<T>(
		v1.x < v2.x ? v1.x : v2.x,
		v1.y < v2.y ? v1.y : v2.y
	);
}

template<typename T>
Vec3<T> Math::min(const Vec3<T> & v1, const Vec3<T> & v2)
{
	return Vec3<T>(
		v1.x < v2.x ? v1.x : v2.x,
		v1.y < v2.y ? v1.y : v2.y,
		v1.z < v2.z ? v1.z : v2.z
	);
}

template<typename T>
Vec4<T> Math::min(const Vec4<T> & v1, const Vec4<T> & v2)
{
	return Vec4<T>(
		v1.x < v2.x ? v1.x : v2.x,
		v1.y < v2.y ? v1.y : v2.y,
		v1.z < v2.z ? v1.z : v2.z,
		v1.w < v2.w ? v1.w : v2.w
	);
}

template<typename T>
Vec2<T> Math::max(const Vec2<T> & v1, const Vec2<T> & v2)
{
	return Vec2<T>(
		v1.x > v2.x ? v1.x : v2.x,
		v1.y > v2.y ? v1.y : v2.y
	);
}

template<typename T>
Vec3<T> Math::max(const Vec3<T> & v1, const Vec3<T> & v2)
{
	return Vec3<T>(
		v1.x > v2.x ? v1.x : v2.x,
		v1.y > v2.y ? v1.y : v2.y,
		v1.z > v2.z ? v1.z : v2.z
	);
}

template<typename T>
Vec4<T> Math::max(const Vec4<T> & v1, const Vec4<T> & v2)
{
	return Vec4<T>(
		v1.x > v2.x ? v1.x : v2.x,
		v1.y > v2.y ? v1.y : v2.y,
		v1.z > v2.z ? v1.z : v2.z,
		v1.w > v2.w ? v1.w : v2.w
	);
}

template<typename T, typename U>
T Math::lerp(const T & a, const T & b, U alpha)
{
	return a + (b - a) * alpha;
}

template<typename T, typename U>
T Math::bilerp(const T & a, const T & b, const T & c, const T & d, U a1, U a2)
{
	return lerp(
		lerp(a, b, a1),
		lerp(c, d, a1),
		a2
	);
}

template<typename T>
Vec3<T> Math::getReflectedVector(const Vec3<T> & v, const Vec3<T> & n)
{
	// Normalize surface normal
	const Vec3<T> normal = n.getNormal();

	return  v - (T(2) * (normal & v) * normal);
}

template<typename T>
Vec3<T> Math::rayPlaneIntersect(const Vec3<T> & ro, const Vec3<T> & rd, const Vec3<T> & po, const Vec3<T> & pn)
{
	/** @see https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection */
	return ((po - ro) & pn) / (rd & pn) * rd + ro;
}

template<typename T>
Vec3<T> Math::linePlaneIntersect(const Vec3<T> & p0, const Vec3<T> & p1, const Vec3<T> & po, const Vec3<T> & pn)
{
	return rayPlaneIntersect(p0, p1 - p0, po, pn);
}

template<typename T>
bool Math::intersect(const Vec3<T> & p0, const Vec3<T> & p1, const Vec3<T> & po, const Vec3<T> & pn, Vec3<T> & hit)
{
	const Vec3<T> pd = (p1 - p0);
	const T d = ((po - p0) & pn) / (pd & pn);

	// If point is outside segment, return false
	if (d < T(0) || d > T(1)) return false;

	// Calc hit point and return true
	hit = d * pd + p0;
	return true;
}

/////////////////////////////////////////////////
// Float 32-bit specialization                 //
/////////////////////////////////////////////////

// No specialization for Vec2<float32>

template<>
Vec3<float32> Math::min(const Vec3<float32> & v1, const Vec3<float32> & v2)
{
	return Vec3<float32>(_mm_min_ps(v1.data, v2.data));
}

template<>
Vec4<float32> Math::min(const Vec4<float32> & v1, const Vec4<float32> & v2)
{
	return Vec4<float32>(_mm_min_ps(v1.data, v2.data));
}

template<>
Vec3<float32> Math::max(const Vec3<float32> & v1, const Vec3<float32> & v2)
{
	return Vec3<float32>(_mm_max_ps(v1.data, v2.data));
}

template<>
Vec4<float32> Math::max(const Vec4<float32> & v1, const Vec4<float32> & v2)
{
	return Vec4<float32>(_mm_max_ps(v1.data, v2.data));
}

template<>
Vec3<float32> Math::rayPlaneIntersect(const Vec3<float32> & ro, const Vec3<float32> & rd, const Vec3<float32> & po, const Vec3<float32> & pn)
{
	// Create bit mask for floats
	float32 _nan; memset(&_nan, 0xffffffff, sizeof(float32));
	const __m128 mask = _mm_set_ps(_nan, _nan, _nan, 0.f);
	
	__m128	a = _mm_mul_ps(_mm_sub_ps(po.data, ro.data), pn.data);
			a = _mm_and_ps(a, mask);
			a = _mm_hadd_ps(a, a);
			a = _mm_hadd_ps(a, a);
	__m128	b = _mm_mul_ps(rd.data, pn.data);
			b = _mm_and_ps(b, mask);
			b = _mm_hadd_ps(b, b);
			b = _mm_hadd_ps(b, b);
	__m128	d = _mm_div_ps(a, b);

	return Vec3<float32>(_mm_add_ps(
		_mm_mul_ps(d, rd.data),
		ro.data
	));
}

template<>
bool Math::intersect(const Vec3<float32> & p0, const Vec3<float32> & p1, const Vec3<float32> & po, const Vec3<float32> & pn, Vec3<float32> & hit)
{
	// Create bit mask for floats
	float32 _nan; memset(&_nan, 0xffffffff, sizeof(float32));
	const __m128 mask = _mm_set_ps(_nan, _nan, _nan, 0.f);

	// Line direction
	const __m128 ld = _mm_sub_ps(p1.data, p0.data);
	
	__m128	a = _mm_mul_ps(_mm_sub_ps(po.data, p0.data), pn.data);
			a = _mm_and_ps(a, mask);
			a = _mm_hadd_ps(a, a);
			a = _mm_hadd_ps(a, a);
	__m128	b = _mm_mul_ps(ld, pn.data);
			b = _mm_and_ps(b, mask);
			b = _mm_hadd_ps(b, b);
			b = _mm_hadd_ps(b, b);
	__m128	d = _mm_div_ps(a, b);

	float32 _d[4]; _mm_store1_ps(_d, d);
	if (*_d < 0.f || *_d > 0.f) return false;

	// Calc hit point
	hit.data = _mm_add_ps(
		_mm_mul_ps(d, ld),
		p0.data
	);

	return true;
}

#endif