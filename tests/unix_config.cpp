#include "exo.hpp"
#include "nix.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Test configuration for nix implementions"

#include "test.h"
{
    std::string base_path = "/tmp/test/";
    exo::nix::data::Config cfg(base_path);

    cfg["foo"].preset("1337");

    {
	int fd = open("/tmp/test/foo", O_RDONLY);
	assert(fd >= 0);
	
	char buf[128] = {};
	read(fd, buf, sizeof(buf));
	assert(atoi(buf) == 1337);
    }

    return 0;
}
