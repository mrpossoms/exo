#include "exo.hpp"
#include "device/class/accelerometer.hpp"
#include "device/class/depth.hpp"
#include "device/class/vision.hpp"
#include "device/class/gyro.hpp"
#include "device/class/magnetometer.hpp"
#include "device/class/orientation.hpp"
#include "device/class/position.hpp"

#include <iostream>

#define DESCRIPTION "Tests exo device abstractions"

using namespace exo;
using namespace exo::math;
using namespace exo::device::cls;

struct DummyAccel : public Accelerometer<float, 3>
{
    Result initialize() { return Result::OK; }

    Result reset() { return Result::OK; }

    Vec<float, 3> acceleration() { return {}; }
};

struct DummyDepth : public Depth<float, 64, 32>
{
    Result initialize() { return Result::OK; }

    Result reset() { return Result::OK; }

    Mat<float, 32, 64>& depth_frame() { return frame; }
};

struct DummyCamera : public Vision<Vec<uint8_t, 3>, 64, 32>
{
    Result initialize() { return Result::OK; }

    Result reset() { return Result::OK; }

    Mat<Vec<uint8_t, 3>, 32, 64>& visual_frame() { return frame; }
};

struct DummyGyro : public Gyro<units::Radians<float>, 3>
{
    Result initialize() { return Result::OK; }

    Result reset() { return Result::OK; }

    Vec<units::Radians<float>, 3> rate() { return {}; }
};

struct DummyMag : public Magnetometer<units::Tesla<float>, 3>
{
    Result initialize() { return Result::OK; }

    Result reset() { return Result::OK; }

    Vec<units::Tesla<float>, 3> field() { return {}; }
};

struct DummyOri : public Orientation<>
{
    Result initialize() { return Result::OK; }

    Result reset() { return Result::OK; }

    Quat orientation() { return {}; }
};

using GpsCoord = Vec<units::Degree<float>, 2>;
struct DummyPosition : public Position<GpsCoord>
{
    Result initialize() { return Result::OK; }

    Result reset() { return Result::OK; }

    GpsCoord position() { return {}; }
};

#include "test.h"
{
    { // Accel test
        DummyAccel d;

        assert(d.initialize() == Result::OK);
        assert(d.reset() == Result::OK);
    }

    { // Depth test
        DummyDepth d;

        assert(d.initialize() == Result::OK);
        assert(d.reset() == Result::OK);
    }

    { // cam test
        DummyCamera d;

        assert(d.initialize() == Result::OK);
        assert(d.reset() == Result::OK);
    }

    { // Gyro test
        DummyGyro d;

        assert(d.initialize() == Result::OK);
        assert(d.reset() == Result::OK);
    }

    { // Mag test
        DummyMag d;

        assert(d.initialize() == Result::OK);
        assert(d.reset() == Result::OK);
    }

    { // Mag test
        DummyOri d;

        assert(d.initialize() == Result::OK);
        assert(d.reset() == Result::OK);
    }

    return 0;
}
