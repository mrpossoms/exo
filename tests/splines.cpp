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

    info("C(0): %s\n",   spline.point(0).to_string().c_str());
    info("C(0.5): %s\n", spline.point(0.5).to_string().c_str());
    info("C(1): %s\n",   spline.point(1).to_string().c_str());

    spline.uniform();
    info("dC(0): %s\n",   spline.tangent(0).to_string().c_str());
    info("dC(0.5): %s\n", spline.tangent(0.5).to_string().c_str());
    info("dC(1): %s\n",   spline.tangent(1).to_string().c_str());


    return 0;
}
