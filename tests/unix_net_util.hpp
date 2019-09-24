#pragma once

#include "exo.hpp"
#include "nix.hpp"
#include "msg.hpp"
#include "nix/net/udp.hpp"
#include "nix/net/tcp.hpp"

exo::Result get_message(exo::msg::Inlet& inlet, TestMessage& msg)
{
    exo::msg::Hdr hdr;
    exo::msg::Payload<sizeof(msg)> payload;

    auto res = (inlet >> hdr);
    if (exo::Result::OUT_OF_DATA == res) { return res; }
    if (exo::Result::NOT_READY == res) { return res; }

    if (res == exo::Result::OK)
    {
        // make sure the header is valid
        if (hdr != TestMessage::hdr()) { return exo::Result::INCOMPATIBLE_MESSAGE; }

        // read the payload from the inlet, and extract the message struct
        auto res = inlet >> payload.buffer();
        if (res == exo::Result::OK)
        {
            payload.get<TestMessage>(msg);
        }
    }

    return res;
}

exo::Result send_message(exo::msg::Outlet& outlet, TestMessage& msg)
{
    // create the header, pack the payload
    exo::msg::Payload<sizeof(TestMessage) + sizeof(exo::msg::Hdr)> payload;
    payload << TestMessage::hdr();
    payload.put<TestMessage>(msg);

    // stuff it into the outlet
    return outlet << payload.buffer();
}
