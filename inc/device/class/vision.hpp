#pragma once

#include "device/base.hpp"

namespace exo
{
namespace device
{
namespace cls
{

/**
 * @brief      Vision is to be used as a generic interface for implementations
 *             of specific vision sensor devices.
 *
 * @tparam     COLOR_TYPE   Data type used to store actual pixel color values
 * @tparam     COLS         Resolution of the columnar dimensionality of the sensor (usually width)
 * @tparam     ROWS         Resolution of the row dimensionality of the sensor (usually height)
 * @tparam     SPATIAL_UNIT Unit of measure used to represent the sensor's location within the robot's reference frame
 */
template <class COLOR_TYPE, size_t COLS, size_t ROWS = 1, class SPATIAL_UNIT=exo::units::M>
struct Vision : public exo::device::Base<SPATIAL_UNIT>
{
    /**
     * Returns a reference to an underlying array storing the current frame.
     */
    virtual math::Mat<COLOR_TYPE, ROWS, COLS>& visual_frame() = 0;

protected:
    math::Mat<COLOR_TYPE, ROWS, COLS> frame;
};

} // class

} // device

} // exo
