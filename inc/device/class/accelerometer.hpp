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
 * @brief      Accelerometer is to be used as a generic interface to be used
 *             as the base for implementations of specific accelerometers.
 *
 * @tparam     ACCEL_TYPE  Numerical storage type to be used for representing
 *                         acceleration values.
 * @tparam     DOF         Number of 'degrees-of-freedom'
 */
template <class ACCEL_TYPE, size_t DOF>
struct Accelerometer : public exo::device::base
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
