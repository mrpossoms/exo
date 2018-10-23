#include "exo.hpp"
#include "unix.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Tests the cli-args implementation."

#include "test.h"
{
    int correct = 0;

    exo::unix::CLI::parser(argc, argv)
        .optional<int>("--foo", [&](int i) {
            std::cout << "foo arg: " << std::to_string(i) << std::endl;

            if (i == 42) correct++;
        })
        .optional<float>("--bar", [&](float s) {
            std::cout << "bar arg: " << std::to_string(s) << std::endl;
            if (s == 13.37f) correct++;
        });

    return !(correct == 2);
}
