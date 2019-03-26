#pragma once

#include "../base.hpp"

namespace exo
{
namespace device
{
namespace cls
{

/**
 * @brief      Depth is to be used as a generic interface for implementations
 *             of specific depth sensor devices.
 *
 * @tparam     DEPTH_TYPE  Data type used to store actual depth measurements
 * @tparam     COLS        Resolution of the columnar dimensionality of the sensor (usually width)
 * @tparam     ROWS        Resolution of the row dimensionality of the sensor (usually height)
 * @tparam     SPATIAL_UNIT Unit of measure used to represent the sensor's location within the robot's reference frame
 */
template <class DEPTH_TYPE, size_t COLS, size_t ROWS = 1, class SPATIAL_UNIT=exo::units::M>
struct Depth : public exo::device::Base<SPATIAL_UNIT>
{
    /**
     * Returns a reference to an underlying matrix storing the current depth frame.
     */
    virtual exo::math::Mat<DEPTH_TYPE, ROWS, COLS>& depth_frame() = 0;

protected:
    math::Mat<DEPTH_TYPE, ROWS, COLS> frame;
};

} // class

} // device

} // exo
