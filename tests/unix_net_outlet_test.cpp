#include "exo.hpp"
#include "nix.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Tests the network implementation for outlets."

#include "test.h"
{
    exo::Log::instance(new exo::nix::Log::Stderr(5, true), 5);
    exo::nix::Net::Out outlet("127.0.0.1", 1337);
    exo::msg::Payload<sizeof(uint8_t) + sizeof(exo::msg::Hdr)> pay;
    uint8_t byte_out = 42;
    bool continuous = false;

    exo::nix::CLI::parser(argc, argv).optional<bool>("-c", [&](bool c){
        continuous = c;
        exo::Log::info(4, "continuous: " + std::to_string(c));
    });

    pay << exo::msg::Hdr{};
    pay << byte_out;

    if (continuous)
    {
        while(true) outlet << pay.buffer();
    }
    else
    {
        while((outlet << pay.buffer()) != exo::Result::OK);
    }

    return 0;
}
