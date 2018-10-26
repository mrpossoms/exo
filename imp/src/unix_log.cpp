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

	if (proc_name == nullptr)
	{
		proc_name = getenv("_");
	}

	switch(type)
	{
		case Log::Type::info: break;
		case Log::Type::warning:
			write(STDERR_FILENO, UNIX_TERM_YELLOW, strlen(UNIX_TERM_YELLOW));
			break;
		case Log::Type::error:
			write(STDERR_FILENO, UNIX_TERM_RED, strlen(UNIX_TERM_RED));
			break;
		case Log::Type::good:
			write(STDERR_FILENO, UNIX_TERM_GREEN, strlen(UNIX_TERM_GREEN));
			break;
	}

	// write prefix
	dprintf(STDERR_FILENO, "[%s] ", proc_name);

	if (_show_time)
	dprintf(STDERR_FILENO, "@%d: ", time(nullptr));

	// write main message
	write(STDERR_FILENO, msg.c_str(), msg.length());
	write(STDERR_FILENO, "\n", 1);

	// turn off coloring
	write(STDERR_FILENO, UNIX_TERM_COLOR_OFF, strlen(UNIX_TERM_COLOR_OFF));
}
