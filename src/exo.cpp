#include "exo.hpp"

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


msg::Hdr::Hdr(uint32_t type, uint32_t magic, uint32_t pay_len)
{
	this->type = type;
	this->magic = magic;
	this->payload_length = pay_len;
}


bool msg::Hdr::operator==(exo::msg::Hdr&& h)
{
	return h.type == type && magic == h.magic;
}


Mod::Mod(ID&& name)
{
    _name = name;
}
