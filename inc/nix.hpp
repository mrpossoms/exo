#pragma once

#include <string.h>
#include <unistd.h>
#include <assert.h>

#include <iostream>
#include <functional>
#include <sstream>
#include <map>

#include "exo.hpp"

#include "nix/negotiator.hpp"
#include "nix/cli.hpp"
#include "nix/log.hpp"
#include "nix/pipeline.hpp"
#include "nix/net.hpp"
#include "nix/fs.hpp"

namespace exo
{
namespace nix
{

        // struct Pipeline
        // {
        //     struct Out : public exo::msg::Outlet
        //     {
        //         exo::Result operator<<(exo::msg::Hdr&& h);
        //         exo::Result operator<<(exo::msg::PayloadBuffer&& buf);
        //     };

        //     struct In : public exo::msg::Inlet
        //     {
        //         exo::Result operator>>(exo::msg::Hdr& h);
        //         exo::Result operator>>(exo::msg::PayloadBuffer&& buf);
        //         exo::Result flush(size_t bytes);
        //     };
        // };

        // struct Net
        // {
        //     struct Out : public exo::msg::Outlet
        //     {
        //         Out(const char* dst_addr, uint16_t port);
        //         ~Out();
        //         exo::Result operator<<(exo::msg::Hdr&& h);
        //         exo::Result operator<<(exo::msg::PayloadBuffer&& buf);

        //     private:
        //         struct impl;
        //         impl* _pimpl;
        //     };

        //     struct In : public exo::msg::Inlet
        //     {
        //         In(uint16_t port);
        //         ~In();
        //         exo::Result operator>>(exo::msg::Hdr& h);
        //         exo::Result operator>>(exo::msg::PayloadBuffer&& buf);
        //         exo::Result flush(size_t bytes);
        //         void timeout (unsigned int timeout_ms);
        //     private:
        //         struct impl;
        //         impl* _pimpl;
        //     };
        // };

        // namespace fs
        // {
        //     exo::Result exists(std::string path);
        // }

}

struct Context
{
    int argc;
    char** argv;
};

}
