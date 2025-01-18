#include "Log.h"

#include <fstream>
#include <iostream>

namespace vimlock
{

class Logger
{
public:
	Logger()
	{
		stream.open("log.txt", std::ios::out | std::ios::app);
	}

	static Logger & instance()
	{
		static Logger singleton;
		return singleton;
	}

	std::fstream stream;
};

void logInfo(const std::string &msg)
{
	Logger::instance().stream << "[info]  " << msg << std::endl;
	std::cerr << "[info]  " << msg << std::endl;
}

void logError(const std::string &msg)
{
	Logger::instance().stream << "[error] " << msg << std::endl;
	std::cerr << "[error] " << msg << std::endl;
}

} // namespace vimlock
