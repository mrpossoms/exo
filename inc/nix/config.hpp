#pragma once

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>
#include <limits.h>
#include <pwd.h>

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "../exo.hpp"

static exo::Result r_mkdir(const char *dir)
{
    char tmp[256];
    char *p = NULL;
    size_t len;
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;

    snprintf(tmp, sizeof(tmp), "%s", dir);
    len = strlen(tmp);

    if (tmp[len - 1] == '/') tmp[len - 1] = 0;

    for (p = tmp + 1; *p; p++)
    {
        if (*p == '/')
        {
            *p = 0;
            if (mkdir(tmp, mode) && errno != EEXIST)
            {
                exo::Log::error(0, "Config: Failed to create " + std::string(tmp) + " errno (" + std::to_string(errno) + ")");
                goto escape;
            }

            *p = '/';
        }
    }

    if (mkdir(tmp, mode) && errno != EEXIST)
    {
        exo::Log::error(0, "Config: Failed to create " + std::string(tmp) + " errno (" + std::to_string(errno) + ")");
        goto escape;
    }

    return exo::Result::OK;

escape:
	switch (errno)
	{
		case EPERM: return exo::Result::NO_PERMISSION;
		case ENOENT: return exo::Result::BAD;
		default: return exo::Result::ERROR;
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

        //make sure the base path ends with a slash
        if (base_path.back() != '/') { base_path += "/"; }

        if (r_mkdir(path.c_str()) != exo::Result::OK)
        {
            struct passwd* pw = getpwuid(getuid());
            const char* homedir = pw->pw_dir;
            base_path = std::string(homedir) + path;
            exo::Log::warning(0, "Path not accessible, using " + base_path);
        }
    }

    Config(std::string& path)
    {
        base_path = path;

        if (r_mkdir(path.c_str()) != exo::Result::OK)
        {
            struct passwd* pw = getpwuid(getuid());
            const char* homedir = pw->pw_dir;
            base_path = std::string(homedir) + path;
            exo::Log::warning(0, "Path not accessible, using " + base_path);
        }
    }

    struct Value
    {
        /**
         * @brief unix style path locating this value
         */
        std::string path;

        /**
         * @brief string representation of the default value
         */
        std::string default_value;

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
            default_value = value;

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

        bool make_paths()
        {
            char tmp[PATH_MAX];
            strncpy(tmp, path.c_str(), PATH_MAX);
            auto parents = dirname(tmp);

            if (parents == nullptr)
            {
                exo::Log::error(0, "Config: failed to get dirname for " + std::string(path));
                return false;
            }

            r_mkdir(parents);
            return true;
        }

        /**
         * @param value new value to be stored at 'path'
         */
        void operator=(std::string value)
        {
            std::fstream fs;

            if (make_paths() == false) { return; }

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

            std::cerr << "default_value:" << default_value << std::endl;

            if (fs.fail()) return default_value;

            std::string str((std::istreambuf_iterator<char>(fs)),
                             std::istreambuf_iterator<char>());

            std::cerr << str << std::endl;

            return str;
        }

        template<class T>
        exo::Result get_blob(T& blob)
        {
            int fd = open(path.c_str(), O_RDONLY);
            if (fd == -1) { return exo::Result::DOES_NOT_EXIST; }

            auto size = sizeof(blob);
            if (read(fd, &blob, size) != size)
            {
                close(fd);
                return exo::Result::READ_ERR;
            }
            close(fd);

            return exo::Result::OK;
        }

        template<class T>
        exo::Result set_blob(T& blob)
        {
            if (is_new())
            {
                if (!make_paths()) { return exo::Result::RESOURCE_CREATE_FAILED; }
            }

            mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
            int fd = open(path.c_str(), O_WRONLY | O_CREAT, mode);

            if (fd > -1)
            {
                auto size = sizeof(blob);
                if (write(fd, &blob, size) != size)
                {
                    close(fd);
                    return exo::Result::WRITE_ERR;
                }
            }

            close(fd);

            return exo::Result::OK;
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
