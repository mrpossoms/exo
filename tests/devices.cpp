#include "exo.hpp"
#include "device/class/accelerometer.hpp"

#include <iostream>

#define DESCRIPTION "Tests exo device abstractions"

using namespace exo::math;
using namespace exo::device::cls;

struct DummyAccel : public Accelerometer<float, 3>
{
    exo::Result initialize() { return exo::Result::OK; }

    exo::Result reset() { return exo::Result::OK; }

    Vec<float, 3> acceleration() { return {}; }
};

#include "test.h"
{
    DummyAccel accel;

    assert(accel.initialize() == exo::Result::OK);
    assert(accel.reset() == exo::Result::OK);

    return 0;
}
