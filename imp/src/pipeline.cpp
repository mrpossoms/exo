#include "exo.hpp"

#include <unistd.h>

#include <iostream>

#define READ_STREAM_VAL(fd, val) read((fd), &val, sizeof(val))
#define WRITE_STREAM_VAL(fd, val) write((fd), &val, sizeof(val))

using namespace exo::msg;

struct Outlet::impl {
    int fd;
};

Outlet::Outlet(Mod& m, std::initializer_list<ID>&& receipients)
{
    _pimpl = new impl{STDOUT_FILENO};
}

Outlet::~Outlet()
{
    delete _pimpl;
}

Outlet& Outlet::operator<<(Hdr& h)
{
    return *this << h;
}

Outlet& Outlet::operator<<(Payload* p)
{
    return *this << p;
}

template<typename T>
Outlet& Outlet::put(T& structure)
{
    WRITE_STREAM_VAL(_pimpl->fd, structure);
    return *this;
}

Outlet& Outlet::operator<<(int8_t& val)
{
    WRITE_STREAM_VAL(_pimpl->fd, val);
    return *this;
}

Outlet& Outlet::operator<<(uint8_t& val)
{
    WRITE_STREAM_VAL(_pimpl->fd, val);
    return *this;
}

Outlet& Outlet::operator<<(int16_t& val)
{
    WRITE_STREAM_VAL(_pimpl->fd, val);
    return *this;
}

Outlet& Outlet::operator<<(uint16_t& val)
{
    WRITE_STREAM_VAL(_pimpl->fd, val);
    return *this;
}

Outlet& Outlet::operator<<(int32_t& val)
{
    WRITE_STREAM_VAL(_pimpl->fd, val);
    return *this;
}

Outlet& Outlet::operator<<(uint32_t& val)
{
    WRITE_STREAM_VAL(_pimpl->fd, val);
    return *this;
}

Outlet& Outlet::operator<<(int64_t& val)
{
    WRITE_STREAM_VAL(_pimpl->fd, val);
    return *this;
}

Outlet& Outlet::operator<<(uint64_t& val)
{
    WRITE_STREAM_VAL(_pimpl->fd, val);
    return *this;
}

Outlet& Outlet::operator<<(float& val)
{
    WRITE_STREAM_VAL(_pimpl->fd, val);
    return *this;
}

Outlet& Outlet::operator<<(double& val)
{
    WRITE_STREAM_VAL(_pimpl->fd, val);
    return *this;
}

struct Inlet::impl {
    int fd;
};

Inlet::Inlet(Mod& m)
{
    _pimpl = new impl{STDIN_FILENO};
}

Inlet::~Inlet()
{
    delete _pimpl;
}

Inlet& Inlet::operator>>(Hdr& h)
{
    return *this >> h;
}

Inlet& Inlet::operator>>(Payload* p)
{
    return *this >> p;
}

Inlet& Inlet::operator>>(int8_t& val)
{
    READ_STREAM_VAL(_pimpl->fd, val);
    return *this;
}

Inlet& Inlet::operator>>(uint8_t& val)
{
    READ_STREAM_VAL(_pimpl->fd, val);
    return *this;
}

Inlet& Inlet::operator>>(int16_t& val)
{
    READ_STREAM_VAL(_pimpl->fd, val);
    return *this;
}

Inlet& Inlet::operator>>(uint16_t& val)
{
    READ_STREAM_VAL(_pimpl->fd, val);
    return *this;
}

Inlet& Inlet::operator>>(int32_t& val)
{
    READ_STREAM_VAL(_pimpl->fd, val);
    return *this;
}

Inlet& Inlet::operator>>(uint32_t& val)
{
    READ_STREAM_VAL(_pimpl->fd, val);
    return *this;
}

Inlet& Inlet::operator>>(int64_t& val)
{
    READ_STREAM_VAL(_pimpl->fd, val);
    return *this;
}

Inlet& Inlet::operator>>(uint64_t& val)
{
    READ_STREAM_VAL(_pimpl->fd, val);
    return *this;
}

Inlet& Inlet::operator>>(float& val)
{
    READ_STREAM_VAL(_pimpl->fd, val);
    return *this;
}

Inlet& Inlet::operator>>(double& val)
{
    READ_STREAM_VAL(_pimpl->fd, val);
    return *this;
}

