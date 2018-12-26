#pragma once

// #include "exo.hpp"

namespace exo
{
namespace nix
{

struct CLI
{
    CLI(unsigned int argc, char* argv[])
    {
        _argc = argc;
        _argv = argv;
    }

    template<typename T>
    CLI& optional(const char* name, std::function<void (T)> handler, const char* desc="")
    {
        if (_show_help)
        {
            std::cerr << name << ": (optional) " << desc << std::endl;
        }
        else
        {
            char* value = find_arg_val(name);

            if (value)
            {
                if  (typeid(T) == typeid(bool))
                {
                    handler({});
                }
                else
                {
                    T parsed;
                    std::stringstream ss(value);
                    ss >> parsed;

                    if(ss)
                    {
                        handler(parsed);
                    }
                }
            }
        }

        return *this;
    }

    template<typename T>
    CLI& required(const char* name, std::function<void (T)> handler, const char* desc="")
    {
        if (_show_help)
        {
            std::cerr << name << ": (required) " << desc << std::endl;
        }
        else
        {
            char* value = find_arg_val(name);

            if (!value)
            {
                // TODO shit the bed
                std::cerr << "Required argument '" << name << "' is missing." << std::endl;
                abort();
                return *this;
            }

            T parsed;
            std::stringstream ss(value);
            ss >> parsed;

            if(ss)
            {
                handler(parsed);
            }
            else
            {
                // TODO parse error
            }
        }

        return *this;
    }

    CLI& help(const char* help_flag, const char* program_desc)
    {
        _show_help = find_arg_val(help_flag) != nullptr;

        if (_show_help)
        {
            std::cerr << _argv[0] << std::endl << std::endl;
            std::cerr << program_desc << std::endl << std::endl;
            std::cerr << "ARGUMENTS:" << std::endl;
        }

        return *this;
    }

    static CLI parser(unsigned int argc, char* argv[])
    {
        return {argc, argv};
    }

private:
    int _argc;
    char** _argv;
    bool _show_help = 0;

    char* find_arg_val(const char* name)
    {
        if (!name) return nullptr;

        auto name_len = strlen(name);

        for (auto i = _argc; i--;)
        {
            if (!strncmp(_argv[i], name, name_len))
            {
                // clip off any assignment characters
                auto val = _argv[i] + name_len;
                while(val[0] == '=') val++;
                return val;
            }
        }

        return nullptr;
    }
};

}
}