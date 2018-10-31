#include "exo.hpp"
#include "unix.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Tests the network implementation for outlets."

#include "test.h"
{
    exo::Log::instance(new exo::unix::Log::Stderr(5, true), 5);
    exo::unix::Net::Out outlet("127.0.0.1", 1337);
    exo::msg::Payload<sizeof(uint8_t)> pay;
    uint8_t byte_out = 42;
    bool continuous = false;

    exo::unix::CLI::parser(argc, argv).optional<bool>("-c", [&](bool c){
        continuous = c;
        exo::Log::info(4, "continuous: " + std::to_string(c));
    });

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
