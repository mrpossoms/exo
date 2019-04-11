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

        exo::Result operator<<(exo::msg::PayloadBuffer&& pay)
        {
		    if ((size_t)write(STDOUT_FILENO, pay.buf, pay.len) == pay.len)
		    {
		        return Result::OK;
		    }

		    return Result::WRITE_ERR;
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

		    return Result::READ_ERR;
        }

        exo::Result operator>>(exo::msg::PayloadBuffer&& pay)
        {
		    if ((size_t)read(STDIN_FILENO, pay.buf, pay.len) == pay.len)
		    {
		        return Result::OK;
		    }

		    return Result::READ_ERR;
        }

        exo::Result flush(size_t bytes)
        {
    	    if (lseek(STDIN_FILENO, bytes, SEEK_CUR) > -1)
		    {
		        return Result::OK;
		    }

		    return Result::BAD;
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
                auto res = read(STDIN_FILENO, pay.buf, pay.len);

                if (res < 0)
                {
                    return Result::READ_ERR;
                }
                else
                {
                    bytes -= res;

                    for (int i = 0; outlets[i] != nullptr; ++i)
                    {
                        auto res = (*outlets[i]) << block.buffer();
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
