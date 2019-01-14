#include "math/math.h"

int main()
{
	Simd::Vector<float32, 8>::Type v;
	float32 s;
	//for (uint64 i = 0; i < 10000000; ++i) v = Simd::Vector<float32, 8>::Type{s, s, s, s, s, s, s, s};
	for (uint64 i = 0; i < 10000000; ++i) v = Simd::Vector<float32, 8>::load(s);

	return 0;
}