#include "exo.hpp"
#include "nix.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Test logging for nix implementions"

#include "test.h"
{
    exo::Log::instance(new exo::nix::Log::Stderr(1), 1);

    exo::Log::good(0, "This is good");
    exo::Log::info(0, "This is okay");
    exo::Log::warning(0, "This is trouble");
    exo::Log::error(0, "This is bad");
    exo::Log::info(2, "This shouldn't show up");

    return 0;
}
