#include "exo.hpp"
#include "math.hpp"

#include <iostream>
#include <random>

#define DESCRIPTION "Tests exo math functions"

using namespace exo::math;

float uniform_random(float max=1)
{
	return max * ((rand() % 2048 - 1024) / 1024.f);
}

#include "test.h"
{
	(void)argc; (void)argv;

	auto v0 = Vec<float, 3>{ 1, 2, 3 };
	auto v1 = Vec<float, 3>{ 1 };


	{ // test addition
		Vec<float, 3> expected = { 2, 3, 4 };
		assert((v0 + v1) == expected);
	}

	{ // test subtraction
		Vec<float, 3> expected = { 0, 1, 2 };
		assert((v0 - v1) == expected);
	}

	{ // test addition
		Vec<float, 3> expected = { 1, 2, 3 };
		assert((v0 * v1) == expected);
	}

	{ // test addition
		Vec<float, 3> expected = { 1, 2, 3 };
		assert((v0 / v1) == expected);
	}

	// test dot
	assert(NEAR(v0.dot(v0), 14));

	{ // test length and norm
		Vec<float, 3> v = { 1, 2, 3 };
		assert(NEAR(v.norm().len(), 1));
	}

	// test indexing and assignment
	v0[0] = 1000;
	assert(v0[0] == 1000);

	// check scaling
	v0 *= 0.001f;
	assert(v0[0] == 1);

	{ // check vector projection 0
		Vec<float,3> v = { 1, 0, 0 };
		Vec<float,3> w = {1, 1, 0};

		auto p = w.project_onto(v);
		assert(NEAR((p - v).len(), 0));
	}

	{ // check vector projection 1
		Vec<float,3> v = { 0, 1, 0 };
		Vec<float,3> w = {1, 1, 0};

		auto p = w.project_onto(v);
		assert(NEAR((p - v).len(), 0));
	}

	auto ortho = Vec<float,3>::cross({1, 0, 0}, {0, 1, 0});

	assert(ortho[0] == 0);
	assert(ortho[1] == 0);
	assert(ortho[2] == 1);

	// Test quaternions
	{
		auto q = Quat::from_axis_angle({0, 0, 1}, M_PI / 4);

		bool expect_true = q.rotate({ 1, 0, 0 }).is_near(Vec<float, 3>{ 1, 1, 0 }.norm(), 0.1);
		assert(expect_true);
	}

	// Test quaternion angular difference
	{
		auto theta = uniform_random() + 1;

		for(int i = 100; i--;)
		{
			Quat q0 = Quat::from_axis_angle({0, 0, 1}, 0);
			Quat q1 = Quat::from_axis_angle({0, 0, 1}, theta);

			auto angle = q0.rotational_difference(q1);

			// std::cerr << "angle: " << std::to_string(angle) << "\n";

			assert(abs(theta - angle) < 0.0001);
		}
	}

	// Test quaternion to matrix conversion
	for (int i = 100; i--;)
	{
		Vec<float, 3> axis = {uniform_random(), uniform_random(), uniform_random()};
		auto q = Quat::from_axis_angle(axis.norm(), uniform_random(M_PI));
		auto m = q.to_matrix<float>();

		auto v = Vec<float, 4>{ 1, 0, 0, 1 };

		auto vq = q.rotate(v.as_dimension<3>());
		auto vm = (m * v).as_dimension<3>();

		// std::cerr << vq.to_string() << std::endl;
		// std::cerr << vm.to_string() << std::endl;
		assert(vq.is_near(vm));
	}

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

	{ // check matrix transpose
		Mat<int, 2, 3> M = {
			{ 1, 2, 3 },
			{ 4, 5, 6 },
		};

		Mat<int, 3, 2> Mt = {
			{ 1, 4 },
			{ 2, 5 },
			{ 3, 6 },
		};

		assert(M.transpose() == Mt);
	}

	{ // check matrix row swap
		Mat<int, 2, 2> M = {
			{ 1, 3 },
			{ 2, 7 },
		};

		M.swap_rows(0, 1);

		Mat<int, 2, 2> E = {
			{ 2, 7 },
			{ 1, 3 },
		};

		assert(M == E);
	}

	{ // check matrix slicing
		Mat<float, 8, 8> M = {
			{ 1, 1, 2, 2 },
			{ 1, 1, 2, 2 },
			{ 3, 3, 4, 4 },
			{ 3, 3, 4, 4 },
		};

		auto twos = M.slice<2, 2>(0, 2);

		for (int i = 4; i--;) assert(twos[i / 2][i % 2] == 2);
	}

	{ // check matrix inverse
		Mat<float, 4, 4> M;

		// initialize totally random matrix. Which will
		// be invertible with very high likelihood.
		M.initialize([](float r, float c) {
			(void)r; (void)c;
			return static_cast<float>((rand() % 32) - 16);
		});

		auto inv = M.inverse();
		std::cout << inv.to_string() << std::endl;

		inv = inv * M;
		std::cout << inv.to_string() << std::endl;

		auto I = Mat<float, 4, 4>::I();

		assert(I.distance(inv) < 0.0001);
	}

	{ // Check 2d rotation #1
		auto v = Vec<float, 2>{ 0, 1 };
		auto rotated = Mat2f::rotation(0) * v;
		assert(v.is_near(rotated));
	}

	{ // Check 2d rotation #2
		auto v = Vec<float, 2>{ 0, 1 };
		auto rotated = Mat2f::rotation(M_PI / 2) * v;
		auto expected = Vec<float, 2>{ 1, 0 };
		assert(expected.is_near(rotated));
	}

	{ // Check 2d rotation #3
		auto v = Vec<float, 2>{ 0, 1 };
		auto rotated = Mat2f::rotation(M_PI) * v;
		auto expected = Vec<float, 2>{ 0, -1 };
		assert(expected.is_near(rotated));
	}

	{ // Check 2d rotation #4
		auto v = Vec<float, 2>{ 0, 1 };
		auto rotated = Mat2f::rotation(3 * (M_PI / 2)) * v;
		auto expected = Vec<float, 2>{ -1, 0 };
		assert(expected.is_near(rotated));
	}

	{ // Check matrix rotation then translation
		auto tm = Mat4f::translate({1, 0, 0});
		auto rm = Mat4f::rotate_z(-M_PI / 2);
		auto v = Vec<float, 4>{ 0, 1, 0, 1 };
		auto trans = tm * rm;
		auto r = trans * v;
		std::cerr << r.to_string() << std::endl;
		assert(r.is_near({ 2, 0, 0, 1 }));
	}

	{ // test covariance estimation
		float sig_0 = 0.5f, sig_1 = 0.25f; // standard deviations of pdf
		float sig_01 = sig_0 * sig_1;
		float sig_0_2 = sig_0 * sig_0;
		float sig_1_2 = sig_1 * sig_1;
		float p = 0.05 / sig_01;           // correlation between variables
		float p2 = p * p;

		// generate sample dataset of correlated bivariate distribution
		constexpr ssize_t n = 1000;
		Mat<float, 2, n> X;
		int i = 0;
		float pi2 = M_PI * 2;

		constexpr int size = 8;
		float counts[size][size] = {};
		float max = 0;

		while(i < n)
		{
			auto x = uniform_random(4), y = uniform_random(4);
			auto x2 = x * x, y2 = y * y;
			auto z = (x2 / sig_0_2) + (y2 / sig_1_2) - ((2 * p * x * y) / sig_01);
			auto sample = 1 / (pi2 * sig_01 * sqrt(1 - p2));
			sample *= exp(-z / (2 * (1 - p2)));

			if (uniform_random(0.5) + 0.5 < sample)
			{
				X[0][i] = x;
				X[1][i] = y;
				auto c = counts[(int)x + 4][(int)y + 4]++;

				if (c > max) { max = c; }
				i++;
			}
		}

		std::cout << "Visualized distribution\n";

		// render densities into terminal
		const char* spectrum = " .,:;+=xX#";
		int max_i = 0;
		for (int y = 0; y < size; ++y)
		{
			for (int x = 0; x < size; ++x)
			{
				int i = 9 * (counts[x][y] / max);
				std::cout << spectrum[i] << spectrum[i];

				if (i > max_i) { max_i = i; }
			}

			std::cout << "|\n";
		}

		Mat<float, 2, 2> expected_sig = {
			{ sig_0_2, p * sig_01 },
			{ p * sig_01, sig_1_2 },
		};
		auto Q = Mat<float, 2, 2>::estimate_covariance(X);
		std::cout << "Max count: " << std::to_string(max) << " Max spec i: " << std::to_string(max_i) << "\n";
		std::cout << "Expected covariance matrix\n";
		std::cout << expected_sig.to_string() << "\n";
		std::cout << "Estimated covariance matrix Q\n";
		std::cout << Q.to_string() << "\n";
	}

	{ // Check kalman filter
		const float dt = 0.1f;

		Mat<float, 2, 2> F = {
			{ 1,  dt },
			{ 0,   1 },
		};

		Mat<float, 2, 1> B = {
			{ powf(dt, 2) / 2 },
			{ dt },
		};

		auto kf = KalmanFilter<float, 2, 1, 2>(F, B);

		float position = uniform_random(100);
		float velocity;

		// sensor noise covariance
		kf.R = {
			{ 1,   0 },
			{ 0,   1 },
		};

		// process noise covariance
		kf.Q = {
			{ 0.1,   0 },
			{ 0,   0.1 },
		};

		std::default_random_engine gen;
		std::normal_distribution<float> norm_dist(0,0.1);

		for (float t = 0; t < 1; t += dt)
		{
			auto acceleration = -sin(t);
			velocity = cos(t);
			position += velocity * dt;

			Vec<float, 2> noise = { norm_dist(gen), norm_dist(gen) };
			Vec<float, 2> truth = { position, velocity };
			Vec<float, 2> noisy = truth + noise;

			auto x_hat = kf.predict({ acceleration });
			kf.update(noisy);

			std::cout << "Noise " << std::to_string(noise[0]) << ", " << std::to_string(noise[1]) << "\n";
			std::cout << "Filter Error: " << std::to_string((x_hat - truth).len()) <<
			              ", raw error: " << std::to_string((truth - noisy).len()) << "\n";
		}

		std::cout << "vv P vv\n" << kf.P.to_string() << "\n";
	}

	return 0;
}
