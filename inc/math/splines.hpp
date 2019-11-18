#pragma once

#include "linalg.hpp"

#include <initializer_list>

namespace exo
{
namespace math
{



template<typename S, size_t D>
struct CatmullRom {
    Vec<S, D>* P = nullptr;

    CatmullRom() = default;

    /**
     * @brief Represents a curve segment that contains exactly 4 control points.
     *        any other points in the array are ignored.
     */
    CatmullRom(Vec<S, D>* control_points)
    {
        P = control_points;
    }

    /**
     * @brief Compute centripetal time knots
     * @returns CatmullRom reference
     */
    CatmullRom& centripetal()
    {
        for (int i = 0; i < 3; ++i)
        {
            auto t = (S)pow((P[i + 1] - P[i]).len(), 0.5);

            _t[i + 1] = t + _t[i];
        }

        return *this;
    }

    /**
     * @brief Compute uniform time knots
     * @returns CatmullRom reference
     */
    CatmullRom& uniform()
    {
        for (int i = 4; i--;) { _t[i] = i / 4.f; }
        return *this;
    }

    /**
     * @brief Computes a point on the spline
     * @parameter t interpolation between control points p1 and p2
     * @returns point on curve at time t
     */
    Vec<S, D> point(S t)
    {
        // t is assumed [0, 1], scale to range [_t0, _t3]
        t *= (_t[2] - _t[1]);
        t += _t[1];

        // compute A's and B's
        compute_A(t);
        compute_B(t);

        { // compute C
            auto den = _t[2] - _t[1];
            return B[0] * ((_t[2] - t) / den) + B[1] * ((t - _t[1]) / den);
        }
    }

    /**
     * @brief Computes the tangent of the spline
     * @parameter t interpolation between control points p1 and p2
     * @returns tangent of curve at time t
     */
    Vec<S, D> tangent(S t)
    {
        Vec<S, D> dA[3] = {};
        Vec<S, D> dB[2] = {};

        // t is assumed [0, 1], scale to range [_t0, _t3]
        t *= (_t[2] - _t[1]);
        t += _t[1];

        compute_A(t);
        compute_B(t);

        { // compute dA's
            for (int i = 0; i < 3; ++i)
            {
                dA[i] = (P[i + 1] - P[0]) / (_t[i + 1] - _t[i]);
            }
        }

        { // compute dB's
            for (int i = 0; i < 2; ++i)
            {
                auto den = _t[i + 2] + _t[i];
                dB[i] = (A[i + 1] - A[i]) / den + dA[i] * ((_t[2] - t) / den) + dA[i + 1] * ((t - _t[i]) / den);
            }
        }

        { // compute dC
            auto den = _t[2] - _t[1];
            return (B[1] - B[0]) / den + dB[0] * ((_t[2] - t) / den) + dB[1] * ((t - _t[1]) / den);
        }
    }


    S nearest_t(Vec<S, D>& point, S t_min={0}, S t_max={1}, unsigned int sub=10)
    {
        S t_mid  = (t_min + t_max) * 0.5f;

        if (sub == 0) { return t_mid; }

        S t_low  = (t_min + t_mid) * 0.5f;
        S t_high = (t_mid + t_max) * 0.5f;

        // difference vectors between our search point, and the
        // high and low midpoints on either side of the time interval
        auto d_low  = point - this->point(t_low);
        auto d_high = point - this->point(t_high);

        // squared distance between the point, low and high
        auto dist_low  = d_low.dot(d_low);
        auto dist_high = d_high.dot(d_high);

        if (dist_low < dist_high)
        {
            return nearest_t(point, t_min, t_mid, sub - 1);
        }

        return nearest_t(point, t_mid, t_max, sub - 1);
    }


    S arc_length(S t_start=0, S t_end=1, unsigned int steps=10)
    {
        auto dt = (t_end - t_start) / static_cast<S>(steps);
        S len_sqr = 0;
        auto last_p = point(t_start);

        for (unsigned int i = 1; i < steps + 1; ++i)
        {
            auto p = point(t_start + i * dt);
            auto delta = p - last_p;
            len_sqr += sqrtf(delta.dot(delta));
            last_p = p;
        }

        return static_cast<S>(len_sqr);
    }

private:
    S _t[4] = {};
    Vec<S, D> A[3] = {};
    Vec<S, D> B[2] = {};

    void compute_A(S t)
    {
        for (int i = 0; i < 3; ++i)
        {
            auto den = _t[i + 1] - _t[i];
            A[i] = P[i] * ((_t[i + 1] - t) / den) + P[i + 1] * ((t - _t[i]) / den);
        }
    }

    void compute_B(S t)
    {
        for (int i = 0; i < 2; ++i)
        {
            auto den = _t[i + 2] - _t[i];
            B[i] = A[i] * ((_t[i + 2] - t) / den) + A[i + 1] * ((t - _t[i]) / den);
        }
    }
};


template<size_t POINTS, typename S, size_t D>
struct path
{
    path() = default;

    path(Vec<S, D>* points)
    {
        // copy control points
        for (unsigned int i = 0; i < POINTS; ++i) { ctrl_pts[i] = points[i]; }

        init();
    }


    path(std::initializer_list<Vec<S, D>> const& points)
    {
        int i = 0;
        for (Vec<S, D> p : points)
        {
            ctrl_pts[i++] = p;
        } 

        init();
    }


    void init()
    {
        // initalize segments with control point pointers
        for (unsigned int i = 0; i < POINTS - 3; ++i)
        {
            segments[i] = CatmullRom<S, D>{ ctrl_pts + i };
            segments[i].centripetal();
        }
    }

    /**
     * @brief Computes a point on the spline
     * @parameter t interpolation between control points p1 and p2
     * @returns point on curve at time t
     */
    Vec<S, D> point(S t)
    {
        auto s_i = segment_idx(t);
        S s_i_t = path_time(t) - s_i; // time within segment s_i

        return segments[s_i].point(s_i_t);
    }


    S nearest_t(Vec<S, D>& point, S t_min=0, S t_max=1)
    {
        auto min_i = segment_idx(t_min);
        auto max_i = segment_idx(t_max);
        S t = {};
        
        for (unsigned int i = min_i; i <= max_i; ++i)
        {
            t = segments[i].nearest_t(point);

            if (t > 0.001 || t < 0.999) { break; }
        }

        return t;
    }


    S arc_length(S t_start=0, S t_end=1, unsigned int steps=10)
    {
        auto min_i = segment_idx(t_start);
        auto max_i = segment_idx(t_end);
        auto min_path_t = path_time(t_start) - min_i;
        auto max_path_t = path_time(t_end) - max_i;
        S len = segments[min_i].arc_length(min_path_t);

        for (int i = min_i + 1; i < max_i; ++i)
        {
            len += segments[i].arc_length();
        }

        return len + segments[max_i].arc_length(0, max_path_t);
    }

    inline float path_time(S t)
    {
        return (POINTS - 3) * t; // path time, as a sum of all segment
                                 // ranges [0,1]
    }

    inline unsigned int segment_idx(S t)
    {
        return path_time(t); // index of the current segment to sample
    }

private:
    Vec<S, D> ctrl_pts[POINTS];
    CatmullRom<S, D> segments[POINTS - 3];
};

}
}
