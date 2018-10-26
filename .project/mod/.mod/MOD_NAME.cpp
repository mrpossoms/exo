#include "exo.hpp"

struct MOD_NAME : public exo::Mod
{
    MOD_NAME() : exo::Mod("MOD_NAME")
    {
        // TODO
    }

    ~MOD_NAME()
    {
        // TODO
    }

    bool msg_compatible(exo::msg::Hdr& h)
    {
        // TODO

        return false;
    }

    exo::Result msg_recieved(exo::msg::Hdr& h, exo::msg::Payload& p)
    {
        // TODO
        return exo::OK;
    }

    template <typename T>
    exo::Result enter(T context)
    {
        // TODO
        return exo::OK;
    }

    exo::Result update()
    {
        // TODO
        return exo::OK;
    }

    exo::Result exit()
    {
        // TODO
        return exo::OK;
    }
};
