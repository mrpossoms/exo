#include "exo.hpp"
#include "nix.hpp"

#include "msg.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Tests the network implementation for outlets."

exo::Result send_message(exo::nix::Net::Out& outlet, TestMessage& msg)
{
    // create the header, pack the payload
    exo::msg::Payload<sizeof(TestMessage) + sizeof(exo::msg::Hdr)> payload;
    payload << TestMessage::hdr();
    payload.put<TestMessage>(msg);

    // stuff it into the outlet
    return outlet << payload.buffer();
}

#include "test.h"
{
    exo::Log::instance(new exo::nix::Log::Stderr(5, true), 5);
    exo::nix::Net::Out outlet("127.0.0.1", 31337);
    bool continuous = false;

    exo::nix::CLI::parser(argc, argv).optional<bool>("-c", [&](bool c){
        continuous = c;
        exo::Log::info(4, "continuous: " + std::to_string(c));
    });

    TestMessage msg;
    strcpy(msg.str, PROPER_GREETING);

    if (continuous)
    {
        while(true) send_message(outlet, msg);
    }
    else
    {
        while((send_message(outlet, msg)) != exo::Result::OK);
    }

    return 0;
}
