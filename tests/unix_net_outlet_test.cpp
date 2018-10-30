#include "exo.hpp"
#include "unix.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Tests the network implementation for outlets."

#include "test.h"
{
    exo::Log::instance(new exo::unix::Log::Stderr(5, true), 5);
    exo::unix::Net::Out outlet("127.0.0.1", 1337);
    exo::msg::Payload<sizeof(uint8_t)> pay;
    uint8_t byte_out = 42;

    pay << byte_out;
    while((outlet << pay.buffer()) != exo::Result::OK);

    return 0;
}
