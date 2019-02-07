#include "exo.hpp"
#include "nix.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Test configuration for nix implementions"

#include "test.h"
{

    { // create, set, open and read foo
        std::string base_path = "/tmp/test/";
        exo::nix::data::Config cfg(base_path);

        cfg["foo"].preset("1337");

        int fd = open("/tmp/test/foo", O_RDONLY);
        assert(fd >= 0);

        char buf[128] = {};
        read(fd, buf, sizeof(buf));
        assert(atoi(buf) == 1337);
    }

    { // try to create a config in a place off limits
        std::string base_path = "/etc/test/";
        exo::nix::data::Config cfg(base_path);

        cfg["foo"].preset("1337");

        struct passwd* pw = getpwuid(getuid());
        const char* homedir = pw->pw_dir;
        assert(exo::nix::fs::exists(std::string(homedir) + "/etc/test/foo") == exo::Result::OK);

    }

    return 0;
}
