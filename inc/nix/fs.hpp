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
    static Result exists(std::string const& path)
    {
    	struct stat statbuf;

    	switch (stat(path.c_str(), &statbuf))
    	{
    		case 0:       return Result::OK;
    		case ENOENT:  return Result::BAD;
    		default:      return Result::ERROR;
    	}
    }

    static Result remove(std::string const& path)
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

    static Result make_dirs(std::string const& path, mode_t mode=0777)
    {
        if (path.length() == 0) { return Result::OK; }

        for (int i = 0; i < path.size(); ++i)
        {
            if (path[i] == '/' || i == path.size() - 1)
            {
                auto stem = path.substr(0, i + 1);

                if (fs::exists(stem) != exo::Result::OK)
                {
                    if (mkdir(stem.c_str(), mode) != 0)
                    {
                        return Result::ERROR;
                    }
                }
            }
        }

        return Result::OK;
    } 
};

}

}
