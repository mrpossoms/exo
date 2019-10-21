#pragma once

#include "../exo.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <fstream>
#include <functional>

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

    static Result modified_time(std::string const& path, struct timespec& time)
    {
        struct stat statbuf;

        switch (stat(path.c_str(), &statbuf))
        {
            case 0:       return Result::OK;
            case ENOENT:  return Result::BAD;
            default:      return Result::ERROR;
        }

        time = statbuf.st_mtimespec;

        return Result::OK;
    }

    static exo::Result foreach_in_path(
            std::string const& path,
            std::function<exo::Result (std::string const& ent_name, char d_type)> cb)
    {
        DIR* dir = opendir(path.c_str());

        if (nullptr == dir)
        {
            return exo::Result::ERROR;
        }

        for(dirent* ent = readdir(dir); nullptr != ent; ent = readdir(dir))
        {
            auto res = cb(std::string(ent->d_name), ent->d_type);

            if (exo::Result::DONE == res) { break; }
            if (exo::Result::OK != res) { return res; }
        }

        return exo::Result::OK;
    }

    static std::string read_file(std::string const& path)
    {
        std::string contents;
        std::ifstream file(path.c_str(), std::ifstream::in | std::ifstream::binary);
        file >> contents;
        file.close();

        return contents;
    }

    static Result remove(std::string const& path)
    {
        auto res = foreach_in_path(path, [&](std::string const& ent_name, char d_type) -> exo::Result
        {
            if (ent_name == "." || ent_name == "..")
            { return exo::Result::OK; }

            auto ent_path = path + "/" + ent_name;
            if (d_type & DT_DIR)
            {
                if (remove(ent_path) != exo::Result::OK)
                {
                    return exo::Result::ERROR;
                }
            }
            else if (d_type & DT_REG)
            {
                unlink(ent_path.c_str());
            }

            return exo::Result::OK;
        });

        rmdir(path.c_str());
        return res;
    }

    static std::string base_name(std::string const& path)
    {
        auto idx = path.find_last_of('/');
        if (std::string::npos == idx) { return ""; }
        idx += 1;
        return path.substr(idx, path.length() - idx);
    }

    static Result make_dirs(std::string const& path, mode_t mode=0777)
    {
        if (path.length() == 0) { return Result::OK; }

        for (size_t i = 0; i < path.size(); ++i)
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
