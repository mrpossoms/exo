#include "exo.hpp"
#include "nix.hpp"
#include "msg.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Tests the network implementation for inlets."

exo::Result get_message(exo::nix::Net::In& inlet, TestMessage& msg)
{
    exo::msg::Hdr hdr;
    exo::msg::Payload<sizeof(msg)> payload;

    auto res = (inlet >> hdr);
    if (exo::Result::OUT_OF_DATA == res) { return res; }
    if (exo::Result::NOT_READY == res) { return res; }

    // if (res == exo::Result::OK)
    {
        // make sure the header is valid
        if (hdr != TestMessage::hdr()) { return exo::Result::INCOMPATIBLE_MESSAGE; }

        // read the payload from the inlet, and extract the message struct
        auto res = inlet >> payload.buffer();
        if (res == exo::Result::OK)
        {
            payload.get<TestMessage>(msg);
        }

        return res;
    }

}

#include "test.h"
{
    exo::Log::instance(new exo::nix::Log::Stderr(5, true), 5);
    exo::nix::Net::In inlet(1337);
    TestMessage msg;

    exo::msg::Payload<sizeof(msg) + sizeof(exo::msg::Hdr)> pay;
    bool continuous = false;

    exo::nix::CLI::parser(argc, argv).optional<bool>("-c", [&](bool c){
        continuous = c;
        exo::Log::info(4, "continuous: " + std::to_string(c));
    });

    uint8_t byte_in;

    if (continuous)
    {
        while(true)
        {
            TestMessage msg;
            if (get_message(inlet, msg) == exo::Result::OK)
            {
                exo::Log::info(0, std::string(msg.str) + "\n");
            }
        }
    }
    else
    {
        exo::Result res = exo::Result::OK;

        while (true)
        {
            if (get_message(inlet, msg) == exo::Result::OK) { break; }
            else { usleep(10000); }
        }

        assert(strcmp(msg.str, PROPER_GREETING) == 0);
    }

    return 0;
}
