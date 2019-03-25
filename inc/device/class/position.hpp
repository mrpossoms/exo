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
 * @brief      Position is to be used as a generic interface to be used
 *             as the base for implementations of devices, that produce
 *             measurements of position in space.
 *
 * @tparam     POSITION_TYPE  Data type representing the position in space
 */
template <class POSITION_TYPE>
struct position : public exo::device::base
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
