#pragma once

#include "core_types.h"
#include "templates/is_void.h"
#include "templates/simd.h"

//////////////////////////////////////////////////
// Forward declarations
//////////////////////////////////////////////////

template<typename>																struct Vec2;
template<typename, bool>	struct Vec3;
template<typename, bool>	struct Vec4;
template<typename>																struct Quat;
template<typename, bool>	struct Mat3;
template<typename, bool>	struct Mat4;
template<typename>																struct Box;

//////////////////////////////////////////////////
// Typedefs for common types
//////////////////////////////////////////////////

#define hasVectorIntrinsics(T) !IsVoid<typename Simd::Vector<T, 4>::Type>::value

using vec2 = Vec2<float32>;
using vec3 = Vec3<float32, hasVectorIntrinsics(float32)>;
using vec4 = Vec4<float32, hasVectorIntrinsics(float32)>;
using mat3 = Mat3<float32, hasVectorIntrinsics(float32)>;
using mat4 = Mat4<float32, hasVectorIntrinsics(float32)>;

using ivec2 = Vec2<int32>;
using ivec3 = Vec3<int32, hasVectorIntrinsics(int32)>;
using ivec4 = Vec4<int32, hasVectorIntrinsics(int32)>;
using imat3 = Mat3<int32, hasVectorIntrinsics(int32)>;
using imat4 = Mat4<int32, hasVectorIntrinsics(int32)>;

using uvec2 = Vec2<uint32>;
using uvec3 = Vec3<uint32, hasVectorIntrinsics(uint32)>;
using uvec4 = Vec4<uint32, hasVectorIntrinsics(uint32)>;
using umat3 = Mat3<uint32, hasVectorIntrinsics(uint32)>;
using umat4 = Mat4<uint32, hasVectorIntrinsics(uint32)>;

using point2 = ivec2;
using point3 = ivec3;
using point4 = ivec4;

using dim2 = uvec2;
using dim3 = uvec3;
using dim4 = uvec4;