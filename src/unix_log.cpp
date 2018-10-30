#include "exo.hpp"
#include "unix.hpp"

#include <unistd.h>
#include <time.h>

using namespace exo;

#define UNIX_TERM_GREEN "\033[0;32m"
#define UNIX_TERM_RED "\033[1;31m"
#define UNIX_TERM_YELLOW "\033[1;33m"
#define UNIX_TERM_COLOR_OFF "\033[0m"


exo::unix::Log::Stderr::Stderr(int verbosity, bool timestamp)
{
	verbosity_level = verbosity;
	_show_time = timestamp;
}


void exo::unix::Log::Stderr::log(Log::Type type, std::string& msg)
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
			str += sprintf(str, "%s", UNIX_TERM_YELLOW);
			break;
		case Log::Type::error:
			str += sprintf(str, "%s", UNIX_TERM_RED);
			break;
		case Log::Type::good:
			str += sprintf(str, "%s", UNIX_TERM_GREEN);
			break;
	}

	// write prefix
	str += sprintf(str, "[%s] ", proc_name);

	if (_show_time)
	str += sprintf(str, "@%ld: ", time(nullptr));

	// write main message
	str += sprintf(str, "%s", msg.c_str());

	// turn off coloring
	str += sprintf(str, "%s\n", UNIX_TERM_COLOR_OFF);
	write(STDERR_FILENO, buf, strlen(buf));
}
