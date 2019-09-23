#pragma once

#include "../exo.hpp"
#include "../math/linalg.hpp"
#include "../units.hpp"

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
    exo::math::Mat<FRAME_UNIT, 4, 4> transform = {
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 },
    };

    /**
     * If the sensor is non-polling, but rather streams data this lambda can be
     * used handle the event when new data arrives.
     */
    std::function<void (void)> on_data;

    /**
     * @brief      Performs generic initialization of the device.
     *
     * @return     exo::Result::OK if initialization was successful.
     */
    virtual Result initialize() = 0;

    /**
     * @brief      Returns non zero when an error has occurred.
     *
     * @return     0 if no errors have occurred. User defined otherwise
     */
    virtual int error() { return 0; }

    /**
     * @brief      Performs a generic reset of the device to some known values.
     *
     * @return     exo::Result::OK if reset was successful.
     */
    virtual Result reset() = 0;

    /**
     * @brief      Used to explicitly cleanup any device specific resources
     */
    virtual void shutdown() {}
};

} // device

} // exo
