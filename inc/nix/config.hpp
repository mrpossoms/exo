#pragma once

#include <libgen.h>

#include <string>
#include <iostream>
#include <fstream>
#include "../exo.hpp"

static void r_mkdir(const char *dir)
{
    char tmp[256];
    char *p = NULL;
    size_t len;
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;

    snprintf(tmp, sizeof(tmp), "%s", dir);
    len = strlen(tmp);

    if(tmp[len - 1] == '/') tmp[len - 1] = 0;

    for(p = tmp + 1; *p; p++)
    {
        if(*p == '/')
        {
            *p = 0;
            if (mkdir(tmp, mode) && errno != 17)
            {
                exo::Log::error(0, "Config: Failed to create " + std::string(tmp) + " errno (" + std::to_string(errno) + ")");
            }
            // chmod(tmp, mode)


            *p = '/';
        }
    }

    if (mkdir(tmp, mode))
    {
        exo::Log::error(0, "Config: Failed to create " + std::string(tmp) + " errno (" + std::to_string(errno) + ")");
    }
}

namespace exo
{

namespace nix
{

namespace data
{
/**
 * @brief Used for loading config values from non-volatile storage.
 */
struct Config
{
    std::string base_path;

    Config(std::string&& path)
    {
        base_path = path;
    }

    struct Value
    {
        /**
         * @brief unix style path locating this value
         */
        std::string path;

        /**
         * @brief C string default value
         */
        char* def_val;

        /**
         * @return true if this value didn't already exist.
         */
        bool is_new()
        {
            std::ifstream fs(path);
            return fs.fail() || fs.bad();
        }

        /**
         * @brief Original value for this configuration value
         * @param value string value to be used as the configuration preset
         * @return reference to the value instance
         */
        Value& preset(std::string value)
        {
            if (is_new())
            {
                (*this) = value;
            }

            return *this;
        }

        template<class T>
        T as()
        {
            T val;
            std::istringstream iss((*this)());

            iss >> val;

            return val;
        }

        /**
         * @param value new value to be stored at 'path'
         */
        void operator=(std::string value)
        {
            std::fstream fs;

            char tmp[PATH_MAX];
            strncpy(tmp, path.c_str(), PATH_MAX);
            auto parents = dirname(tmp);

            if (parents == nullptr)
            {
                exo::Log::error(0, "Config: failed to get dirname for " + std::string(path));
                return;
            }

            r_mkdir(parents);
            fs.open(path.c_str(), std::ios::out | std::ios::trunc);
            fs << value;
            fs.close();
        }

        /**
         * @return current value of 'path'
         */
        std::string operator()()
        {
            std::ifstream fs(path.c_str());

            if (fs.bad()) return "";

            std::string str((std::istreambuf_iterator<char>(fs)),
                             std::istreambuf_iterator<char>());

            return str;
        }
    };

    /**
     * @brief Loads or creates a new parameter at unix like path location.
     * @param path path to value, etc "/foo/bar/key"
     * @return The value for the named path
     */
    Value operator[](std::string&& path)
    {
        Value val;

        val.path = base_path + path;

        return val;
    }
};

}

}

}
