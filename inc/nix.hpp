#pragma once

#include <string.h>
#include <unistd.h>
#include <assert.h>

#include <iostream>
#include <functional>
#include <sstream>
#include <map>

#include "exo.hpp"

#include "nix/negotiator.hpp"
#include "nix/cli.hpp"
#include "nix/log.hpp"
#include "nix/pipeline.hpp"
#include "nix/net.hpp"
#include "nix/fs.hpp"

namespace exo
{

struct Context
{
    int argc;
    char** argv;
};

}
