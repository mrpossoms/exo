#pragma once

namespace exo
{
namespace math
{

template<typename S, ssize_t D>
struct CatmulRom {
    /**
     * @brief Represents a curve segment that contains exactly 4 control points.
     *        any other points in the array are ignored.
     */
    CatmulRom(Vec<S, D>* control_points)
    {
        _control_points = control_points;
    }

    void compute_knots()
    {
        for (int i = 1; i < 3; ++i)
        {
            auto& p_i   = _control_points[i];
            auto& p_i_1 = _control_points[i + 1];

            auto t = (S)pow((p_i_1 - p_i).len(), 0.5);

            _t_knots[i] = _t_knots[i - 1] + t;
        }
    }

    Vec<S, D> point(S t)
    {
        Vec<S, D> A[3] = {};
        Vec<S, D> B[2] = {};
        auto P = _control_points;

        { // compute A's
            for (int i = 0; i < 3; ++i)
            {
                auto t_i = _t_knots[i], t_i_1 = _t_knots[i + 1];
                auto den = t_i_1 - t_i;
                A[i] = P[i] * ((t_i_1 - t) / den) + P[i + 1] * ((t - t_i) / den);
            }
        }

        { // compute B's
            for (int i = 0; i < 2; ++i)
            {
                auto t_i = _t_knots[i], t_i_2 = _t_knots[i + 2];
                auto den = t_i_2 - t_i;
                B[i] = A[i] * ((t_i_2 - t) / den) + A[i + 1] * ((t - t_i) / den);
            }
        }


    }

    Vec<S, D> slope(S t)
    {

    }

private:

    Vec<S, D>* _control_points = nullptr;
    S _t_knots[4] = {};
};

}
}
