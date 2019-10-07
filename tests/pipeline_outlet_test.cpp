#include "exo.hpp"
#include "nix.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Tests the pipeline implementation for outlets."

#include "test.h"
{
    (void)argc; (void)argv;
    exo::nix::Pipeline::Out outlet;
    exo::msg::Payload<sizeof(uint8_t)> pay;

    uint8_t byte_out = 42;

    pay.put<uint8_t>(byte_out);
    outlet << pay.buffer();

    return 0;
}
