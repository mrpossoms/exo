#include "exo.hpp"
#include "math.hpp"

#define DESCRIPTION "Tests exo math functions"

using namespace exo::math;

#include "test.h"
{
	auto v0 = Vec<float, 3>{ 1, 2, 3 };
	auto v1 = Vec<float, 3>{ 1 };

	// test addition
	auto v2 = v0 + v1;
	assert(v2[0] == 2);
	assert(v2[1] == 3);
	assert(v2[2] == 4);

	// test dot
	assert(NEAR(v2.dot(v2), 29));

	// test indexing and assignment
	v2[0] = 1000;
	assert(v2[0] == 1000);

	// check scaling
	v2 *= 0.001f;
	assert(v2[0] == 1);

	auto ortho = Vec<float,3>::cross({1, 0, 0}, {0, 1, 0});

	assert(ortho[0] == 0);
	assert(ortho[1] == 0);
	assert(ortho[2] == 1);

	// Test quaternions
	auto q = Quat::from_axis_angle({0, 0, 1}, M_PI / 2);

	bool expect_true = q.rotate({ 1, 0, 0 }).is_near(Vec<float, 3>{ 0, 1, 0 }, 0.1);
	assert(expect_true);

	{ // check matrix equality testing
		auto I0 = Mat<float, 3, 3>::I();
		auto I1 = Mat<float, 3, 3>::I();

		assert(I0 == I1);
	}

	{ // check matrix multiplication correctness
		auto R = Mat<float, 3, 3>{
			{ (float)rand(), (float)rand(), (float)rand(), },
			{ (float)rand(), (float)rand(), (float)rand(), },
			{ (float)rand(), (float)rand(), (float)rand(), },
		};
		auto I  = Mat<float, 3, 3>::I();
		auto res = R * I;

		assert(res == R);	
	}

	return 0;
}
