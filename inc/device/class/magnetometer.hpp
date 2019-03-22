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
 * @tparam     FIELD_TYPE  Numerical storage type to be used for representing
 *                         magnetic-field values.
 * @tparam     DOF         Number of 'degrees-of-freedom'
 */
template <class FIELD_TYPE, size_t DOF>
struct Magnetometer : public exo::device::base
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
