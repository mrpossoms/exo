#pragma once

#include "linalg.hpp"

using namespace exo::math;

namespace exo
{
namespace math
{

template <class S, ssize_t F_SIZE, ssize_t U_SIZE, ssize_t Z_SIZE>
struct KalmanFilter
{
	KalmanFilter(
		Mat<S, F_SIZE, F_SIZE> f,
		Mat<S, F_SIZE, U_SIZE> b,
		Mat<S, F_SIZE, Z_SIZE> h) : F(f), B(b), H(h)
	{

	}

	virtual Mat<S, 

	void update()
	{

	}

private:
	// current estimate
	Mat<S, F_SIZE> x_hat;
	// prediction matrix
	Mat<S, F_SIZE, F_SIZE> F;
	// prediction co-variance matrix
	Mat<S, F_SIZE, F_SIZE> P = Mat<S, F_SIZE, F_SIZE>::I();
	// control matrix
	Mat<S, F_SIZE, U_SIZE> B;
	// unexpected noise co-variance
	Mat<S, F_SIZE, F_SIZE> Q = Mat<S, F_SIZE, F_SIZE>::I();
	// sensor mapping matrix
	Mat<S, Z_SIZE, F_SIZE> H;
	// sensor noise matrix
	Mat<S, 
};

} // math

} // exo