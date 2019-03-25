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
 * @brief      Vision is to be used as a generic interface for implementations
 *             of specific vision sensor devices.
 *
 * @tparam     COLOR_TYPE   Data type used to store actual pixel color values
 * @tparam     COLUMNS      Resolution of the columnar dimensionality of the sensor (usually width)
 * @tparam     ROWS         Resolution of the row dimensionality of the sensor (usually height)
 * @tparam     SPATIAL_UNIT Unit of measure used to represent the sensor's location within the robot's reference frame
 */
template <class COLOR_TYPE, size_t COLUMNS, size_t ROWS = 1, SPATIAL_UNIT=exo::units::MetersSPATIAL_UNIT=exo::units::M>
struct Vision : public exo::device::base<SPATIAL_UNIT>
{
    /**
     * Returns a reference to an underlying array storing the current frame.
     */
    virtual COLOR_TYPE[COLUMNS][ROWS]& visual_frame() = 0;
};

} // class

} // device

} // exo
