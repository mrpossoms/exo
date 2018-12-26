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
		    if (write(STDOUT_FILENO, pay.buf, pay.len) == pay.len)
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
		    if (read(STDIN_FILENO, &h, sizeof(h)) == sizeof(h))
		    {
		        return Result::OK;
		    }

		    return Result::READ_ERR;
        }

        exo::Result operator>>(exo::msg::PayloadBuffer&& pay)
        {
		    if (read(STDIN_FILENO, pay.buf, pay.len) == pay.len)
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
    };
};

}
}