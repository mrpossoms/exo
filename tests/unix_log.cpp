#include "exo.hpp"
#include "nix.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Test logging for nix implementions"

#include "test.h"
{
    exo::Log::inst(new exo::nix::Log::Stderr(1), 1);

    exo::Log::good(0, "This is good");
    exo::Log::info(0, "This is okay");
    exo::Log::warning(0, "This is trouble");
    exo::Log::error(0, "This is bad");
    exo::Log::info(2, "This shouldn't show up");

    exo::Log::info("Greeting") << "Hello info! " << "yo " << "yo " << "yo!";
    exo::Log::error("Greeting") << "Hello error!";
	exo::Log::good("Greeting") << "Hello good!";
	exo::Log::warning("Greeting") << "Hello warning!";

    return 0;
}
