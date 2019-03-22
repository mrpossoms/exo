#pragma once

#include "exo.hpp"

namespace exo
{
namespace device
{
namespace bus
{

struct I2C
{
    struct DeviceAction : public I2C
    {
        DeviceAction(I2C& i2c, uint16_t dev_addr) : I2C(i2c)
        {
            address_device(dev_addr);
        }

        template<MSG_PAYLOAD>
        Result write(MSG_PAYLOAD& payload)
        {
            return write(static_cast<void*>(&payload), sizeof(MSG_PAYLOAD));
        }

        template<MSG_PAYLOAD>
        Result read(MSG_PAYLOAD& payload)
        {
            return read(static_cast<void*>(&payload), sizeof(MSG_PAYLOAD));
        }

        DeviceAction& reg(uint32_t reg_addr)
        {
            address_register(reg_addr);
            return *this;
        }

    };

    DeviceAction dev(uint16_t dev_addr)
    {
        return DeviceAction(*this, dev_addr);
    }

protected:
    virtual Result address_device(uint16_t dev_addr) = 0;
    virtual Result address_register(uint32_t reg_addr) = 0;
    virtual Result write(void* buf, size_t size) = 0;
    virtual Result read(void* buf, size_t size) = 0;
};

} // class

} // device

} // exo
