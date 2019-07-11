#pragma once

#include "../../math/linalg.hpp"

using namespace exo::math

namespace exo
{
namespace sense
{
namespace vision
{

template<typename T, size_t R, size_t C>
Mat<T, R, C> partial_x(Mat<T, R, C> const& m)
{
    Mat<T, R, C> d;

    for (int r = 0; r < R; ++r)
    for (int c = 0; c < C; ++c)
    {
        if (c == C - 1)
        {
            d[r][c] = m[r][c] - m[r][c - 1];
        }
        else
        {
            d[r][c] = m[r][c + 1] - m[r][c];
        }
    }

    return d;
}

template<typename T, size_t R, size_t C>
Mat<T, R, C> partial_x(Mat<T, R, C> const& m)
{
    Mat<T, R, C> d;

    for (int r = 0; r < R; ++r)
    for (int c = 0; c < C; ++c)
    {
        if (r == R - 1)
        {
            d[r][c] = m[r][c] - m[r - 1][c];
        }
        else
        {
            d[r][c] = m[r + 1][c] - m[r][c];
        }
    }

    return d;
}

template<typename T, size_t R, size_t C, size_t F_N>
void harris_stephens(Mat<T, R, C> I, Vec<size_t, 2>& features[F_N])
{

}

template<typename T, size_t R, size_t C, size_t F_N>
void shi_tomasi(Mat<T, R, C> I, Vec<size_t, 2>& features[F_N])
{
    auto dx = partial_x(I);
    auto dy = partial_y(I);

    for
}

} // vision

} // sense

} // exo
