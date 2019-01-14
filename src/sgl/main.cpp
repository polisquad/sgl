#include "math/math.h"

int main()
{
	Vec3<float32, false> v(1.f, 0.f, 0.f);
	Quat<float32, false> q(M_PI / 2.f, Vec3<float32, false>(0.f, 1.f, 0.f));

	return 0;
}