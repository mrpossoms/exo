#pragma once

namespace exo
{
namespace nix
{
namespace chunker
{

static inline exo::Result writer(int fd, uint8_t* buf, size_t to_write)
{
    auto end = buf;

    // Allow the payload to be read in chunks at a time
    while (to_write > 0)
    {
        auto bytes_written = write(fd, end, to_write);

        switch(bytes_written)
        {
            case -1: return exo::Result::WRITE_ERR;
            case  0: return exo::Result::OUT_OF_DATA;
            default:
                to_write -= bytes_written;
                end += bytes_written;
        }
    }

    return exo::Result::OK;
}

static inline exo::Result reader(int fd, uint8_t* buf, size_t to_read)
{
    auto end = (uint8_t*)buf;

    // Allow the payload to be read in chunks at a time
    while (to_read > 0)
    {
        auto bytes_read = read(fd, end, to_read);

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

} // chunker

} // nix

} // eox