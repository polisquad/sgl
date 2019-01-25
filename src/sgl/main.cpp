#include "math/math.h"

int main()
{
	(quat(M_PI_4, vec3::up) * quat(M_PI_4, vec3::up) * vec3::right).print();
	return 0;
}