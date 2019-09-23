#pragma once

#include "../../exo.hpp"
#include "../../datastructures.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>

#include <iostream>

#define EXO_NIX_UDP_LEN 65535

namespace exo
{
namespace nix
{
namespace net
{

//------------------------------------------------------------------------------
//    _  _     _     _   _    ___       _
//   | \| |___| |_  (_) (_)  / _ \ _  _| |_
//   | .` / -_)  _|  _   _  | (_) | || |  _|
//   |_|\_\___|\__| (_) (_)  \___/ \_,_|\__|
//
struct UDP
{
struct Out : public exo::msg::Outlet
{
    Out() = default;

    Out(const Out& i)
    {
        strcpy(_addr, i._addr);
        _port = i._port;
        _socket = ::socket(AF_INET, SOCK_DGRAM, 0);
    }

    Out(const char* dst_addr, uint16_t port)
    {
        strcpy(_addr, dst_addr);
        _port = port;
        _socket = ::socket(AF_INET, SOCK_DGRAM, 0);
    }

    ~Out()
    {
        close(_socket);
    }

    void set_dst_addr(const char* dst_addr)
    {
        strcpy(_addr, dst_addr);
    }

    void set_dst_port(uint16_t port)
    {
        _port = port;
    }

    exo::Result operator<<(exo::msg::Hdr const& h)
    {
        fd_set w_fds;
        FD_ZERO(&w_fds);
        FD_SET(_socket, &w_fds);

        struct timeval timeout = {
            _timeout_ms / 1000,
            (_timeout_ms % 1000) * 1000
        };

        switch(select(
            _socket + 1,                     // largest fd no + 1
            nullptr,                         // read fds
            &w_fds,                          // write fds including our socket
            nullptr,                         // error fds
            _timeout_ms ? &timeout : nullptr // pointer to timeout struct
            ))
        {
            case -1: // error
                return Result::WRITE_ERR;
            case 0: // timeout
                return Result::TIMEOUT;
            case 1: // ok to write!
                if (write(_socket, &h, sizeof(h)) == sizeof(h))
                {
                    return Result::OK;
                }
        }

        return Result::WRITE_ERR;
    }

    exo::Result operator<<(exo::msg::PayloadBuffer const& pay)
    {
        exo::Log::info(4, "Writing to: " + std::to_string(_port));
        auto to_write = pay.len;

        auto res = chunker::writer(_socket, (uint8_t*)pay.buf, to_write, [&](int fd, uint8_t* buf, size_t to_write) {
            struct sockaddr_in host_addr;
            if (!get_sockaddr(&host_addr))
            {
                return (ssize_t)0;
            }

            auto written = sendto(fd, buf, to_write, 0, (const struct sockaddr *)&host_addr, sizeof(host_addr));

            if (written < 0)
            {
                exo::Log::error(4, "Error writing: " + std::string(strerror(errno)));
            }

            return written;
        });

        if (res != exo::Result::OK)
        {
            return res;
        }

        return res;
    }

    void timeout (unsigned int timeout_ms)
    {
        _timeout_ms = timeout_ms;
    }
private:
    char _addr[64] = {};
    uint16_t _port;
    int _socket;
    int _timeout_ms = 0;

    bool get_sockaddr(struct sockaddr_in* host_addr)
    {
        // resolve host
        auto host = gethostbyname(_addr);
        if (host == nullptr)
        {
            exo::Log::error(4, "Host resolution failed");
            return false;
        }

        // fill in host_addr with resolved info
        bcopy(
            (char *)host->h_addr,
            (char *)&host_addr->sin_addr.s_addr,
            host->h_length
        );
        host_addr->sin_port   = htons(_port);
        host_addr->sin_family = AF_INET;

        return true;
    }
};

//------------------------------------------------------------------------------
//    _  _     _     _   _   ___
//   | \| |___| |_  (_) (_) |_ _|_ _
//   | .` / -_)  _|  _   _   | || ' \
//   |_|\_\___|\__| (_) (_) |___|_||_|
//
struct In : public exo::msg::Inlet
{
    struct Client : public exo::msg::Inlet
    {
        bool corrupt = false;

        int sock() { return _sock; }

        bool got_header() { return _hdr.sanity == exo::msg::sanity; }

        bool got_payload() { return _got_payload; }

        virtual int client_read(int fd, void* dst, size_t size) = 0;

        Client() = default;

        Client(int sock)
        {
            _sock = sock;
        }

        Client(Client& c)
        {
            _sock = c._sock;
        }

        exo::Result operator>>(msg::Hdr& h)
        {
            if (client_read(_sock, &h, sizeof(h)) != sizeof(h))
            {
                return exo::Result::OUT_OF_DATA;
            }

            if (h.sanity != exo::msg::sanity)
            {
                corrupt = true;
                return exo::Result::CORRUPTION;
            }

            _hdr = h;

            return exo::Result::OK;
        }

        exo::Result operator>>(msg::PayloadBuffer const& pay)
        {
            auto to_read = pay.len;
            auto end = (uint8_t*)pay.buf;

            auto res = chunker::reader(_sock, end, to_read, [&](int fd, uint8_t* buf, size_t to_read) {
                return client_read(fd, buf, to_read);
            });

            if (res != exo::Result::OK)
            {
                return res;
            }

            _got_payload = true;

            return res;
        }

        exo::Result flush(size_t bytes)
        {
            char junk[1024];

            while (bytes > 0)
            {
                auto to_read = bytes > sizeof(junk) ? sizeof(junk) : bytes;
                auto bytes_read = read(_sock, &junk, to_read);

                if (bytes_read <= 0)
                {
                    return Result::READ_ERR;
                }

                bytes -= bytes_read;
            }

            return Result::OK;
        }

        exo::Result forward(msg::Hdr& hdr, exo::msg::Outlet* outlets[])
        {

            { // forward the header
                exo::msg::Payload<sizeof(msg::Hdr)> hdr_payload;
                auto hdr_buf = hdr_payload.buffer();
                memcpy(hdr_buf.buf, &hdr, hdr_buf.len);

                for (int i = 0; outlets[i] != nullptr; ++i)
                {
                    if (outlets[i]->allows_msg(hdr) == false) { continue; }

                    auto res = (*outlets[i]) << hdr_payload.buffer();
                    if (res != exo::Result::OK) { return res; }
                }
            }

            // write the whole payload
            size_t bytes = hdr.payload_length;
            while (bytes > 0)
            {
                exo::msg::Payload<4096> block;

                // read each section of the payload by block sized chunks
                auto pay = block.buffer();
                auto to_read = bytes > pay.len ? pay.len : bytes;
                auto bytes_read = client_read(_sock, pay.buf, to_read);

                if (bytes_read < 0)
                {
                    return Result::READ_ERR;
                }
                else
                {
                    bytes -= bytes_read;

                    for (int i = 0; outlets[i] != nullptr; ++i)
                    {
                        if (outlets[i]->allows_msg(hdr) == false) { continue; }

                        auto pay = block.buffer();
                        pay.len = bytes_read; // send only the bytes read

                        auto res = (*outlets[i]) << pay;
                        if (res != exo::Result::OK) { return res; }
                    }
                }
            }

            return Result::OK;
        }

        bool operator==(Client& c) { return c._sock == _sock; }
    protected:
        int _sock = -1;
        exo::msg::Hdr _hdr = {};
        bool _got_payload  = false;
    };

    In() = default;

    In(In& i) = default;

    In(uint16_t port)
    {
        this->_port = port;
    }

    ~In()
    {
        close(_listen_sock);
    }

    int udp_read(void* dst, size_t len)
    {
        if (_last_packet.size() <= 0)
        {
            // fetch a new packet store it in the buffer to incremental reading
            int bytes_read = recvfrom(
                _listen_sock, _last_packet.buf,
                sizeof(_last_packet.buf),
                0,  nullptr, nullptr
            );

            _last_packet.end = (size_t)bytes_read;
            _last_packet.start = 0;
        }

        return _last_packet.dequeue((uint8_t*)dst, len);
    }

    exo::Result operator>>(exo::msg::Hdr& h)
    {
        if (!is_setup())
        {
            auto setup_res = setup();
            if (setup_res != Result::OK)
            {
                return setup_res;
            }
        }

        return udp_read(&h, sizeof(h)) == sizeof(h) ? Result::OK : Result::READ_ERR;
    }

    exo::Result operator>>(exo::msg::PayloadBuffer const& pay)
    {
        if (!is_setup())
        {
            auto setup_res = setup();
            if (setup_res != Result::OK)
            {
                return setup_res;
            }
        }

        return udp_read(pay.buf, pay.len) == (int)pay.len ? Result::OK : Result::READ_ERR;
    }

    exo::Result flush(size_t bytes)
    {
        uint8_t buf[EXO_NIX_UDP_LEN];
        udp_read(buf, bytes);

        return Result::OK;
    }

    exo::Result forward(msg::Hdr& hdr, exo::msg::Outlet* outlets[])
    {
        { // forward the header
            exo::msg::Payload<sizeof(msg::Hdr)> hdr_payload;
            auto hdr_buf = hdr_payload.buffer();
            memcpy(hdr_buf.buf, &hdr, hdr_buf.len);

            for (int i = 0; outlets[i] != nullptr; ++i)
            {
                if (outlets[i]->allows_msg(hdr) == false) { continue; }

                auto res = (*outlets[i]) << hdr_payload.buffer();
                if (res != exo::Result::OK) { return res; }
            }
        }

        // write the whole payload
        size_t bytes = hdr.payload_length;
        while (bytes > 0)
        {
            exo::msg::Payload<4096> block;

            // read each section of the payload by block sized chunks
            auto pay = block.buffer();
            auto to_read = bytes > pay.len ? pay.len : bytes;
            auto bytes_read = udp_read(pay.buf, to_read);

            if (bytes_read < 0)
            {
                return Result::READ_ERR;
            }
            else
            {
                bytes -= bytes_read;

                for (int i = 0; outlets[i] != nullptr; ++i)
                {
                    if (outlets[i]->allows_msg(hdr) == false) { continue; }

                    auto pay = block.buffer();
                    pay.len = bytes_read; // send only the bytes read

                    auto res = (*outlets[i]) << pay;
                    if (res != exo::Result::OK) { return res; }
                }
            }
        }

        return Result::OK;
    }

    void timeout (unsigned int timeout_ms)
    {
        _timeout_ms = timeout_ms;
    }
private:
    uint16_t _port;
    int _listen_sock = -1;
    int _timeout_ms = 0;

    struct {
        uint8_t buf[EXO_NIX_UDP_LEN];
        size_t start = 0, end = 0;

        size_t size() { return end - start; }

        int dequeue(uint8_t* b, size_t size)
        {
            auto bytes_left = end - start;
            if (size < bytes_left)
            {
                memcpy(b, buf + start, size);
                start += size;
                return size;
            }
            else
            {
                memcpy(b, buf + start, bytes_left);
                start = end = 0;
                return bytes_left;
            }
        }
    } _last_packet;

    Result setup()
    {
        _listen_sock = ::socket(AF_INET, SOCK_DGRAM, 0);

        if (_listen_sock < 0) { return Result::RESOURCE_CREATE_FAILED; }

        struct sockaddr_in name = {};
        name.sin_family      = AF_INET;
        name.sin_port        = htons(_port);
        name.sin_addr.s_addr = htonl(INADDR_ANY);

        int use = 1;
        if (setsockopt(_listen_sock, SOL_SOCKET, SO_REUSEPORT, (char*)&use, sizeof(use)))
        {
            exo::Log::error(4, "Setting SO_REUSEPORT to listen socket failed");
            close(_listen_sock);
            return Result::ERROR;
        }

        if (bind(_listen_sock, (const struct sockaddr*)&name, sizeof(name)))
        {
            exo::Log::error(4, "Binding to port failed");
            close(_listen_sock);
            _listen_sock = -1;
            return Result::BIND_FAILED;
        }

        return Result::OK;
    }

    bool is_setup()
    {
        return _listen_sock > 0;
    }
};

};

}

}

}
