#pragma once

#include "../exo.hpp"

namespace exo
{
namespace nix
{

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
                    for (size_t i = len; i--; ) if (buf[i] == ';') { buf[i] = '\0'; }

                    char* kvp = buf;

                    while ((size_t)(kvp - static_cast<char*>(buf)) < len)
                    {
                        size_t kvp_len = strlen(kvp);
                        char *key = kvp, *value = nullptr;

                        // find the equal sign, replace it with a null terminator
                        size_t i = 0;
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
        assert((unsigned long)written == (unsigned long)_request_string.length());
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

}
}
