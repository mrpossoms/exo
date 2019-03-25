#pragma once

#include "math/linalg.hpp"
#include "device/base.hpp"

namespace exo
{
namespace device
{
namespace cls
{

/**
 * @brief      Position is to be used as a generic interface to be used
 *             as the base for implementations of devices, that produce
 *             measurements of position in space.
 *
 * @tparam     POSITION_TYPE  Data type representing the position in space
 * @tparam     SPATIAL_UNIT Unit of measure used to represent the sensor's location within the robot's reference frame
 */
template <class POSITION_TYPE, SPATIAL_UNIT=exo::units::M>
struct position : public exo::device::base<SPATIAL_UNIT>
{
    /**
     * @brief      Raw position in space of the sensor.
     *
     * @return     Most recent position measurement represented as user type
     *             POSITION_TYPE
     */
    virtual POSITION_TYPE position() = 0;
};

} // class

} // device

} // exo
