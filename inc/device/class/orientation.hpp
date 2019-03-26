#pragma once

#include "../base.hpp"

namespace exo
{
namespace device
{
namespace cls
{

/**
 * @brief      Orientation is to be used as a generic interface to be used
 *             as the base for implementations of devices, that through some
 *             means, directly produce orientations.
* @tparam      SPATIAL_UNIT Unit of measure used to represent the sensor's location within the robot's reference frame
 */
template <class SPATIAL_UNIT=exo::units::M>
struct Orientation : public exo::device::Base<SPATIAL_UNIT>
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
