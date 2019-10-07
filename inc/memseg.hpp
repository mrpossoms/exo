#pragma once

#include "../exo.hpp"
#include "utils.hpp"

namespace exo
{

template<size_t SEG_SIZE>
struct memseg
{
exo::msg::Hdr header;
uint8_t segment[SEG_SIZE];

struct inlet : public exo::msg::Inlet
{
    inlet(memseg<SEG_SIZE>* seg) : _seg(seg) {}

    Result operator<<(exo::msg::Hdr const& h)
    {

    }

    Result operator<<(PayloadBuffer const& buf)
    {

    }

    /**
     * @brief      Can be used to filter which messages are permitted to pass through
     *             a specific outlet implementation.
     *
     * @param      hdr   The message header
     *
     * @return     True if the message may be transmitted, false otherwise.
     */
    bool allows_msg(Hdr& hdr) { (void)hdr; return true; }

private:
    memseg<SEG_SIZE>* _seg
};

struct outlet : public exo::msg::Outlet
{
    outlet(memseg<SEG_SIZE>* seg) : _seg(seg) {}

    Result operator>>(Hdr& hdr)
    {

    }

    Result operator>>(PayloadBuffer const& buf)
    {

    }

    Result flush(size_t bytes)
    {

    }


    /**
     * @brief      Takes a message header that's been received over this
     *             outlet, and forwards that header, and the rest of the
     *             message to the outlets pointed to.
     *
     * @param      hdr      The message header
     * @param      outlets  Pointer to a null terminated array of
     *                      outlets to forward to.
     *
     * @return     Result::OK on success, error results are inherited
     *             from each specific outlet implementation.
     */
    Result forward(Hdr& hdr, Outlet* outlets[]) = 0;

private:
    memseg<SEG_SIZE>* _seg
};

};

} // exo namespace
