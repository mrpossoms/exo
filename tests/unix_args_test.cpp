#include "exo.hpp"
#include "nix.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Tests the cli-args implementation."

#include "test.h"
{
    (void)argc; (void)argv;
    int correct = 0;

    exo::nix::CLI::parser(argc, argv)
        .help("--help",
              "Tests the functionality of the cli argument parser.")
        .required<int>("--foo", [&](int i) {
            std::cout << "foo arg: " << std::to_string(i) << std::endl;

            if (i == 42) correct++;
        }, "Integer, must be 42 to pass.")
        .required<float>("--bar", [&](float s) {
            std::cout << "bar arg: " << std::to_string(s) << std::endl;
            if (s == 13.37f) correct++;
        }, "Float, must be 13.37 to pass.");

    return !(correct == 2);
}
