#include "exo.hpp"
#include "nix/fs.hpp"

#include <iostream>
#include <fstream>


#define DESCRIPTION "Test file system functions"

#include "test.h"
{
    system("rm -rf /tmp/junk");

    mkdir("/tmp/junk", 0777);
    assert(exo::nix::fs::exists("/tmp/junk") == exo::Result::OK);

    system("touch /tmp/junk/file");
    system("mkdir /tmp/junk/dir");

    assert(exo::nix::fs::remove("/tmp/junk") == exo::Result::OK);
    
    return 0;
}
