#include "exo.hpp"
#include "unix.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Tests the network implementation for inlets."

#include "test.h"
{
    exo::Log::instance(new exo::unix::Log::Stderr(5, true), 5);
    exo::unix::Net::In inlet(1337);
    exo::msg::Payload<sizeof(uint8_t)> pay;
    bool continuous = false;

    exo::unix::CLI::parser(argc, argv).optional<bool>("-c", [&](bool c){
        continuous = c;
        exo::Log::info(4, "continuous: " + std::to_string(c));
    });

    uint8_t byte_in;

    if (continuous)
    {
        while(true)
        {
            if ((inlet >> pay.buffer()) == exo::Result::OK)
            {
                pay >> byte_in;
                exo::Log::info(4, "data: " + std::to_string(byte_in));
                assert(byte_in == 42);
            }
        }
    }
    else
    {
        while((inlet >> pay.buffer()) != exo::Result::OK);
        pay >> byte_in;
        assert(byte_in == 42);
    }

    return 0;
}
