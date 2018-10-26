#include "exo.hpp"
#include "../inc/exo.hpp"

using namespace exo;


Log* Log::instance(Log* logger, int log_level)
{
	static Log* inst;

	if (inst == nullptr && logger != nullptr)
	{
		inst = logger;
		inst->verbosity_level = log_level;
	}

	return inst;
}


void Log::good(int level, std::string&& msg)
{
	auto logger = Log::instance();
	if (logger != nullptr && level <= logger->verbosity_level)
	{
		logger->log(Log::Type::good, msg);
	}
}


void Log::info(int level, std::string&& msg)
{
	auto logger = Log::instance();
	if (logger != nullptr && level <= logger->verbosity_level)
	{
		logger->log(Log::Type::info, msg);
	}
}


void Log::warning(int level, std::string&& msg)
{
	auto logger = Log::instance();
	if (logger != nullptr && level <= logger->verbosity_level)
	{
		logger->log(Log::Type::warning, msg);
	}
}

void Log::error(int level, std::string&& msg)
{
	auto logger = Log::instance();
	if (logger != nullptr && level <= logger->verbosity_level)
	{
		logger->log(Log::Type::error, msg);
	}
}


msg::Hdr::Hdr(exo::msg::Inlet& is)
{
    is >> *this;
}


msg::Hdr msg::Hdr::from_inlet(Inlet &is)
{
    return { is };
}


// Result msg::Payload::seek_set(size_t pos);

Mod::Mod(ID&& name)
{
    _name = name;
}
/*
bool Mod::msg_compatible(msg::Hdr& h) { return false; }
Result Mod::msg_recieved(msg::Hdr& h, msg::Payload& p) { return {}; }
Result Mod::enter()  { return {}; }
Result Mod::update() { return {}; }
Result Mod::exit()   { return {}; }
*/
