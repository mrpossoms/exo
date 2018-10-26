#include "exo.hpp"

#ifdef __unix__
#include "unix.hpp"
#endif

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

    exo::Result msg_received(exo::msg::Hdr& h, exo::msg::Inlet& in)
    {
        // TODO
        return exo::OK;
    }

    exo::Result enter(exo::Context ctx)
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




#ifdef __unix__

int main(int argc, char* argv[])
{
    MOD_NAME mod;
    exo::Result res;
    int log_verbosity = 0;

    // Inlet and outlet setup
    exo::unix::Pipeline::In  inlet;
    exo::unix::Pipeline::Out outlet; 

    // Log level handling
    exo::unix::CLI::parser(argc, argv)
    .optional<int>("-v", [&](int verbosity) {
        log_verbosity = verbosity;
    });

    // instantiate stderr logger
    exo::Log::instance(new exo::unix::Log::Stderr(log_verbosity), log_verbosity);

    res = mod.enter({ argc, argv });

    if (res != exo::OK)
    {
        exo::Log::error(0, "MOD_NAME: enter() failed");
        return 1;
    }

    // process messages and perform updates until
    // mod.update() no longer returns exo::OK
    do
    {
        exo::msg::Hdr hdr;
        inlet >> hdr;

        // we've gotten a message, does the module
        // understand this message type?
        if (mod.msg_compatible(hdr))
        {
            // it does, process it
            mod.msg_received(hdr, inlet);
        }
        else
        {
            // it doesn't, discard the payload.
            inlet.flush(hdr.payload_length);
        }

    } while (mod.update() == exo::OK);

    // tear down
    mod.exit();

    // clean up the logger
    auto li = exo::Log::instance();
    if (li != nullptr) delete li;

    return 0;
}
#endif 
