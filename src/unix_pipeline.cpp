#include "exo.hpp"
#include "unix.hpp"

#include <unistd.h>

#include <iostream>


using namespace exo::unix;
using namespace exo;

Result Pipeline::Out::operator<<(msg::Hdr&& h)
{
    if (write(STDOUT_FILENO, &h, sizeof(h)) == sizeof(h))
    {
        return Result::OK;
    }

    return Result::WRITE_ERR;
}

Result Pipeline::Out::operator<<(msg::PayloadBuffer&& pay)
{
    if (write(STDOUT_FILENO, pay.buf, pay.len) == pay.len)
    {
        return Result::OK;
    }

    return Result::WRITE_ERR;
}


Result Pipeline::In::operator>>(msg::Hdr& h)
{
    if (read(STDIN_FILENO, &h, sizeof(h)) == sizeof(h))
    {
        return Result::OK;
    }

    return Result::READ_ERR;
}

Result Pipeline::In::operator>>(msg::PayloadBuffer&& pay)
{
    if (read(STDIN_FILENO, pay.buf, pay.len) == pay.len)
    {
        return Result::OK;
    }

    return Result::READ_ERR;
}

Result Pipeline::In::flush(size_t bytes)
{
    if (lseek(STDIN_FILENO, bytes, SEEK_CUR) > -1)
    {
        return Result::OK;
    }

    return Result::BAD;
}
