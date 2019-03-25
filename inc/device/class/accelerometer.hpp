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
 * @brief      Accelerometer is to be used as a generic interface to be used
 *             as the base for implementations of specific accelerometers.
 *
 * @tparam     ACCEL_TYPE  Numerical storage type to be used for representing
 *                         acceleration values.
 * @tparam     DOF         Number of 'degrees-of-freedom'
 * @tparam     SPATIAL_UNIT Unit of measure used to represent the sensor's location within the robot's reference frame
 */
template <class ACCEL_TYPE, size_t DOF, class SPATIAL_UNIT=exo::units::M>
struct Accelerometer : public exo::device::Base<SPATIAL_UNIT>
{
    /**
     * @brief      Used to retrieve an acceleration vector.
     *
     * @return     The most recent acceleration measurement.
     */
    virtual exo::math::Vec<ACCEL_TYPE, DOF> acceleration() = 0;
};

} // class

} // device

} // exo
