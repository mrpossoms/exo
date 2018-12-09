#include "exo.hpp"
#include "nix.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace exo::nix;
using namespace exo;

Result fs::exists(std::string path)
{
	struct stat statbuf;

	switch (stat(path.c_str(), &statbuf))
	{
		case 0:       return Result::OK;
		case ENOENT:  return Result::BAD;
		default:      return Result::ERROR;
	}
}