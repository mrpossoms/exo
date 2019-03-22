#pragma once

#include "math/linalg.hpp"
#include "device/base.hpp"

namespace exo
{
namespace device
{
namespace class
{

/**
 * @brief      Orientation is to be used as a generic interface to be used
 *             as the base for implementations of devices, that through some
 *             means, directly produce orientations.
 */

struct Orientation : public exo::device::base
{
    /**
     * @brief      Raw orientation of the sensor.
     *
     * @return     exo::math Quaternion representation of the most recent
     *             orientation.
     */
    virtual exo::math::Quat orientation() = 0;

    /**
     * @brief      If applicable, this returns the rotational reference frame
     *             which the measurement is taken in respect of.
     *
     * @return     exo::math Quaternion representation of the reference frame.
     */
    virtual exo::math::Quat reference_frame() { return {}; }
};

} // class

} // device

} // exo
