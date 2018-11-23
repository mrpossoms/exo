#pragma once

#include <string.h>
#include <unistd.h>

#include <iostream>
#include <functional>
#include <sstream>

namespace exo
{
    namespace unix
    {
        static void daemonize();

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
                        T parsed;
                        std::stringstream ss(value);
                        ss >> parsed;

                        if(ss)
                        {
                            handler(parsed);
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

        namespace Log
        {
            struct Stderr : public exo::Log
            {
                Stderr(int verbosity, bool timestamp=false);

            protected:
                virtual void log(Log::Type type, std::string& msg);

            private:
                bool _show_time;
            };
        }

        struct Pipeline
        {
            struct Out : public exo::msg::Outlet
            {
                exo::Result operator<<(exo::msg::Hdr&& h);
                exo::Result operator<<(exo::msg::PayloadBuffer&& buf);
            };

            struct In : public exo::msg::Inlet
            {
                exo::Result operator>>(exo::msg::Hdr& h);
                exo::Result operator>>(exo::msg::PayloadBuffer&& buf);
                exo::Result flush(size_t bytes);
            };
        };

        struct Net
        {
            struct Out : public exo::msg::Outlet
            {
                Out(const char* dst_addr, uint16_t port);
                ~Out();
                exo::Result operator<<(exo::msg::Hdr&& h);
                exo::Result operator<<(exo::msg::PayloadBuffer&& buf);

            private:
                struct impl;
                impl* _pimpl;
            };

            struct In : public exo::msg::Inlet
            {
                In(uint16_t port);
                ~In();
                exo::Result operator>>(exo::msg::Hdr& h);
                exo::Result operator>>(exo::msg::PayloadBuffer&& buf);
                exo::Result flush(size_t bytes);

            private:
                struct impl;
                impl* _pimpl;
            };
        };
    }

    struct Context
    {
        int argc;
        char** argv;
    };
}
