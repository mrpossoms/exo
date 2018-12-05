#include "exo.hpp"
#include "nix.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Tests the pipeline implementation for inlets."

#include "test.h"
{
    exo::nix::Pipeline::In inlet;
    exo::msg::Payload<sizeof(uint8_t)> pay;
    
    uint8_t byte_in;
    inlet >> pay.buffer();
    pay >> byte_in;

    assert(byte_in == 42);

    return 0;
}
