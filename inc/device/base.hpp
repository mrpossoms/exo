#pragma once

#include "exo.hpp"

namespace exo
{
namespace device
{

template <class FRAME_UNIT>
struct Base
{
    exo::math::Mat<FRAME_UNIT, 4, 4> spatial_frame;
    virtual Result initialize() = 0;
    virtual Result reset() = 0;
};

} // device

} // exo
