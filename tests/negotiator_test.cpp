#include "exo.hpp"
#include "nix.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Tests the pipeline negotiator."

#include "test.h"
{
    (void)argc; (void)argv;
    bool is_sender = false;

    exo::nix::CLI::parser(argc, argv)
    .optional<bool>("--sender", [&](bool value) { (void)value; is_sender = true; });

    if (is_sender)
    {
        exo::nix::Negotiator::instance(0)
        .request("foo", "bar")
        .request("answer", "42")
        .send_request();

        info("Sender finished\n");
    }
    else
    {
        std::string foo, answer;

        exo::nix::Negotiator::instance(2000)
        .react_to("foo", [&](std::string value) { foo = value; })
        .react_to("answer", [&](std::string value) { answer = value; });
        assert(foo == "bar");
        assert(answer == "42");
        info("Receiver finished\n");
    }

    return 0;
}
