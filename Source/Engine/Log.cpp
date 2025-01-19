#include "Log.h"
#include "Format.h"

#include <fstream>
#include <iostream>

namespace vimlock
{

class Logger
{
public:
	Logger():
		stream(std::cerr)
	{
	}

	Logger(const std::string &file_):
		file(file_, std::ios::out),
		stream(file)
	{
	}

	static Logger & instance()
	{
		static Logger singleton("log.txt");
		return singleton;
	}

	std::fstream file;
	std::ostream &stream;
};

void logInfo(const std::string &msg)
{
	Logger::instance().stream << "[info]  " << msg << std::endl;
}

void logInfo(const std::string &msg, const Board &board)
{
	BoardTerminalFormatter fmt;
	fmt.setBoard(board);

	Logger::instance().stream << "[info]  " << msg << std::endl;
	Logger::instance().stream << fmt.toString() << std::endl;
	Logger::instance().stream.flush();
}

void logInfo(const std::string &msg, const Board &board, const Bitboard &bitboard)
{
	BoardTerminalFormatter fmt;
	fmt.setBoard(board);
	fmt.setBitboard(bitboard);

	Logger::instance().stream << "[info]  " << msg << std::endl;
	Logger::instance().stream << fmt.toString() << std::endl;
	Logger::instance().stream.flush();
}

void logError(const std::string &msg)
{
	Logger::instance().stream << "[error] " << msg << std::endl;
	std::cerr << "[error] " << msg << std::endl;
}

} // namespace vimlock
