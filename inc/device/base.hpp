#pragma once

#include "exo.hpp"
#include "math/linalg.hpp"
#include "units.hpp"

namespace exo
{
namespace device
{

/**
 * @brief      exo::device::Base is intended to be used as the super class for
 *             sensors and other such devices.
 *
 * @tparam     FRAME_UNIT  Unit type used to represent the coordinates in the
 *             device's reference frame.
 */
template <class FRAME_UNIT>
struct Base
{
    /**
     * Matrix which represents the device's location within the robot's
     * local reference frame.
     */
    exo::math::Mat<FRAME_UNIT, 4, 4> spatial_frame;

    /**
     * @brief      Performs generic initialization of the device.
     *
     * @return     exo::Result::OK if initialization was successful.
     */
    virtual Result initialize() = 0;

    /**
     * @brief      Performs a generic reset of the device to some known values.
     *
     * @return     exo::Result::OK if reset was successful.
     */
    virtual Result reset() = 0;
};

} // device

} // exo
