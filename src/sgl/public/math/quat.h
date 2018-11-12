#ifndef SGL_QUAT_H
#define SGL_QUAT_H

#include "vec4.h"

/**
 * @struct Quat quat.h
 * @brief Quaternion implementation (used for rotation)
 */
struct Quat
{
protected:
	/**
	 * @brief Quat components
	 * @{
	 */
	float32 x, y, z, w;
	/** @} */

public:
	/**
	 * @brief Default constructor
	 */
	inline Quat();

	/**
	 * @brief Components-constructor
	 * 
	 * @param [in] x,y,z,w
	 */
	inline Quat(float32 x, float32 y, float32 z, float32 w);

	/**
	 * @brief Axis-rotation-constructor
	 * 
	 * @param [in]	v	rotation axis
	 * @param [in]	a	rotation angle
	 */
	inline Quat(const vec3 & v, float32 a);

	/**
	 * @brief Quaternion squared size
	 * 
	 * @return quaternion squared size
	 */
	inline float32 getSquaredSize() const;

	/**
	 * @brief Quaternion size
	 * 
	 * @return quaternion size (L^2-norm)
	 */
	inline float32 getSize() const;

	/**
	 * @brief Return normalized copy of quaternion
	 * 
	 * @return normalized quat
	 */
	inline Quat getNormal() const;

	/**
	 * @brief Normalize in-place
	 * 
	 * @return self
	 */
	inline Quat & normalize();

	/**
	 * @brief Comparison operators
	 * @details Compare two unit quaternion.
	 * If the quaternions are not normalized
	 * result is not well defined
	 * 
	 * @param [in] q other quat
	 * 
	 * @return comparison result as boolean
	 * 
	 * @{
	 */
	inline bool operator==(const Quat & q) const;
	inline bool operator!=(const Quat & q) const;
	/** @} */

	/**
	 * @brief Invert quaternion components
	 * 
	 * @return Copy with inverted components
	 */
	inline Quat operator-() const;

	/**
	 * @brief Quaternion multiplication
	 * @details Combines two rotation.
	 * Note that this operation is not commutative
	 * 
	 * @param [in] q other quaternion
	 */
	inline Quat operator*(const Quat & q) const;

	/**
	 * @brief Quat-Vec3 multiplication
	 * @details Semantically, rotates a vector
	 * by this quaternion
	 * 
	 * @param [in] v vector to multiply
	 */
	inline vec3 operator*(const vec3 & v) const;

	/**
	 * @brief Quat to direction vectors conversion
	 * 
	 * @return direction vector
	 * 
	 * @{
	 */
	inline vec3 toForward() const;
	inline vec3 toRight() const;
	inline vec3 toUp() const;
	/** @} */

	/**
	 * @brief Print to stream
	 * 
	 * @param [in] stream output stream
	 */
	inline void print(FILE * stream = stdout);
};

Quat::Quat() : x(0.f), y(1.f), z(0.f), w(0.f) {}

Quat::Quat(float32 x, float32 y, float32 z, float32 w) : x(x), y(y), z(z), w(w) {}

Quat::Quat(const vec3 & v, float32 a)
{
	const vec3 axis = v.getNormal();
	const float32 s = sinf(a / 2.f), c = cosf(a / 2.f);
	x = s * axis.x, y = s * axis.y, z = s * axis.z;
	w = c;
}

float32 Quat::getSquaredSize() const
{
	return x * x + y * y + z * z + w * w;
}

float32 Quat::getSize() const
{
	return sqrtf(getSquaredSize());
}

Quat Quat::getNormal() const
{
	const float32 size = getSize();
	return Quat(x / size, y / size, z / size, w / size);
}

Quat & Quat::normalize()
{
	const float32 size = getSize();
	x /= size, y /= size, z /= size, w /= size;
	return *this;
}

bool Quat::operator==(const Quat & q) const
{
	return
		(x == q.x && y == q.y && z == q.z && w == q.w) ||
		(x == -q.x && y == -q.y && z == -q.z && w == -q.w);
}

bool Quat::operator!=(const Quat & q) const
{
	return
		(x != q.x || y != q.y || z != q.z || w != q.w) &&
		(x != -q.x || y != -q.y || z != -q.z || w != -q.w);
}

Quat Quat::operator-() const
{
	return Quat(-x, -y, -z, -w);
}

Quat Quat::operator*(const Quat & q) const
{
	return Quat(
		w * q.x + x * q.w + y * q.z - z * q.y,
		w * q.y - x * q.z + y * q.w + z * q.x,
		w * q.z + x * q.y - y * q.x + z * q.w,
		w * q.w - x * q.x - y * q.y - z * q.z
	).normalize();
}

vec3 Quat::operator*(const vec3 & v) const
{
	// http://people.csail.mit.edu/bkph/articles/Quaternions.pdf
	const vec3 q(x, y, z);
	const vec3 t = 2.f * (q ^ v);
	return v + (w * t) + (q ^ t);
}

vec3 Quat::toForward() const
{
	return *this * vec3::forward;
}

vec3 Quat::toRight() const
{
	return *this * vec3::right;
}

vec3 Quat::toUp() const
{
	return *this * vec3::up;
}

void Quat::print(FILE * stream)
{
	fprintf(stream, "q(%.3f, %.3f, %.3f, %.3f)\n", x, y, z, w);
}

#endif