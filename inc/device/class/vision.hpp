#pragma once

#include "math/linalg.hpp"
#include "device/base.hpp"

namespace exo
{
namespace device
{
namespace class
{

template <class COLOR_TYPE, size_t COLUMNS, size_t ROWS = 1>
struct Vision : public exo::device::base
{
    virtual COLOR_TYPE[COLUMNS][ROWS]& visual_frame() = 0;
};

} // class

} // device

} // exo
