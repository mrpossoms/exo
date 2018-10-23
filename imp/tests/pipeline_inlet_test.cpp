#include "exo.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Tests the pipeline implementation for inlets."

#include "test.h"
{
    exo::Mod mod("test_mod");
    exo::msg::Inlet inlet(mod);
    uint8_t byte_in = 0;

    inlet >> byte_in;
    assert(byte_in == 42);

    return 0;
}
