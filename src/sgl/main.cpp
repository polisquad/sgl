#include "math/vec3.h"

using VecOps = Vec3<float32>::VecOps;
using VecT = Vec3<float32>::VecT;

int main()
{
	vec3 a(1.f), b(1.f), v;
	b[3] = 0.f;

	vec3 acceleration, velocity, location;

	printf("%d\n", a.isNearlyZero());

	auto startTick = clock();
	for (uint64 i = 0; i < 100000; ++i)
	{
		const float dt = 0.000001f;
		acceleration = (a + b).getNormal();
		if (!acceleration.isNearlyZero())
		{
			velocity += acceleration * dt;
			location += velocity * dt;
		}

		velocity += a;
		velocity += b;
		velocity += a * 2;
		velocity += b * 2;
	}
	printf("elasped time: %ld clock ticks;\n", clock() - startTick);

	return 0;
}