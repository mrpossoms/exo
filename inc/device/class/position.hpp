#pragma once

#include "math/linalg.hpp"
#include "device/base.hpp"

namespace exo
{
namespace device
{
namespace class
{

template <class POSITION_TYPE>
struct position : public exo::device::base
{
    virtual POSITION_TYPE position() = 0;
};

} // class

} // device

} // exo
