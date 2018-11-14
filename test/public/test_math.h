#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/quat.h"
#include "math/mat4.h"

#include <gtest/gtest.h>

/**
 * @brief Register tests
 * @{
 */

TEST(Math, v2f_component_x)	{ ASSERT_FLOAT_EQ(Vec2<>(1.f).x, 1.f); }
TEST(Math, v2f_component_y)	{ ASSERT_FLOAT_EQ(Vec2<>(1.f).y, 1.f); }
TEST(Math, v2f_sq_size)		{ ASSERT_FLOAT_EQ(Vec2<>(1.f).getSquaredSize(), 2.f); }
TEST(Math, v2f_size)		{ ASSERT_FLOAT_EQ(Vec2<>(1.f).getSize(), sqrtf(2.f)); }
TEST(Math, v2f_eq)			{ ASSERT_EQ(Vec2<>(0.f) == Vec2<>(0.f), true); }
TEST(Math, v2f_neq)			{ ASSERT_EQ(Vec2<>(0.f) != Vec2<>(0.f), false); }
TEST(Math, v2f_norm)		{ ASSERT_EQ(Vec2<>(3.f, 4.f).getNormal().getSquaredSize(), 1.f); }
TEST(Math, v2f_add_v2f)		{ ASSERT_EQ(Vec2<>(2.f) + Vec2<>(1.f), Vec2<>(3.f)); }
TEST(Math, v2f_sub_v2f)		{ ASSERT_EQ(Vec2<>(2.f) - Vec2<>(1.f), Vec2<>(1.f)); }
TEST(Math, v2f_mul_v2f)		{ ASSERT_EQ(Vec2<>(2.f) * Vec2<>(2.f), Vec2<>(4.f)); }
TEST(Math, v2f_div_v2f)		{ ASSERT_EQ(Vec2<>(4.f) / Vec2<>(2.f), Vec2<>(2.f)); }
TEST(Math, v2f_add_f)		{ ASSERT_EQ(Vec2<>(2.f) + 1.f, Vec2<>(3.f)); }
TEST(Math, v2f_sub_f)		{ ASSERT_EQ(Vec2<>(2.f) - 1.f, Vec2<>(1.f)); }
TEST(Math, v2f_mul_f)		{ ASSERT_EQ(Vec2<>(2.f) * 2.f, Vec2<>(4.f)); }
TEST(Math, v2f_div_f)		{ ASSERT_EQ(Vec2<>(4.f) / 2.f, Vec2<>(2.f)); }
TEST(Math, v2f_dot)			{ ASSERT_FLOAT_EQ(Vec2<>(1.f, 0.f) & Vec2<>(1.f, 0.f), 1.f); }
TEST(Math, v2f_cross)		{ ASSERT_EQ(Vec2<>(1.f, 0.f) ^ Vec2<>(0.f, 1.f), 1.f); }

/////////////////////////////////////////////////
// Vec2 tests                                  //
/////////////////////////////////////////////////

TEST(Math, v3f_component_x)	{ ASSERT_FLOAT_EQ(Vec3<>(1.f).x, 1.f); }
TEST(Math, v3f_component_y)	{ ASSERT_FLOAT_EQ(Vec3<>(1.f).y, 1.f); }
TEST(Math, v3f_component_z)	{ ASSERT_FLOAT_EQ(Vec3<>(1.f).z, 1.f); }
TEST(Math, v3f_sq_size)		{ ASSERT_FLOAT_EQ(Vec3<>(1.f).getSquaredSize(), 3.f); }
TEST(Math, v3f_size)		{ ASSERT_FLOAT_EQ(Vec3<>(1.f).getSize(), sqrtf(3.f)); }
TEST(Math, v3f_eq)			{ ASSERT_EQ(Vec3<>(0.f) == Vec3<>(0.f), true); }
TEST(Math, v3f_neq)			{ ASSERT_EQ(Vec3<>(0.f) != Vec3<>(0.f), false); }
TEST(Math, v3f_norm)		{ ASSERT_EQ(Vec3<>(3.f, 4.f, 0.f).getNormal().getSquaredSize(), 1.f); }
TEST(Math, v3f_add_v3f)		{ ASSERT_EQ(Vec3<>(2.f) + Vec3<>(1.f), Vec3<>(3.f)); }
TEST(Math, v3f_sub_v3f)		{ ASSERT_EQ(Vec3<>(2.f) - Vec3<>(1.f), Vec3<>(1.f)); }
TEST(Math, v3f_mul_v3f)		{ ASSERT_EQ(Vec3<>(2.f) * Vec3<>(2.f), Vec3<>(4.f)); }
TEST(Math, v3f_div_v3f)		{ ASSERT_EQ(Vec3<>(4.f) / Vec3<>(2.f), Vec3<>(2.f)); }
TEST(Math, v3f_add_f)		{ ASSERT_EQ(Vec3<>(2.f) + 1.f, Vec3<>(3.f)); }
TEST(Math, v3f_sub_f)		{ ASSERT_EQ(Vec3<>(2.f) - 1.f, Vec3<>(1.f)); }
TEST(Math, v3f_mul_f)		{ ASSERT_EQ(Vec3<>(2.f) * 2.f, Vec3<>(4.f)); }
TEST(Math, v3f_div_f)		{ ASSERT_EQ(Vec3<>(4.f) / 2.f, Vec3<>(2.f)); }
TEST(Math, v3f_dot)			{ ASSERT_FLOAT_EQ(Vec3<>(1.f, 0.f, 0.f) & Vec3<>(1.f, 0.f, 0.f), 1.f); }
TEST(Math, v3f_cross)		{ ASSERT_EQ(Vec3<>(1.f, 0.f, 0.f) ^ Vec3<>(0.f, 1.f, 0.f), Vec3<>(0.f, 0.f, 1.f)); }

/////////////////////////////////////////////////
// Vec4 tests                                  //
/////////////////////////////////////////////////

TEST(Math, v4f_component_x)	{ ASSERT_FLOAT_EQ(Vec4<>(1.f).x, 1.f); }
TEST(Math, v4f_component_y)	{ ASSERT_FLOAT_EQ(Vec4<>(1.f).y, 1.f); }
TEST(Math, v4f_component_z)	{ ASSERT_FLOAT_EQ(Vec4<>(1.f).z, 1.f); }
TEST(Math, v4f_component_w)	{ ASSERT_FLOAT_EQ(Vec4<>(1.f).w, 1.f); }
TEST(Math, v4f_sq_size)		{ ASSERT_FLOAT_EQ(Vec4<>(1.f).getSquaredSize(), 4.f); }
TEST(Math, v4f_size)		{ ASSERT_FLOAT_EQ(Vec4<>(1.f).getSize(), sqrtf(4.f)); }
TEST(Math, v4f_eq)			{ ASSERT_EQ(Vec4<>(0.f) == Vec4<>(0.f), true); }
TEST(Math, v4f_neq)			{ ASSERT_EQ(Vec4<>(0.f) != Vec4<>(0.f), false); }
TEST(Math, v4f_norm)		{ ASSERT_EQ(Vec4<>(6.f, 8.f, 0.f, 0.f).getNormal().getSquaredSize(), 1.f); }
TEST(Math, v4f_add_v4f)		{ ASSERT_EQ(Vec4<>(2.f) + Vec4<>(1.f), Vec4<>(3.f)); }
TEST(Math, v4f_sub_v4f)		{ ASSERT_EQ(Vec4<>(2.f) - Vec4<>(1.f), Vec4<>(1.f)); }
TEST(Math, v4f_mul_v4f)		{ ASSERT_EQ(Vec4<>(2.f) * Vec4<>(2.f), Vec4<>(4.f)); }
TEST(Math, v4f_div_v4f)		{ ASSERT_EQ(Vec4<>(4.f) / Vec4<>(2.f), Vec4<>(2.f)); }
TEST(Math, v4f_add_f)		{ ASSERT_EQ(Vec4<>(2.f) + 1.f, Vec4<>(3.f)); }
TEST(Math, v4f_sub_f)		{ ASSERT_EQ(Vec4<>(2.f) - 1.f, Vec4<>(1.f)); }
TEST(Math, v4f_mul_f)		{ ASSERT_EQ(Vec4<>(2.f) * 2.f, Vec4<>(4.f)); }
TEST(Math, v4f_div_f)		{ ASSERT_EQ(Vec4<>(4.f) / 2.f, Vec4<>(2.f)); }
TEST(Math, v4f_dot)			{ ASSERT_FLOAT_EQ(Vec4<>(1.f, 0.f, 0.f, 0.f) & Vec4<>(1.f, 0.f, 0.f, 0.f), 1.f); }

/////////////////////////////////////////////////
// Quat tests                                  //
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Mat4 tests                                  //
/////////////////////////////////////////////////

TEST(Math, m4f_eq)			{ ASSERT_EQ(mat4::translation(vec3(1.f)) == mat4::translation(vec3(1.f)), true); }
TEST(Math, m4f_neq)			{ ASSERT_EQ(mat4::translation(vec3(1.f)) != mat4::translation(vec3(1.f)), false); }
TEST(Math, m4f_mul_m4f)		{ ASSERT_EQ(mat4::translation(vec3(1.f)).getTranspose(), mat4(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f)); }
TEST(Math, m4f_transpose)	{ ASSERT_EQ(mat4::translation(vec3(1.f)) * mat4::translation(vec3(1.f)), mat4::translation(vec3(2.f))); }
TEST(Math, m4f_inverse)		{ ASSERT_EQ(!mat4(2.f, 1.f, 0.f, 1.f, 3.f, 1.f, 1.f, 1.f, 2.f, 0.f, 3.f, 1.f, 0.f, 0.f, 0.f, 1.f), mat4(-3.f, 3.f, -1.f, 1.f, 7.f, -6.f, 2.f, -3.f, 2.f, -2.f, 1.f, -1.f, 0.f, -0.f, 0.f, 1.f)); }
TEST(Math, m4f_mul_v2f)		{ ASSERT_EQ(mat4::translation(vec3(1.f)) * vec2(1.f), vec2(2.f)); }
TEST(Math, m4f_mul_v3f)		{ ASSERT_EQ(mat4::translation(vec3(1.f)) * vec3(1.f), vec3(2.f)); }
TEST(Math, m4f_mul_v4f)		{ ASSERT_EQ(mat4::translation(vec3(1.f)) * vec4(1.f), vec4(2.f)); }

/**
 * @}
 */