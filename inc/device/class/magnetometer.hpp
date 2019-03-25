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
 * @brief      Magnetometer is to be used as a generic interface to be used
 *             as the base for implementations of specific magnetometers.
 *
 * @tparam     FIELD_TYPE   Numerical storage type to be used for representing
 *                          magnetic-field values.
 * @tparam     DOF          Number of 'degrees-of-freedom'
 * @tparam     SPATIAL_UNIT Unit of measure used to represent the sensor's location within the robot's reference frame
 */
template <class FIELD_TYPE, size_t DOF, SPATIAL_UNIT=exo::units::M>
struct Magnetometer : public exo::device::base<SPATIAL_UNIT>
{
    /**
     * @brief      Used to retrieve an magnetic-field vector.
     *
     * @return     The most recent magnetic-field measurement.
     */
    virtual exo::math::Vec<FIELD_TYPE, DOF> magnetic_field() = 0;
};

} // class

} // device

} // exo
