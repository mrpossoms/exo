#include "exo.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Tests the pipeline implementation for outlets."

#include "test.h"
{
    exo::Mod mod("test_mod");
    exo::msg::Outlet outlet(mod, {});

    uint8_t byte_out = 42;

    outlet << byte_out;

    return 0;
}
