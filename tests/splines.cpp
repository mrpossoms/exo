#include "exo.hpp"
#include "nix.hpp"
#include "math/splines.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Test spline implementions"

using namespace exo::math;

#include "test.h"
{
    Vec<float, 2> ctrl_pts[] = {
        { 0.0,  0 },
        { 0.25, 0 },
        { 0.75, 0 },
        { 1.0,  0 }
    };

    auto spline = CatmullRom<float, 2>(ctrl_pts);
    spline.compute_knots();

    info("t_0: %s\n",   spline.point(0).to_string().c_str());
    info("t_0.5: %s\n", spline.point(0.5).to_string().c_str());
    info("t_1: %s\n",   spline.point(1).to_string().c_str());

    return 0;
}
