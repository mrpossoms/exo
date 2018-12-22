#pragma once

#include <string.h>
#include <unistd.h>
#include <assert.h>

#include <iostream>
#include <functional>
#include <sstream>
#include <map>

namespace exo
{
    namespace nix
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
                void timeout (unsigned int timeout_ms);
            private:
                struct impl;
                impl* _pimpl;
            };
        };

        struct Negotiator
        {
            Negotiator(uint32_t negotiation_timeout_ms)
            {

                fd_set rfds;
                struct timeval tv;

                FD_ZERO(&rfds);
                FD_SET(STDIN_FILENO, &rfds);

                tv.tv_sec = negotiation_timeout_ms / 1000;
                tv.tv_usec = (negotiation_timeout_ms % 1000) * 1000;

                switch (select(STDIN_FILENO + 1, &rfds, nullptr, nullptr, &tv))
                {
                    case 0:
                        exo::Log::error(1, "Negotiator: timeout");
                        break;
                    case -1:
                        exo::Log::error(1, "Negotiator: select() error");
                        break;
                    default:
                    {
                        // reads a key-value string from the previous negotiator
                        // of a format like "key0=value0;key1=value1;..."
                        char buf[2048];
                        size_t len = read(STDIN_FILENO, buf, sizeof(buf));
                        if (len > 0)
                        {
                            // replace all the semi-colons with null terminators
                            for (int i = len; i--; ) if (buf[i] == ';') { buf[i] = '\0'; }

                            char* kvp = buf;

                            while ((int)(kvp - static_cast<char*>(buf)) < len)
                            {
                                size_t kvp_len = strlen(kvp);
                                char *key = kvp, *value = nullptr;

                                // find the equal sign, replace it with a null terminator
                                int i = 0;
                                for (; kvp[i] != '=' && i < kvp_len; ++i);
                                if (kvp[i] == '=')
                                {
                                    kvp[i] = '\0';
                                    value = kvp + i + 1;
                                }

                                if (value)
                                {
                                    kvp = value + strlen(value) + 1;
                                    _demand_args[std::string(key)] = std::string(value);
                                }
                            }
                        }
                    } break;
                }
            }

            Negotiator& react_to(std::string key, std::function<void (std::string&)> handler)
            {
                if (_demand_args.find(key) != _demand_args.end()) { handler(_demand_args[key]); }
                return *this;
            }

            Negotiator& request(std::string key, std::string value)
            {
                _request_string += key + "=" + value + ";";
                return *this;
            }

            Negotiator& send_request()
            {
                auto written = write(STDOUT_FILENO, _request_string.c_str(), _request_string.length());
                assert(written == _request_string.length());
                return *this;
            }

            static Negotiator instance(uint32_t negotiation_timeout_ms)
            {
                return { negotiation_timeout_ms };
            }

        private:
            std::map<std::string, std::string> _demand_args;
            std::string _request_string;
        };

        namespace fs
        {
            exo::Result exists(std::string path);
        }
    }

    struct Context
    {
        int argc;
        char** argv;
    };
}
