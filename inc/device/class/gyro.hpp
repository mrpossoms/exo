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
 * @brief      Gyro is to be used as a generic interface to be used
 *             as the base for implementations of specific gyroscopes.
 *
 * @tparam     RATE_TYPE  Numerical storage type to be used for representing
 *                        rotation rate values.
 * @tparam     DOF        Number of 'degrees-of-freedom'
 */
template <class RATE_TYPE, size_t DOF>
struct Gyro : public exo::device::base
{
    /**
     * @brief      Used to retrieve an rotation-rate vector.
     *
     * @return     The most recent rotation-rate measurement.
     */
    virtual exo::math::Vec<RATE_TYPE, DOF> rate() = 0;
};

} // class

} // device

} // exo
