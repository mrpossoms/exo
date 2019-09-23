#include "exo.hpp"
#include "nix.hpp"
#include "msg.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Tests the network implementation for inlets."

#include "unix_net_util.hpp"

#include "test.h"
{
    exo::Log::inst(new exo::nix::Log::Stderr(5, true), 5);

    bool continuous = false;
    uint16_t inlet_port = 31338;

    { // parse arguments
        exo::nix::CLI::parser(argc, argv)
        .optional<bool>("-c", [&](bool c){
            continuous = c;
            exo::Log::info(4, "continuous: " + std::to_string(c));
        })
        .optional<uint16_t>("--port", [&](uint16_t port) {
            inlet_port = port;
            exo::Log::info(4, "using inlet port: " + std::to_string(port));
        })
        ;
    }

    exo::nix::net::UDP::In inlet(inlet_port);

    { // communicate
        TestMessage msg;
        exo::msg::Payload<sizeof(msg) + sizeof(exo::msg::Hdr)> pay;

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
            while (true)
            {
                if (get_message(inlet, msg) == exo::Result::OK) { break; }
                else { usleep(10000); }
            }

            exo::Log::info(0, "Expected '" PROPER_GREETING "' got '" + std::string(msg.str) + "'");
            assert(strcmp(msg.str, PROPER_GREETING) == 0);
        }
    }

    exo::Log::info(0, "Done!");

    return 0;
}
