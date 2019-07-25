#pragma once

#include "../exo.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

namespace exo
{
namespace nix
{

struct fs
{
    static Result exists(std::string path)
    {
    	struct stat statbuf;

    	switch (stat(path.c_str(), &statbuf))
    	{
    		case 0:       return Result::OK;
    		case ENOENT:  return Result::BAD;
    		default:      return Result::ERROR;
    	}
    }

    static Result remove(std::string path)
    {
        auto res = exo::Result::OK;
        DIR* dir = opendir(path.c_str());

        if (nullptr == dir)
        {
            closedir(dir);
            return exo::Result::ERROR;
        }

        for(dirent* ent = readdir(dir); nullptr != ent; ent = readdir(dir))
        {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            { continue; }

            auto ent_path = path + "/" + std::string(ent->d_name);
            if (ent->d_type & DT_DIR)
            {
                res = remove(ent_path);
                if (res != exo::Result::OK)
                {
                    closedir(dir);
                    return exo::Result::ERROR;
                }
            }
            else if (ent->d_type & DT_REG)
            {
                unlink(ent_path.c_str());
            }
        }

        rmdir(path.c_str());
        closedir(dir);
        return res;
    }
};

}

}
