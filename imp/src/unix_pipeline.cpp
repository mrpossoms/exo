#include "exo.hpp"
#include "unix.hpp"

#include <unistd.h>

#include <iostream>


using namespace exo::unix;

exo::msg::Outlet& Pipeline::Out::operator<<(exo::msg::Hdr& h)
{
    write(STDOUT_FILENO, &h, sizeof(h));
    return *this;
}

exo::msg::Outlet& Pipeline::Out::operator<<(exo::msg::PayloadBuffer&& pay)
{
    write(STDOUT_FILENO, pay.buf, pay.len);
    return *this;
}


exo::msg::Inlet& Pipeline::In::operator>>(exo::msg::Hdr& h)
{
    read(STDIN_FILENO, &h, sizeof(h));
    return *this;
}

exo::msg::Inlet& Pipeline::In::operator>>(exo::msg::PayloadBuffer&& pay)
{
    read(STDIN_FILENO, pay.buf, pay.len);
    return *this;
}

exo::msg::Inlet& Pipeline::In::flush(size_t bytes)
{
    lseek(STDIN_FILENO, bytes, SEEK_CUR);
    return *this;
}
