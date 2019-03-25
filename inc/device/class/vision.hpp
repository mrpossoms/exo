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
 * @brief      Vision is to be used as a generic interface for implementations
 *             of specific vision sensor devices.
 *
 * @tparam     COLOR_TYPE  Data type used to store actual pixel color values
 * @tparam     COLUMNS     Resolution of the columnar dimensionality of the sensor (usually width)
 * @tparam     ROWS        Resolution of the row dimensionality of the sensor (usually height)
 */
template <class COLOR_TYPE, size_t COLUMNS, size_t ROWS = 1>
struct Vision : public exo::device::base
{
    /**
     * Returns a reference to an underlying array storing the current frame.
     */
    virtual COLOR_TYPE[COLUMNS][ROWS]& visual_frame() = 0;
};

} // class

} // device

} // exo
