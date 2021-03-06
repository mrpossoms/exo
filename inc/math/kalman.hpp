#pragma once

#include "linalg.hpp"

using namespace exo::math;

namespace exo
{
namespace math
{

/**
 * @brief      Container for a Kalman filter's matrices and values.
 *
 * @tparam     S       Numerical storage type to be used for elements in
 *                     matrices and vectors.
 * @tparam     X_SIZE  Number of elements in state vector.
 * @tparam     U_SIZE  Number of elements in the control vector.
 * @tparam     Z_SIZE  Number of elements in the sensor reading vector.
 */
template <class S, ssize_t X_SIZE, ssize_t U_SIZE, ssize_t Z_SIZE>
struct KalmanFilter
{
	KalmanFilter() {}

	KalmanFilter(
		Mat<S, X_SIZE, X_SIZE> f) : F(f)
	{

	}


	KalmanFilter(
		Mat<S, X_SIZE, X_SIZE> f,
		Mat<S, X_SIZE, U_SIZE> b) : F(f), B(b)
	{

	}


	virtual Mat<S, X_SIZE, Z_SIZE> gain()
	{
		auto Ht = H.transpose();
		auto innovation_cov = H * P * Ht + R;
		auto innovation_cov_1 = innovation_cov.inverse();

		return P * Ht * innovation_cov_1;
	}


	void update(Vec<S, Z_SIZE> const& sensor_readings)
	{
		// if (dirty) { return; }

		const auto I = Mat<S, Z_SIZE, X_SIZE>::I();
		auto K = gain();

		auto innovation = sensor_readings - H * x_hat;

		x_hat = x_hat + K * innovation;
		P = (I - K * H) * P;

		dirty = true;
	}


	Vec<S, X_SIZE> predict(Vec<S, U_SIZE> const& control_vector)
	{
		// if (dirty)
		{
			x_hat = F * x_hat + B * control_vector;
			P = F * P * F.transpose() + Q;
			dirty = false;
		}

		return x_hat;
	}

	/**
	 * Current system state estimate
	 */
	Vec<S, X_SIZE> x_hat = {};

	/**
	 * Prediction matrix given the state in one time step, the prediction matrix
	 * defines how the values in the next time step are expected to change without
	 * outside influences, control signals or excessive noise.s
	 */
	Mat<S, X_SIZE, X_SIZE> F;

	/**
	 * Prediction covariance matrix. Defines the relationship between each element
	 * in the state vector, and how they are expected to vary together.
	 */
	Mat<S, X_SIZE, X_SIZE> P = Mat<S, X_SIZE, X_SIZE>::I();

	/**
	 * Control matrix, maps the control vector into the state vector's space.
	 */
	Mat<S, X_SIZE, U_SIZE> B = Mat<S, X_SIZE, U_SIZE>::I();

	/**
	 * Unexpected noise covariance matrix. Defines the relationship between
	 * an element in the state vector, and every other element in the state vector
	 * and how they are related in regards to random noise.
	 */
	Mat<S, X_SIZE, X_SIZE> Q = Mat<S, X_SIZE, X_SIZE>::I() * 0.01f;

	/**
	 * Sensor mapping matrix maps the sensor reading vector into the state
	 * vector space.
	 */
	Mat<S, Z_SIZE, X_SIZE> H = Mat<S, Z_SIZE, X_SIZE>::I();

	/**
	 * Sensor noise matrix
	 */
	Mat<S, Z_SIZE, Z_SIZE> R = Mat<S, Z_SIZE, Z_SIZE>::I() * 0.01;

private:
	bool dirty = true;
};

} // math

} // exo
