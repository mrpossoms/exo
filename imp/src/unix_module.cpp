
#include "exo.hpp"
#include "unix.hpp"


int main(int argc, char* argv[])
{
    UNIX_MOD mod;
    exo::Result res = exo::OK;
    exo::msg::Inlet inlet(mod);
    exo::msg::Outlet outlet(mod, {});

    // call entry of module
    auto ctx = exo::Context{argc, argv};
    res = mod.enter(&ctx);

    // enter update loop
    while (true)
    {
        exo::msg::Hdr hdr;

        inlet >> hdr;

        if (mod.msg_compatible(hdr))
        {
            // TODO process result
            res = mod.msg_recieved(hdr, inlet);
        }

        // TODO process result
        res = mod.update();
    }

    // shutdown
    mod.exit();

    return 0;
}
