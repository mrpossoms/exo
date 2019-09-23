#include "exo.hpp"
#include "nix/fs.hpp"

#include <iostream>
#include <fstream>


#define DESCRIPTION "Test file system functions"

#include "test.h"
{
    (void)argc; (void)argv;
    system("rm -rf /tmp/junk");
    system("rm -rf /tmp/junker");

    mkdir("/tmp/junk", 0777);
    assert(exo::nix::fs::exists("/tmp/junk") == exo::Result::OK);
    assert(exo::nix::fs::exists("/tmp/junker") != exo::Result::OK);

    system("touch /tmp/junk/file");
    system("mkdir /tmp/junk/dir");

    assert(exo::nix::fs::remove("/tmp/junk") == exo::Result::OK);
    assert(exo::nix::fs::exists("/tmp/junk") != exo::Result::OK);

    assert(exo::nix::fs::make_dirs("/tmp/junker/junkiest") == exo::Result::OK);
    assert(exo::nix::fs::exists("/tmp/junker/junkiest") == exo::Result::OK);

    assert(exo::nix::fs::base_name("/tmp/junker") == "junker");

    return 0;
}
