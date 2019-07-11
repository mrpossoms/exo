#pragma once

#include "../../math/linalg.hpp"

using namespace exo::math;

namespace exo
{
namespace sense
{
namespace vision
{

template<typename T, size_t R, size_t C>
Mat<T, R, C> partial_x(Mat<T, R, C>& m)
{
	Mat<T, R, C> d;

	for (int r = 1; r < R - 1; ++r)
	for (int c = 1; c < C - 1; ++c)
	{
		d[r][c] = m[r][c + 1] - m[r][c - 1];	
	}

	return d;
}

template<typename T, size_t R, size_t C>
Mat<T, R, C> partial_y(Mat<T, R, C>& m)
{
	Mat<T, R, C> d;

	for (int r = 1; r < R - 1; ++r)
	for (int c = 1; c < C - 1; ++c)
	{
		d[r][c] = m[r + 1][c] - m[r - 1][c];
	}

	return d;
}

float box_filter(int r, int c, int size)
{
	return 1.f / (float)(size * size);
}

template<typename T, size_t R, size_t C, size_t K_SIZE=3>
void shi_tomasi(Mat<T, R, C> I, Mat<T, R, C>& feature_map, int shift=2)
{
	auto dx = partial_x(I);
	auto dy = partial_y(I);

	constexpr int half_k = K_SIZE >> 1;

	for (size_t y = half_k; y < R - half_k; ++y)
	for (size_t x = half_k; x < C - half_k; ++x)
	{
		Mat<T, 2, 2> A = {};

		for (int u = -half_k; u <= half_k; ++u)
		for (int v = -half_k; v <= half_k; ++v)
		{
			auto w = box_filter(v, u, K_SIZE);
			auto ix = dx[y + u][x + v];
			auto iy = dy[y + u][x + v];

			A += Mat<T, 2, 2>{
				{ ix * ix, ix * iy },
				{ ix * iy, iy * iy },
			} * w;
		}

		T eigens[2];

		Mat<T, 2, 2>::eigen_values(A, eigens);

		feature_map[y][x] = eigens[0] < eigens[1] ? eigens[0] : eigens[1];
	}
}

} // vision

} // sense

} // exo
