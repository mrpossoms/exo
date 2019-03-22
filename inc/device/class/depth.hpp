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
 * @brief      Depth is to be used as a generic interface for implementations
 *             of specific depth sensor devices.
 *
 * @tparam     DEPTH_TYPE  Data type used to store actual depth measurements
 * @tparam     COLUMNS     Resolution of the columnar dimensionality of the sensor (usually width)
 * @tparam     ROWS        Resolution of the row dimensionality of the sensor (usually height)
 */
template <class DEPTH_TYPE, size_t COLUMNS, size_t ROWS = 1>
struct Depth : public exo::device::base
{
    /**
     * Returns a reference to an underlying array storing the current depth frame.
     */
    virtual DEPTH_TYPE[COLUMNS][ROWS]& depth_frame() = 0;
};

} // class

} // device

} // exo
