#pragma once

#include "../exo.hpp"

namespace exo
{
namespace nix
{

struct Pipeline
{
    struct Out : public exo::msg::Outlet
    {
        exo::Result operator<<(exo::msg::Hdr&& h)
        {
		    if (write(STDOUT_FILENO, &h, sizeof(h)) == sizeof(h))
		    {
		        return Result::OK;
		    }

		    return Result::WRITE_ERR;
        }

        exo::Result operator<<(exo::msg::PayloadBuffer const& pay)
        {
            auto to_write = pay.len;
            auto end = (uint8_t*)pay.buf;

            // Allow the payload to be read in chunks at a time
            while (to_write > 0)
            {
                auto bytes_written = write(STDOUT_FILENO, end, to_write);

                switch(bytes_written)
                {
                    case -1: return exo::Result::WRITE_ERR;
                    case  0: return exo::Result::OUT_OF_DATA;
                    default:
                        to_write -= bytes_written;
                        end += bytes_written;
                }
            }

		    return Result::OK;
        }
    };

    struct In : public exo::msg::Inlet
    {
        exo::Result operator>>(exo::msg::Hdr& h)
        {
		    if ((size_t)read(STDIN_FILENO, &h, sizeof(h)) == sizeof(h))
		    {
		        return Result::OK;
		    }

            assert(errno != EAGAIN);

		    return Result::READ_ERR;
        }

        exo::Result operator>>(exo::msg::PayloadBuffer const& pay)
        {
            auto to_read = pay.len;
            auto end = (uint8_t*)pay.buf;

            // Allow the payload to be read in chunks at a time
            while (to_read > 0)
            {
                auto bytes_read = read(STDIN_FILENO, end, to_read);

                switch(bytes_read)
                {
                    case -1: return exo::Result::ERROR;
                    case  0: return exo::Result::OUT_OF_DATA;
                    default:
                        to_read -= bytes_read;
                        end += bytes_read;
                }
            }

		    return Result::OK;
        }

        exo::Result flush(size_t bytes)
        {
            char junk[1024];

            while (bytes > 0)
            {
                auto to_read = bytes > sizeof(junk) ? sizeof(junk) : bytes;
                auto bytes_read = read(STDIN_FILENO, &junk, to_read);

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
                auto bytes_read = read(STDIN_FILENO, pay.buf, to_read);

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
                        auto pay = block.buffer();
                        pay.len = bytes_read; // send only the bytes read

                        auto res = (*outlets[i]) << pay;
                        if (res != exo::Result::OK) { return res; }
                    }
                }
            }

            return Result::OK;
        }
    };
};

}
}
