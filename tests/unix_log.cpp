#include "exo.hpp"
#include "nix.hpp"

#include <iostream>
#include <fstream>

#define DESCRIPTION "Test logging for nix implementions"

void standard_logging()
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
}


void topic_file_logging()
{
    exo::Log::inst(new exo::nix::Log::TopicFiles("/tmp/exo-test-pipes"));

    exo::Log::info("info") << "Hello info! " << "yo " << "yo " << "yo!";
    exo::Log::error("error") << "Hello error!";
    exo::Log::good("good") << "Hello good!";
    exo::Log::warning("warning") << "Hello warning!";

    assert(exo::nix::fs::exists("/tmp/exo-test-pipes/info") == exo::Result::OK);
    assert(exo::nix::fs::exists("/tmp/exo-test-pipes/error") == exo::Result::OK);
    assert(exo::nix::fs::exists("/tmp/exo-test-pipes/good") == exo::Result::OK);
    assert(exo::nix::fs::exists("/tmp/exo-test-pipes/warning") == exo::Result::OK);
}


#include "test.h"
{
    umask(0000);

    standard_logging();
    topic_file_logging();

    return 0;
}
