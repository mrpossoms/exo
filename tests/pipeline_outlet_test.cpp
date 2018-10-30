#include "exo.hpp"
#include "unix.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Tests the pipeline implementation for outlets."

#include "test.h"
{
    exo::unix::Pipeline::Out outlet;
    exo::msg::Payload<sizeof(uint8_t)> pay; 

    uint8_t byte_out = 42;

    pay << byte_out;
    outlet << pay.buffer();

    return 0;
}
