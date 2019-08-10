#pragma once

#include "../exo.hpp"

#include <unistd.h>
#include <time.h>

namespace exo
{
namespace nix
{
namespace Log
{

#define NIX_TERM_GREEN "\033[0;32m"
#define NIX_TERM_RED "\033[1;31m"
#define NIX_TERM_YELLOW "\033[1;33m"
#define NIX_TERM_COLOR_OFF "\033[0m"

struct Stderr : public exo::Log
{
    Stderr(int verbosity, bool timestamp=false)
    {
		verbosity_level = verbosity;
		_show_time = timestamp;
    }

    virtual ~Stderr() { };

protected:
    virtual void log(Log::Type type, std::string const& msg, std::string const& topic)
    {
		static char* proc_name;
		char buf[512] = {};
		char* str = buf;

		if (proc_name == nullptr)
		{
			proc_name = getenv("_");
		}

		switch(type)
		{
			case Log::Type::info: break;
			case Log::Type::warning:
				str += sprintf(str, "%s", NIX_TERM_YELLOW);
				break;
			case Log::Type::error:
				str += sprintf(str, "%s", NIX_TERM_RED);
				break;
			case Log::Type::good:
				str += sprintf(str, "%s", NIX_TERM_GREEN);
				break;
		}

		// write prefix
		str += sprintf(str, "[%s] ", proc_name);
		if (topic.length() > 0)
		{
			str += sprintf(str, "%s: ", topic.c_str());		
		}

		if (_show_time)
		str += sprintf(str, "@%ld: ", time(nullptr));

		// write main message
		str += sprintf(str, "%s", msg.c_str());

		// turn off coloring
		str += sprintf(str, "%s\n", NIX_TERM_COLOR_OFF);
		write(STDERR_FILENO, buf, strlen(buf));
    }

    virtual Log::Statement operator[](std::string const& topic)
    {
    	return { exo::Log::inst(), topic };
    }

private:
    bool _show_time;
};

}

}

}
