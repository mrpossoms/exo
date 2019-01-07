#pragma once

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
    struct Value
    {
        /**
         * @brief unix style path locating this value
         */
        char* path;

        /**
         * @brief C string default value
         */
        char* def_val;

        /**
         * @return true if this value didn't already exist.
         */
        bool is_new();

        /**
         * @param value new value to be stored at 'path'
         */
        void operator=(std::string value);

        /**
         * @return current value of 'path'
         */
        std::string operator()();
    };

    /**
     * @brief Loads or creates a new parameter at unix like path location.
     * @param path path to value, etc "/foo/bar/key"
     * @return The value for the named path
     */
    Value operator[](char* path);

private:
    struct impl;
    impl* _pimpl;
};

}

}

}