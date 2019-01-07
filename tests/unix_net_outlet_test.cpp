#include "exo.hpp"
#include "nix.hpp"

#include "msg.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Tests the network implementation for outlets."

#include "unix_net_util.hpp"

#include "test.h"
{
    exo::Log::instance(new exo::nix::Log::Stderr(5, true), 5);

    bool        continuous      = false;
    std::string outlet_hostname = { "127.0.0.1" };
    uint16_t    outlet_port     = 31337;

    { // parse arguments
        exo::nix::CLI::parser(argc, argv)
        .optional<bool>("-c", [&](bool c){
            continuous = c;
            exo::Log::info(4, "continuous: " + std::to_string(c));
        })
        .optional<uint16_t>("--port", [&](uint16_t port) {
            outlet_port = port;
        })
        .optional<std::string>("--host", [&](std::string hostname) {
            outlet_hostname = hostname;
        })
        ;
    }

    exo::nix::Net::Out outlet(outlet_hostname.c_str(), outlet_port);

    { // communicate
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
    }

    return 0;
}
