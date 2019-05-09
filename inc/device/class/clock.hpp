#pragma once

#include "../base.hpp"

namespace exo
{
namespace device
{
namespace cls
{

/**
 * @brief      Clock is a generic time keeping interface to be used
 *             as the base for implementations of specific clock hardware.
 *
 * @tparam     TIME_UNIT Unit of measure used to represent the sensor's location within the robot's reference frame
 */
template <class TIME_UNIT=exo::units::Second<float>>
struct Clock : public exo::device::Base<TIME_UNIT>
{
    /**
     * @brief      Used to retrieve an rotation-rate vector.
     *
     * @return     The most recent rotation-rate measurement.
     */
    virtual TIME_UNIT now() = 0;
};

} // class

} // device

} // exo
