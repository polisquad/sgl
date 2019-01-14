#include "math/math.h"

int main()
{
	vec3 location(1.f), scale(1.f);
	quat rotation(M_PI_4, vec3::up);

	for (uint64 i = 0; i < 100000; ++i) mat4::projection(M_PI);

	return 0;
}