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
TEST(Math, v2f_sq_size)		{ EXPECT_FLOAT_EQ(Vec2<>(1.f).getSquaredSize(), 2.f); }
TEST(Math, v2f_size)		{ EXPECT_FLOAT_EQ(Vec2<>(1.f).getSize(), sqrtf(2.f)); }
TEST(Math, v2f_eq)			{ EXPECT_TRUE(Vec2<>(0.f) == Vec2<>(0.f)); }
TEST(Math, v2f_neq)			{ EXPECT_FALSE(Vec2<>(0.f) != Vec2<>(0.f)); }
TEST(Math, v2f_norm)		{ EXPECT_EQ(Vec2<>(3.f, 4.f).getNormal().getSquaredSize(), 1.f); }
TEST(Math, v2f_add_v2f)		{ EXPECT_EQ(Vec2<>(2.f) + Vec2<>(1.f), Vec2<>(3.f)); }
TEST(Math, v2f_sub_v2f)		{ EXPECT_EQ(Vec2<>(2.f) - Vec2<>(1.f), Vec2<>(1.f)); }
TEST(Math, v2f_mul_v2f)		{ EXPECT_EQ(Vec2<>(2.f) * Vec2<>(2.f), Vec2<>(4.f)); }
TEST(Math, v2f_div_v2f)		{ EXPECT_EQ(Vec2<>(4.f) / Vec2<>(2.f), Vec2<>(2.f)); }
TEST(Math, v2f_add_f)		{ EXPECT_EQ(Vec2<>(2.f) + 1.f, Vec2<>(3.f)); }
TEST(Math, v2f_sub_f)		{ EXPECT_EQ(Vec2<>(2.f) - 1.f, Vec2<>(1.f)); }
TEST(Math, v2f_mul_f)		{ EXPECT_EQ(Vec2<>(2.f) * 2.f, Vec2<>(4.f)); }
TEST(Math, v2f_div_f)		{ EXPECT_EQ(Vec2<>(4.f) / 2.f, Vec2<>(2.f)); }
TEST(Math, v2f_dot)			{ EXPECT_FLOAT_EQ(Vec2<>(1.f, 0.f) & Vec2<>(1.f, 0.f), 1.f); }
TEST(Math, v2f_cross)		{ EXPECT_EQ(Vec2<>(1.f, 0.f) ^ Vec2<>(0.f, 1.f), 1.f); }

/////////////////////////////////////////////////
// Vec2 tests                                  //
/////////////////////////////////////////////////

TEST(Math, v3f_component_x)	{ ASSERT_FLOAT_EQ(Vec3<>(1.f).x, 1.f); }
TEST(Math, v3f_component_y)	{ ASSERT_FLOAT_EQ(Vec3<>(1.f).y, 1.f); }
TEST(Math, v3f_component_z)	{ ASSERT_FLOAT_EQ(Vec3<>(1.f).z, 1.f); }
TEST(Math, v3f_sq_size)		{ EXPECT_FLOAT_EQ(Vec3<>(1.f).getSquaredSize(), 3.f); }
TEST(Math, v3f_size)		{ EXPECT_FLOAT_EQ(Vec3<>(1.f).getSize(), sqrtf(3.f)); }
TEST(Math, v3f_eq)			{ EXPECT_TRUE(Vec3<>(0.f) == Vec3<>(0.f)); }
TEST(Math, v3f_neq)			{ EXPECT_FALSE(Vec3<>(0.f) != Vec3<>(0.f)); }
TEST(Math, v3f_norm)		{ EXPECT_EQ(Vec3<>(3.f, 4.f, 0.f).getNormal().getSquaredSize(), 1.f); }
TEST(Math, v3f_add_v3f)		{ EXPECT_EQ(Vec3<>(2.f) + Vec3<>(1.f), Vec3<>(3.f)); }
TEST(Math, v3f_sub_v3f)		{ EXPECT_EQ(Vec3<>(2.f) - Vec3<>(1.f), Vec3<>(1.f)); }
TEST(Math, v3f_mul_v3f)		{ EXPECT_EQ(Vec3<>(2.f) * Vec3<>(2.f), Vec3<>(4.f)); }
TEST(Math, v3f_div_v3f)		{ EXPECT_EQ(Vec3<>(4.f) / Vec3<>(2.f), Vec3<>(2.f)); }
TEST(Math, v3f_add_f)		{ EXPECT_EQ(Vec3<>(2.f) + 1.f, Vec3<>(3.f)); }
TEST(Math, v3f_sub_f)		{ EXPECT_EQ(Vec3<>(2.f) - 1.f, Vec3<>(1.f)); }
TEST(Math, v3f_mul_f)		{ EXPECT_EQ(Vec3<>(2.f) * 2.f, Vec3<>(4.f)); }
TEST(Math, v3f_div_f)		{ EXPECT_EQ(Vec3<>(4.f) / 2.f, Vec3<>(2.f)); }
TEST(Math, v3f_dot)			{ ASSERT_FLOAT_EQ(Vec3<>(1.f, 0.f, 0.f) & Vec3<>(1.f, 0.f, 0.f), 1.f); }
TEST(Math, v3f_cross)		{ EXPECT_EQ(Vec3<>(1.f, 0.f, 0.f) ^ Vec3<>(0.f, 1.f, 0.f), Vec3<>(0.f, 0.f, 1.f)); }

/////////////////////////////////////////////////
// Vec4 tests                                  //
/////////////////////////////////////////////////

TEST(Math, v4f_component_x)	{ ASSERT_FLOAT_EQ(Vec4<>(1.f).x, 1.f); }
TEST(Math, v4f_component_y)	{ ASSERT_FLOAT_EQ(Vec4<>(1.f).y, 1.f); }
TEST(Math, v4f_component_z)	{ ASSERT_FLOAT_EQ(Vec4<>(1.f).z, 1.f); }
TEST(Math, v4f_component_w)	{ ASSERT_FLOAT_EQ(Vec4<>(1.f).w, 1.f); }
TEST(Math, v4f_sq_size)		{ EXPECT_FLOAT_EQ(Vec4<>(1.f).getSquaredSize(), 4.f); }
TEST(Math, v4f_size)		{ EXPECT_FLOAT_EQ(Vec4<>(1.f).getSize(), sqrtf(4.f)); }
TEST(Math, v4f_eq)			{ EXPECT_TRUE(Vec4<>(0.f) == Vec4<>(0.f)); }
TEST(Math, v4f_neq)			{ EXPECT_FALSE(Vec4<>(0.f) != Vec4<>(0.f)); }
TEST(Math, v4f_norm)		{ EXPECT_EQ(Vec4<>(6.f, 8.f, 0.f, 0.f).getNormal().getSquaredSize(), 1.f); }
TEST(Math, v4f_add_v4f)		{ EXPECT_EQ(Vec4<>(2.f) + Vec4<>(1.f), Vec4<>(3.f)); }
TEST(Math, v4f_sub_v4f)		{ EXPECT_EQ(Vec4<>(2.f) - Vec4<>(1.f), Vec4<>(1.f)); }
TEST(Math, v4f_mul_v4f)		{ EXPECT_EQ(Vec4<>(2.f) * Vec4<>(2.f), Vec4<>(4.f)); }
TEST(Math, v4f_div_v4f)		{ EXPECT_EQ(Vec4<>(4.f) / Vec4<>(2.f), Vec4<>(2.f)); }
TEST(Math, v4f_add_f)		{ EXPECT_EQ(Vec4<>(2.f) + 1.f, Vec4<>(3.f)); }
TEST(Math, v4f_sub_f)		{ EXPECT_EQ(Vec4<>(2.f) - 1.f, Vec4<>(1.f)); }
TEST(Math, v4f_mul_f)		{ EXPECT_EQ(Vec4<>(2.f) * 2.f, Vec4<>(4.f)); }
TEST(Math, v4f_div_f)		{ EXPECT_EQ(Vec4<>(4.f) / 2.f, Vec4<>(2.f)); }
TEST(Math, v4f_dot)			{ EXPECT_FLOAT_EQ(Vec4<>(1.f, 0.f, 0.f, 0.f) & Vec4<>(1.f, 0.f, 0.f, 0.f), 1.f); }

/////////////////////////////////////////////////
// Quat tests                                  //
/////////////////////////////////////////////////

TEST(Math, quat_angle)		{ EXPECT_FLOAT_EQ(quat(vec3::up, M_PI).getAngle(), M_PI); }
TEST(Math, quat_axis)		{ EXPECT_EQ(quat(vec3::up, M_PI).getAxis(), vec3::up); }
TEST(Math, quat_inverse)	{ EXPECT_EQ(!quat(vec3::up, M_PI), quat(vec3::up, M_PI)); }
TEST(Math, quat_mul_quat)	{ EXPECT_EQ(quat(vec3::up, 1.f) * quat(vec3::up, 1.f), quat(vec3::up, 2.f)); }
TEST(Math, quat_mul_v3f)	{ EXPECT_EQ(quat(vec3::up, M_PI) * vec3::forward, vec3::backward); }
TEST(Math, quat_forward)	{ EXPECT_EQ(quat(vec3::up, M_PI).forward(), vec3::backward); }
TEST(Math, quat_backward)	{ EXPECT_EQ(quat(vec3::up, M_PI).backward(), vec3::forward); }
TEST(Math, quat_left)		{ EXPECT_EQ(quat(vec3::up, M_PI).left(), vec3::right); }
TEST(Math, quat_right)		{ EXPECT_EQ(quat(vec3::up, M_PI).right(), vec3::left); }
TEST(Math, quat_down)		{ EXPECT_EQ(quat(vec3::right, M_PI).down(), vec3::up); }
TEST(Math, quat_up)			{ EXPECT_EQ(quat(vec3::right, M_PI).up(), vec3::down); }

/////////////////////////////////////////////////
// Mat4 tests                                  //
/////////////////////////////////////////////////

TEST(Math, m4f_eq)			{ EXPECT_TRUE(mat4::translation(vec3(1.f)) == mat4::translation(vec3(1.f))); }
TEST(Math, m4f_neq)			{ EXPECT_FALSE(mat4::translation(vec3(1.f)) != mat4::translation(vec3(1.f))); }
TEST(Math, m4f_fill)		{ EXPECT_EQ(mat4::fill(1.f), mat4(1.f, 1.f, 1.f, 1.f,  1.f, 1.f, 1.f, 1.f,  1.f, 1.f, 1.f, 1.f,  1.f, 1.f, 1.f, 1.f)); }
TEST(Math, m4f_eye)			{ EXPECT_EQ(mat4::eye(1.f), mat4(1.f, 0.f, 0.f, 0.f,  0.f, 1.f, 0.f, 0.f,  0.f, 0.f, 1.f, 0.f,  0.f, 0.f, 0.f, 1.f)); }
TEST(Math, m4f_diag)		{ EXPECT_EQ(mat4::diag(vec4(1.f, 2.f, 3.f, 4.f)), mat4(1.f, 0.f, 0.f, 0.f,  0.f, 2.f, 0.f, 0.f,  0.f, 0.f, 3.f, 0.f,  0.f, 0.f, 0.f, 4.f)); }
TEST(Math, m4f_translation)	{ EXPECT_EQ(mat4::translation(vec3(1.f)), mat4(1.f, 0.f, 0.f, 1.f, 0.f,  1.f, 0.f, 1.f, 0.f,  0.f, 1.f, 1.f, 0.f,  0.f, 0.f, 1.f)); }
TEST(Math, m4f_scale)		{ EXPECT_EQ(mat4::scale(vec3(1.f, 2.f, 3.f)), mat4(1.f, 0.f, 0.f, 0.f,  0.f, 2.f, 0.f, 0.f,  0.f, 0.f, 3.f, 0.f,  0.f, 0.f, 0.f, 1.f)); }
TEST(Math, m4f_rotation)	{ EXPECT_EQ(mat4::rotation(vec3::up, M_PI / 2.f), mat4(0.f, 0.f, -1.f, 0.f,  0.f, 1.f, 0.f, 0.f,  1.f, 0.f, 0.f, 0.f,  0.f, 0.f, 0.f, 1.f)); }
TEST(Math, m4f_det)			{ EXPECT_EQ(mat4::eye(3.f).getDeterminant(), 81.f); }
TEST(Math, m4f_transpose)	{ EXPECT_EQ(mat4::translation(vec3(1.f)).getTranspose(), mat4(1.f, 0.f, 0.f, 0.f,  0.f, 1.f, 0.f, 0.f,  0.f, 0.f, 1.f, 0.f,  1.f, 1.f, 1.f, 1.f)); }
TEST(Math, m4f_add_s)		{ EXPECT_EQ(mat4::eye(0.f) + 1.f, mat4(1.f, 1.f, 1.f, 1.f,  1.f, 1.f, 1.f, 1.f,  1.f, 1.f, 1.f, 1.f,  1.f, 1.f, 1.f, 1.f)); }
TEST(Math, m4f_sub_s)		{ EXPECT_EQ(mat4::eye(0.f) - 1.f, mat4(-1.f, -1.f, -1.f, -1.f,  -1.f, -1.f, -1.f, -1.f,  -1.f, -1.f, -1.f, -1.f,  -1.f, -1.f, -1.f, -1.f)); }
TEST(Math, m4f_mul_s)		{ EXPECT_EQ(mat4::eye(1.f) * 3.f, mat4::eye(3.f)); }
TEST(Math, m4f_div_s)		{ EXPECT_EQ(mat4::eye(2.f) / 2.f, mat4::eye(1.f)); }
TEST(Math, v4f_add_m4f)		{ EXPECT_EQ(vec4(1.f) + mat4::eye(0.f), mat4::eye(0.f) + 1.f); }
TEST(Math, v4f_sub_m4f)		{ EXPECT_EQ(vec4(0.f) - mat4::eye(1.f), mat4::eye(-1.f)); }
TEST(Math, v4f_mul_m4f)		{ EXPECT_EQ(vec4(3.f) * mat4::eye(1.f), mat4::eye(1.f) * 3.f); }
TEST(Math, v4f_div_m4f)		{ EXPECT_EQ(vec4(2.f) / mat4::fill(2.f), mat4::fill(1.f)); }
TEST(Math, s_add_m4f)		{ EXPECT_EQ(1.f + mat4::eye(0.f), mat4::eye(0.f) + 1.f); }
TEST(Math, s_sub_m4f)		{ EXPECT_EQ(0.f - mat4::eye(1.f), mat4::eye(-1.f)); }
TEST(Math, s_mul_m4f)		{ EXPECT_EQ(3.f * mat4::eye(1.f), mat4::eye(1.f) * 3.f); }
TEST(Math, s_div_m4f)		{ EXPECT_EQ(2.f / mat4::fill(2.f), mat4::fill(1.f)); }
TEST(Math, m4f_mul_m4f)		{ EXPECT_EQ(mat4::translation(vec3(1.f)) * mat4::translation(vec3(1.f)), mat4::translation(vec3(2.f))); }
TEST(Math, m4f_inverse)		{ EXPECT_EQ(!mat4(2.f, 1.f, 0.f, 1.f,  3.f, 1.f, 1.f, 1.f,  2.f, 0.f, 3.f, 1.f,  0.f, 0.f, 0.f, 1.f), mat4(-3.f, 3.f, -1.f, 1.f,  7.f, -6.f, 2.f, -3.f,  2.f, -2.f, 1.f, -1.f,  0.f, -0.f, 0.f, 1.f)); }
TEST(Math, m4f_inverse_eq)	{ EXPECT_EQ(mat4::eye(4.f) * mat4::eye(4.f).getInverse(), mat4::eye(1.f)); }
TEST(Math, m4f_mul_v2f)		{ EXPECT_EQ(mat4::translation(vec3(1.f)) * vec2(1.f), vec2(2.f)); }
TEST(Math, m4f_mul_v3f)		{ EXPECT_EQ(mat4::translation(vec3(1.f)) * vec3(1.f), vec3(2.f)); }
TEST(Math, m4f_mul_v4f)		{ EXPECT_EQ(mat4::translation(vec3(1.f)) * vec4(1.f), vec4(2.f, 2.f, 2.f, 1.f)); }
TEST(Math, m4f_quat_rot_eq)	{ EXPECT_EQ(mat4::rotation(quat(vec3::up, M_PI / 6.f)) * vec3::right, quat(vec3::up, M_PI / 6.f) * vec3::right); }

/**
 * @}
 */