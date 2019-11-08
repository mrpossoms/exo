#include "exo.hpp"
#include "nix.hpp"
#include "math/splines.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Test spline implementions"

using namespace exo::math;

#include "test.h"
{
    (void)argc; (void)argv;

    { // check basic interpolation
        Vec<float, 2> ctrl_pts[] = {
            { 0.0,  0 },
            { 0.333, 0 },
            { 0.666, 0 },
            { 1.0,  0 }
        };

        auto spline = CatmullRom<float, 2>(ctrl_pts);
        spline.uniform();

        Vec<float, 2> middle = { 0.5, 0 };
        assert((spline.point(0) - ctrl_pts[1]).len() < 0.001 );
        assert((spline.point(0.5) - middle).len() < 0.001 );
        assert((spline.point(1) - ctrl_pts[2]).len() < 0.001 );

        assert(fabsf(spline.arc_length() - 0.333) < 0.01);

        info("C(0): %s\n",   spline.point(0).to_string().c_str());
        info("C(0.5): %s\n", spline.point(0.5).to_string().c_str());
        info("C(1): %s\n",   spline.point(1).to_string().c_str());

        spline.uniform();
        info("dC(0): %s\n",   spline.tangent(0).to_string().c_str());
        info("dC(0.5): %s\n", spline.tangent(0.5).to_string().c_str());
        info("dC(1): %s\n",   spline.tangent(1).to_string().c_str());
    }

    { // check nearest time searches
        Vec<float, 2> ctrl_pts[] = {
            { 0.0,  0 },
            { 0.333, 0 },
            { 0.666, 0 },
            { 1.0,  0 }
        };

        auto spline = CatmullRom<float, 2>(ctrl_pts);
        spline.uniform();

        assert(spline.nearest_t(ctrl_pts[0]) < 0.001);
        assert(spline.nearest_t(ctrl_pts[3]) > 0.999);
    }

    { // check path interpolation
        Vec<float, 2> ctrl_pts[] = {
            { 0.0, 0 },
            { 1.0, 0 },
            { 2.0, 0 },
            { 3.0, 0 },
            { 4.0, 0 },
            { 5.0, 0 },
            { 6.0, 0 },
            { 7.0, 0 },
            { 8.0, 0 },
            { 9.0, 0 },
            {10.0, 0 },
        };

        exo::math::path<11, float, 2> p(ctrl_pts);
        auto point = p.point(0.5);

        assert((point - ctrl_pts[5]).len() < 0.01);
    }

    { // check nearest time searches
        Vec<float, 2> ctrl_pts[] = {
            { 0.0, 0 },
            { 1.0, 0 },
            { 2.0, 0 },
            { 3.0, 0 },
            { 4.0, 0 },
            { 5.0, 0 },
            { 6.0, 0 },
            { 7.0, 0 },
            { 8.0, 0 },
            { 9.0, 0 },
            {10.0, 0 },
        };

        exo::math::path<11, float, 2> p(ctrl_pts);

        assert(p.nearest_t(ctrl_pts[0]) < 0.001);
        assert(p.nearest_t(ctrl_pts[10]) > 0.999);
    }

    return 0;
}
