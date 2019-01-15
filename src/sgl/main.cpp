#include "math/math.h"

int main()
{
	mat4 t = mat4::translation(1.f, 2.f, 3.f);
	mat4 r = mat4::rotation(M_PI_4, vec3::up);
	mat4 s = mat4::scaling(2.f, 2.f, 2.f);

	vec3 v(1.f, 2.f, 0.5f);
	vec3 u = (t * r * s) * v;
	vec3 w = (t * r * s).getInverseTransform() * u;
	v.print(),
	u.print(),
	w.print();

	return 0;
}