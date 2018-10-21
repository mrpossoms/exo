#pragma once

#include <unordered_map>
#include <functional>
#include <sstream>

namespace exo
{
    namespace unix
    {
        struct CLI
        {
            CLI(unsigned int argc, char* argv[])
            {
                _argc = argc;
                _argv = argv;
            }

            template<typename T>
            CLI& optional(const char* name, std::function<void (T)> handler)
            {
                char* value = find_arg_val(name);

                if (value)
                {
                    T parsed;
                    std::stringstream ss(value);
                    ss >> parsed;

                    if(ss)
                    {
                        handler(parsed);
                    }
                }

                return *this;
            }

            template<typename T>
            CLI& required(const char* name, std::function<void (T)> handler)
            {
                char* value = find_arg_val(name);

                if (!value)
                {
                    // TODO shit the bed
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

                return *this;
            }

            static CLI parser(unsigned int argc, char* argv[])
            {
                return {argc, argv};
            }

        private:
            int _argc;
            char** _argv;

            char* find_arg_val(const char* name)
            {
                if (!name) return NULL;

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

                return NULL;
            }
        };
    }

    struct Context
    {
        std::string proc_name;
        int argc, char* argv[];
    };
}
