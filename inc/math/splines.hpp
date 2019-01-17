#pragma once

#include "linalg.hpp"

namespace exo
{
namespace math
{

template<typename S, ssize_t D>
struct CatmullRom {
    /**
     * @brief Represents a curve segment that contains exactly 4 control points.
     *        any other points in the array are ignored.
     */
    CatmullRom(Vec<S, D>* control_points)
    {
        _p = control_points;
    }

    /**
     * @brief Compute knots for a centripetal Catmull-Rom spline
     */
    void compute_knots()
    {
        for (int i = 0; i < 3; ++i)
        {
            auto t = (S)pow((_p[i + 1] - _p[i]).len(), 0.5);

            _t[i + 1] = t + _t[i];
        }
    }

    Vec<S, D> point(S t)
    {
        Vec<S, D> A[3] = {};
        Vec<S, D> B[2] = {};

        // t is assumed [0, 1], scale to range [_t0, _t3]
        t *= (_t[2] - _t[1]);
        t += _t[1];

        { // compute A's
            for (int i = 0; i < 3; ++i)
            {
                auto den = _t[i + 1] - _t[i];
                A[i] = _p[i] * ((_t[i + 1] - t) / den) + _p[i + 1] * ((t - _t[i]) / den);
            }
        }

        { // compute B's
            for (int i = 0; i < 2; ++i)
            {
                auto den = _t[i + 2] - _t[i];
                B[i] = A[i] * ((_t[i + 2] - t) / den) + A[i + 1] * ((t - _t[i]) / den);
            }
        }


        { // compute C
            auto den = _t[2] - _t[1];
            return B[0] * ((_t[2] - t) / den) + B[1] * ((t - _t[1]) / den);
        }
    }

    Vec<S, D> slope(S t)
    {

    }

private:

    Vec<S, D>* _p = nullptr;
    S _t[4] = {};
};

}
}
