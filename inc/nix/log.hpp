#pragma once

#include "../exo.hpp"
#include "fs.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <libgen.h>

#include <unordered_map>

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
            char* proc_path = getenv("_");
			proc_name = ::basename(proc_path != nullptr ? proc_path : "log");
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

struct TopicFiles : public exo::Log
{
    TopicFiles(std::string const& path, bool timestamp=false)
    {
    	_files_path = path;
		_show_time = timestamp;

		exo::nix::fs::make_dirs(_files_path);
    }

    virtual ~TopicFiles()
    {
    	// clean-up pipe fd's and files
    	for (auto name_pipe : _files)
    	{
    		close(name_pipe.second);
    		unlink((_files_path + "/" + name_pipe.first).c_str());
    	}
    };

protected:
    virtual void log(Log::Type type, std::string const& msg, std::string const& topic)
    {
    	std::string topic_name = topic;
		char buf[512] = {};
		char* str = buf;

		if (topic_name.length() == 0)
		{
			topic_name = std::string(basename(getenv("_")));
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

		auto fd_itr = _files.find(topic_name);
		int fd = -1;

		if (fd_itr != _files.end())
		{
			fd = (*fd_itr).second;
		}
		else
		{
			auto fd_path = _files_path + "/" + topic_name;

			fd = open(fd_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
			if (fd < 0)
			{
				fprintf(stderr, "FATAL: Couldn't open named fd '%s' for logging (%s)\n", fd_path.c_str(), strerror(errno));
				exit(-2);
			}

			_files[topic_name] = fd;
		}

		if (_show_time)
		str += sprintf(str, "@%ld: ", time(nullptr));

		// write main message
		str += sprintf(str, "%s", msg.c_str());

		// turn off coloring
		str += sprintf(str, "%s\n", NIX_TERM_COLOR_OFF);
		write(fd, buf, strlen(buf));
    }

    virtual Log::Statement operator[](std::string const& topic)
    {
    	return { exo::Log::inst(), topic };
    }

private:
	std::unordered_map<std::string, int> _files;
	std::string _files_path;
    bool _show_time;
};

}

}

}
