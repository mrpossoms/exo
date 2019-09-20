#pragma once

#include "../exo.hpp"
#include "utils.hpp"

namespace exo
{
namespace nix
{

struct Pipeline
{
    struct Out : public exo::msg::Outlet
    {
        Out() = default;
        Out(int fd) : _fd(fd) {}

        exo::Result operator<<(exo::msg::Hdr const& h)
        {
		    if (write(_fd, &h, sizeof(h)) == sizeof(h))
		    {
		        return Result::OK;
		    }

		    return Result::WRITE_ERR;
        }

        exo::Result operator<<(exo::msg::PayloadBuffer const& pay)
        {
            return exo::nix::chunker::writer(_fd, (uint8_t*)pay.buf, pay.len);
        }

    private:
        int _fd = STDOUT_FILENO;
    };

    struct In : public exo::msg::Inlet
    {
        In() = default;
        In(int fd) : _fd(fd) {}

        exo::Result operator>>(exo::msg::Hdr& h)
        {
		    if ((size_t)read(_fd, &h, sizeof(h)) != sizeof(h))
		    {
		        return Result::OUT_OF_DATA;
		    }

            if (h.sanity != exo::msg::sanity)
            {
                return exo::Result::CORRUPTION;
            }

            assert(errno != EAGAIN);

		    return Result::OK;
        }

        exo::Result operator>>(exo::msg::PayloadBuffer const& pay)
        {
            return exo::nix::chunker::reader(_fd, (uint8_t*)pay.buf, pay.len);
        }

        exo::Result flush(size_t bytes)
        {
            char junk[1024];

            while (bytes > 0)
            {
                auto to_read = bytes > sizeof(junk) ? sizeof(junk) : bytes;
                auto bytes_read = read(_fd, &junk, to_read);

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
                memset(pay.buf, 0, pay.len);
                auto to_read = bytes > pay.len ? pay.len : bytes;
                auto bytes_read = read(_fd, pay.buf, to_read);

                if (bytes_read < 0)
                {
                    assert(errno != EAGAIN);

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
    private:
        int _fd = STDIN_FILENO;
    };
};

}
}
